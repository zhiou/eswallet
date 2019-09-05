//
//  thread.hpp
//  transmit
//
//  Created by zhiou on 2019/9/3.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>

#include "spinlock.h"
#include "any.hpp"
#include "thread.hpp"


typedef pthread_t thread_tid;

namespace excelsecu {
    class ThreadUtil {
    public:
        static void yield() {
            sched_yield();
        }
        
        static thread_tid current_thread_id() {
            return pthread_self();
        }
        
        static bool isrunning(thread_tid _id) {
            int ret = pthread_kill(_id, 0);
            
            if (0 == ret)
                return true;
            else if (ESRCH == ret)
                return false;
            else if (EINVAL == ret)
                assert(false);
            
            return false;
        }
        
        static int join(thread_tid _id) {
            if (_id == current_thread_id()) return EDEADLK;
            int ret = pthread_join(_id, 0);
            return ret;
        }
    };
    
    struct Runnable {
        virtual ~Runnable() {}
        virtual void run() = 0;
    };
    
    namespace detail {
        
        template <class T>
        class RunnableFunctor : public Runnable {
        public:
            RunnableFunctor(const T& f) : func_(f) {}
            virtual void run() { func_(); }
        private:
            T func_;
        };
        
        template <class T>
        class RunnableFunctor<T*> : public Runnable {
        public:
            RunnableFunctor(T* f) : func_(f) {}
            virtual void run() { (*func_)(); }
            
        private:
            RunnableFunctor(const RunnableFunctor&);
            RunnableFunctor& operator=(const RunnableFunctor&);
            
        private:
            T* func_;
        };
        
        template <>
        class RunnableFunctor<Runnable> : public Runnable {
            RunnableFunctor();
        };
        
        template <>
        class RunnableFunctor<Runnable*> : public Runnable {
        public:
            RunnableFunctor(Runnable* f) : func_(f) {}
            virtual void run() { static_cast<Runnable*>(func_)->run();}
            
        private:
            RunnableFunctor(const RunnableFunctor&);
            RunnableFunctor& operator=(const RunnableFunctor&);
            
        private:
            Runnable* func_;
        };
        
        // base template for no argument functor
        template <class T>
        struct TransformImplement {
            static Runnable* transform(const T& t) {
                return new RunnableFunctor<T>(t);
            }
        };
        
        template <class T>
        inline Runnable* transform(const T& t) {
            return TransformImplement<T>::transform(t);
        }
        
    }
    
    class Thread {
    private:
        class RunnableReference {
        public:
            RunnableReference(Runnable* _target)
            : target(_target), count(0), tid(0), isjoined(false), isended(true)
            , aftertime(LONG_MAX), periodictime(LONG_MAX), iscanceldelaystart(false)
            , condtime(), splock(), isinthread(false), killsig(0) {
                memset(thread_name, 0, sizeof(thread_name));
            }
            
            ~RunnableReference() {
                delete target;
                assert(0 == count);
                assert(isended);
            }
            
            void AddRef() { count++;}
            void RemoveRef(SpinLock& _lock) {
                assert(0 < count);
//                assert(_lock.islocked());
                
                bool willdel = false;
                count--;
                
                if (0 == count) willdel = true;
                
                _lock.unlock();
                
                if (willdel) delete this;
            }
            
        private:
            RunnableReference(const RunnableReference&);
            RunnableReference& operator=(const RunnableReference&);
            
        public:
            Runnable* target;
            int count;
            thread_tid tid;
            bool isjoined;
            bool isended;
            long aftertime;
            long periodictime;
            bool iscanceldelaystart;
            std::condition_variable condtime;
            std::mutex mutex;
            SpinLock splock;
            bool isinthread;  // 猥琐的东西，是为了解决线程还没有起来的时就发送信号出现crash的问题
            int killsig;
            char thread_name[128];
        };
        
    public:
        template<class T>
        explicit Thread(const T& op, const char* _thread_name = NULL, bool _outside_join = false)
        : runable_ref_(NULL), outside_join_(_outside_join) {
            runable_ref_ = new RunnableReference(detail::transform(op));
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            runable_ref_->AddRef();
            
            int res = pthread_attr_init(&attr_);
            assert(res == 0);
            
            if (_thread_name) strncpy(runable_ref_->thread_name, _thread_name, sizeof(runable_ref_->thread_name));
        }
        
        Thread(const char* _thread_name = NULL, bool _outside_join = false)
        : runable_ref_(NULL), outside_join_(_outside_join) {
            runable_ref_ = new RunnableReference(NULL);
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            runable_ref_->AddRef();
            
            int res = pthread_attr_init(&attr_);
            assert(res == 0);
            
            if (_thread_name) strncpy(runable_ref_->thread_name, _thread_name, sizeof(runable_ref_->thread_name));
        }
        
        virtual ~Thread() {
            int res = pthread_attr_destroy(&attr_);
            assert(res == 0);
            
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            if (0 != runable_ref_->tid && !runable_ref_->isjoined) pthread_detach(runable_ref_->tid);
//            runable_ref_->RemoveRef(lock);
        }
        
        int start(bool* _newone = NULL) {
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            
            if (_newone) *_newone = false;
            
            if (is_runing())return 0;
            if (0 != runable_ref_->tid && !runable_ref_->isjoined) pthread_detach(runable_ref_->tid);
            
            assert(runable_ref_->target);
            runable_ref_->isended = false;
            runable_ref_->isjoined = outside_join_;
            runable_ref_->AddRef();
            
            int ret =  pthread_create(reinterpret_cast<thread_tid*>(&runable_ref_->tid), &attr_, start_routine, runable_ref_);
            assert(0 == ret);
            
            if (_newone) *_newone = true;
            
            if (0 != ret) {
                runable_ref_->isended = true;
//                runable_ref_->RemoveRef(lock);
            }
            
            return ret;
        }
        
        template <typename T>
        int start(const T& op, bool* _newone = NULL) {
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            
            if (_newone) *_newone = false;
            
            if (is_runing())return 0;
            if (0 != runable_ref_->tid && !runable_ref_->isjoined) pthread_detach(runable_ref_->tid);
            
            delete runable_ref_->target;
            runable_ref_->target = detail::transform(op);
            
            runable_ref_->isended = false;
            runable_ref_->isjoined = outside_join_;
            runable_ref_->AddRef();
            
            int ret =  pthread_create(reinterpret_cast<thread_tid*>(&runable_ref_->tid), &attr_, start_routine, runable_ref_);
            assert(0 == ret);
            
            if (_newone) *_newone = true;
            
            if (0 != ret) {
                runable_ref_->isended = true;
//                runable_ref_->RemoveRef(lock);
            }
            
            return ret;
        }
        
        int start_after(long after) {
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            
            if (is_runing())return 0;
            if (0 != runable_ref_->tid && !runable_ref_->isjoined) pthread_detach(runable_ref_->tid);
            
            assert(runable_ref_->target);
//            runable_ref_->condtime.cancelAnyWayNotify();
            runable_ref_->isjoined = outside_join_;
            runable_ref_->isended = false;
            runable_ref_->aftertime = after;
            runable_ref_->iscanceldelaystart = false;
            runable_ref_->AddRef();
            
            int ret =  pthread_create(reinterpret_cast<thread_tid*>(&runable_ref_->tid), &attr_, start_routine_after, runable_ref_);
            assert(0 == ret);
            
            if (0 != ret) {
                runable_ref_->isended = true;
                runable_ref_->aftertime = LONG_MAX;
//                runable_ref_->RemoveRef(lock);
            }
            
            return ret;
        }
        
        void cancel_after() {
//            SpinLock lock(runable_ref_->splock);
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            
            if (!is_runing()) return;
            
            runable_ref_->iscanceldelaystart = true;
            runable_ref_->condtime.notify_all();
        }
        
        int start_periodic(long after, long periodic) { // ms
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            
            if (is_runing()) return 0;
            if (0 != runable_ref_->tid && !runable_ref_->isjoined) pthread_detach(runable_ref_->tid);
            
            assert(runable_ref_->target);
//            runable_ref_->condtime.cancelAnyWayNotify();
            runable_ref_->isended = false;
            runable_ref_->isjoined = outside_join_;
            runable_ref_->iscanceldelaystart = false;
            runable_ref_->aftertime = after;
            runable_ref_->periodictime = periodic;
            runable_ref_->AddRef();
            
            int ret = pthread_create(reinterpret_cast<thread_tid*>(&runable_ref_->tid), &attr_, start_routine_periodic, runable_ref_);
            assert(0 == ret);
            
            if (0 != ret) {
                runable_ref_->isended = true;
                runable_ref_->aftertime = LONG_MAX;
                runable_ref_->periodictime = LONG_MAX;
//                runable_ref_->RemoveRef(lock);
            }
            
            return ret;
        }
        
        void cancel_periodic() {
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            
            if (!is_runing()) return;
            
            runable_ref_->iscanceldelaystart = true;
            runable_ref_->condtime.notify_all();
        }
        
        int join() const {
            int ret = 0;
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            assert(!outside_join_);
            assert(!runable_ref_->isjoined);
            
            if (tid() == ThreadUtil::current_thread_id()) return EDEADLK;
            
            if (is_runing()) {
                runable_ref_->isjoined = true;
                lock.unlock();
                ret = pthread_join(tid(), 0);
                
            }
            
            return ret;
        }
        
        int kill(int sig) const {
            std::unique_lock<SpinLock> lock(runable_ref_->splock);
            
            if (!is_runing()) return ESRCH;
            
            if (!runable_ref_->isinthread) {
                runable_ref_->killsig = sig;
                return 0;
            }
            
            lock.unlock();
            
            int ret = pthread_kill(tid(), sig);
            return ret;
        }
        
        int unsafe_exit() const {
#ifdef ANDROID
            struct sigaction actions;
            memset(&actions, 0, sizeof(actions));
            sigemptyset(&actions.sa_mask);
            actions.sa_flags = 0;
            actions.sa_handler = &exit_handler;
            sigaction(SIGUSR2, &actions, NULL);
            assert2(false, "In Android, use SIGUSR2(handler call pthread_exit) to pthread_cancel");
            return kill(SIGUSR2);
#else
            return pthread_cancel(tid());
#endif
        }
        
        thread_tid tid() const {
            return runable_ref_->tid;
        }
        
        bool is_runing() const {
            return !runable_ref_->isended;
        }
        
        void stack_size(size_t _stacksize) {
            if (_stacksize == 0) return;
            
            int res = pthread_attr_setstacksize(&attr_, _stacksize);
            assert(res == 0);
           
        }
        
        size_t stack_size() const {
            size_t _stacksize = 0;
            int res = pthread_attr_getstacksize(&attr_, &_stacksize);
            assert(res == 0);
            return _stacksize;
        }
        
        const char* thread_name() const {
            return runable_ref_->thread_name;
        }
        
    private:
        
#ifdef ANDROID
        static void exit_handler(int _sig) {
            pthread_exit(0);
        }
#endif
        
        static void init(void* arg) {
            volatile RunnableReference* runableref = static_cast<RunnableReference*>(arg);
            std::unique_lock<SpinLock> lock((const_cast<RunnableReference*>(runableref))->splock);
            assert(runableref != 0);
            assert(runableref->target != 0);
            assert(!runableref->isinthread);
            
            runableref->isinthread = true;
            
            if (0 < strnlen((const char*)runableref->thread_name, sizeof(runableref->thread_name))) {
#ifdef __APPLE__
                pthread_setname_np((const char*)runableref->thread_name);
#elif defined(ANDROID)
                pthread_setname_np(runableref->tid, (const char*)runableref->thread_name);
#else
                
#endif
            }
            
            if (!(0 < runableref->killsig && runableref->killsig <= 32))
                return;
            
            lock.unlock();
            pthread_kill(pthread_self(), runableref->killsig);
        }
        
        static void cleanup(void* arg) {
            volatile RunnableReference* runableref = static_cast<RunnableReference*>(arg);
            std::unique_lock<SpinLock> lock((const_cast<RunnableReference*>(runableref))->splock);
            
            assert(runableref != 0);
            assert(runableref->target != 0);
            assert(runableref->tid != 0);
            assert(runableref->isinthread);
            
            runableref->isinthread = false;
            runableref->killsig = 0;
            runableref->isended = true;
            
//            (const_cast<RunnableReference*>(runableref))->RemoveRef(lock);
        }
        
        static void* start_routine(void* arg) {
            init(arg);
            volatile RunnableReference* runableref = static_cast<RunnableReference*>(arg);
            pthread_cleanup_push(&cleanup, arg);
            runableref->target->run();
            pthread_cleanup_pop(1);
            return 0;
        }
        
        static void* start_routine_after(void* arg) {
            init(arg);
            volatile RunnableReference* runableref = static_cast<RunnableReference*>(arg);
            pthread_cleanup_push(&cleanup, arg);
            
            if (!runableref->iscanceldelaystart) {
                auto time = std::chrono::milliseconds(runableref->aftertime);
                std::unique_lock<std::mutex> lock;
                auto& ct = (const_cast<RunnableReference*>(runableref))->condtime;

                ct.wait_for(lock, time);
                
                if (!runableref->iscanceldelaystart)
                    runableref->target->run();
            }
            
            pthread_cleanup_pop(1);
            return 0;
        }
        
        static void* start_routine_periodic(void* arg) {
            init(arg);
            volatile RunnableReference* runableref = static_cast<RunnableReference*>(arg);
            pthread_cleanup_push(&cleanup, arg);
            
            if (!runableref->iscanceldelaystart) {
                auto time = std::chrono::milliseconds(runableref->aftertime);
                std::unique_lock<std::mutex> lock;
                (const_cast<RunnableReference*>(runableref))->condtime.wait_for(lock, time);
                
                while (!runableref->iscanceldelaystart) {
                    runableref->target->run();
                    auto time = std::chrono::milliseconds(runableref->periodictime);
                    if (!runableref->iscanceldelaystart)
                        (const_cast<RunnableReference*>(runableref))->condtime.wait_for(lock, time);
                }
            }
            
            pthread_cleanup_pop(1);
            return 0;
        }
        
    private:
        Thread(const Thread&);
        Thread& operator=(const Thread&);
        
    private:
        RunnableReference*  runable_ref_;
        pthread_attr_t attr_;
        bool outside_join_;
    };
    
    
    inline bool operator==(const Thread& lhs, const Thread& rhs) {
        return pthread_equal(lhs.tid(), rhs.tid()) != 0;
    }
    
    inline bool operator!=(const Thread& lhs, const Thread& rhs) {
        return pthread_equal(lhs.tid(), rhs.tid()) == 0;
    }

}


