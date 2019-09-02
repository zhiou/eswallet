//
//  message_queue.cpp
//  transmit
//
//  Created by zhiou on 2019/8/28.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#include "message_queue.hpp"

#include <list>
#include <map>

namespace excelsecu {
    
    #define MAX_MQ_SIZE 5000
    
    static unsigned int make_seq() {
        static unsigned int s_seq = 0;
        return ++s_seq;
    }
    
struct message_wrapper {
    message_wrapper(const message_handler_t& _handlerid, const message_t& _message, const message_timing& _timing, unsigned int _seq)
    : message(_message), timing(_timing)
    {
        postid.reg = _handlerid;
        postid.seq = _seq;
        period_status = kImmediately;
        record_time = 0;
        
        if (kImmediately != _timing.type) {
            period_status = kAfter;
            record_time = utility::get_tick_count();
        }
    }
    
    ~message_wrapper() {
        if (wait_end_cond)
            wait_end_cond->notify_all();
    }
    
    message_post_t postid;
    message_t message;
    
    message_timing timing;
    message_timing_t period_status;
    uint64_t record_time;
    std::shared_ptr<std::condition_variable> wait_end_cond;
};

struct handler_wrapper {
    handler_wrapper(const message_handler& _handler, bool _recvbroadcast, const message_queue_t& _messagequeueid, unsigned int _seq)
    : handler(_handler), recvbroadcast(_recvbroadcast) {
        reg.seq = _seq;
        reg.queue = _messagequeueid;
    }
    
    message_handler_t reg;
    message_handler handler;
    bool recvbroadcast;
};

struct runloop_info {
    runloop_info(): runing_message(nullptr)
    {runing_cond = std::make_shared<std::condition_variable>();}
    
    std::shared_ptr<std::condition_variable> runing_cond;
    message_post_t runing_message_id;
    message_t* runing_message;
    std::list <message_handler_t> runing_handler;
};

class Cond: public runloop_cond {
public:
    Cond() = default;
public:
    const std::type_info& type() const {
        return typeid(Cond);
    }
    
    virtual void wait(std::unique_lock<std::mutex>& _lock, long _millisecond) {
        auto to = std::chrono::milliseconds(_millisecond);
        cond_.wait_for(_lock, to);
    }
    
    virtual void notify(std::unique_lock<std::mutex>& _lock) {
        cond_.notify_all();
    }
private:
    Cond(const Cond&);
    Cond& operator=(const Cond&);
private:
    std::condition_variable cond_;
};

struct message_queue_content {
    message_queue_content(): breakflag(false) {}
    
    message_handler_t invoke_reg;
    bool breakflag;
    std::shared_ptr<runloop_cond> breaker;
    std::list<message_wrapper*> lst_msg;
    std::list<handler_wrapper*> lst_handler;
    std::list<runloop_info> lst_runloop_info;
private:
    message_queue_content(const message_queue_content&);
    message_queue_content& operator=(const message_queue_content&);
};

static std::mutex& messagequeue_map_mutex() {
    static std::mutex* mutex = new std::mutex;
    return *mutex;
}
    
#define sg_messagequeue_map_mutex messagequeue_map_mutex()
    
static std::map<message_queue_t, message_queue_content>& messagequeue_map() {
    static auto *mq_map = new std::map<message_queue_t, message_queue_content>;
    return *mq_map;
}
#define sq_messagequeue_map messagequeue_map()
    
    static std::string dump_message(const std::list<message_wrapper*>& _message_list) {
        return "";
    }
    
    std::string dump_mq(const message_queue_t& _msg_queue_id) {
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        const message_queue_t& id = _msg_queue_id;
        
        auto pos = sq_messagequeue_map.find(id);
        if (pos == std::end(sq_messagequeue_map))
        {
            return "";
        }
        message_queue_content& content = pos->second;
        return dump_message(content.lst_msg);
    }
    
    message_queue_t current_thread_message_queue() {
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        message_queue_t id = (message_queue_t)pthread_self();
        
        if (sq_messagequeue_map.end() == sq_messagequeue_map.find(id))  id = k_invalid_queue_id;
        
        return id;
    }
    
    message_queue_t tid_to_message_queue(uint64_t tid)
    {
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        
        message_queue_t id = (message_queue_t)tid;
        
        if (sq_messagequeue_map.end() == sq_messagequeue_map.find(id)) id = k_invalid_queue_id;
        
        return id;
    }
    
    uint64_t message_queue_to_tid(message_queue_t _id) {
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        message_queue_t& id = _id;
        
        if (sq_messagequeue_map.end() == sq_messagequeue_map.find(id)) return 0;
        
        return (uint64_t)id;
    }
    
    void wait_for_running_lock_end(const message_post_t& _msg) {
        if (handler2queue(post2handler(_msg)) == current_thread_message_queue()) return;
        
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        const message_queue_t& id = handler2queue(post2handler(_msg));
        
        auto pos = sq_messagequeue_map.find(id);
        if (sq_messagequeue_map.end() == pos) return;
        message_queue_content& content = pos->second;
        
        if (content.lst_runloop_info.empty()) return;
        
        auto find_it = std::find_if(content.lst_runloop_info.begin(), content.lst_runloop_info.end(), [&_msg](const runloop_info& _v) {
            return _msg == _v.runing_message_id;
        });
        
        if (find_it == content.lst_runloop_info.end()) return;
        
        std::shared_ptr<std::condition_variable> running_cond = find_it->runing_cond;
        running_cond->wait(lock);
    }
    
    void wait_for_running_lock_end(const message_queue_t& _msg_queue) {
        if (_msg_queue == current_thread_message_queue()) return;
        
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        const message_queue_t& id = _msg_queue;
        
        auto pos = sq_messagequeue_map.find(id);
        if (sq_messagequeue_map.end() == pos) return;
        message_queue_content& content = pos->second;
        
        if (content.lst_runloop_info.empty()) return;
        
        if (kNullPost == content.lst_runloop_info.front().runing_message_id) return;
        
        std::shared_ptr<std::condition_variable> running_cond = content.lst_runloop_info.front().runing_cond;
        running_cond->wait(lock);
    }
    
    void wait_for_running_lock_end(const message_handler_t& _handler) {
        if (handler2queue(_handler) == current_thread_message_queue()) return;
        
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        const message_queue_t& id = handler2queue(_handler);
        
        auto pos = sq_messagequeue_map.find(id);
        if (sq_messagequeue_map.end() == pos) return;
        message_queue_content& content = pos->second;
        
        if (content.lst_runloop_info.empty()) return;
        
        for (auto& i: content.lst_runloop_info) {
            for (auto& x: i.runing_handler) {
                if (_handler == x) {
                    std::shared_ptr<std::condition_variable> runing_cond = i.runing_cond;
                    runing_cond->wait(lock);
                    return;
                }
            }
        }
    }
    
    void break_message_queue_runloop(const message_queue_t& _messagequeueid) {
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        const message_queue_t& id = _messagequeueid;
        
        auto pos = sq_messagequeue_map.find(id);
        if (sq_messagequeue_map.end() == pos) return;
        
        pos->second.breakflag = true;
        pos->second.breaker->notify(lock);
    }
    
    message_handler_t install_message_handler(const message_handler& _handler, bool _recvbroadcast, const message_queue_t& _messagequeue) {
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        const message_queue_t& id = _messagequeue;
        auto pos = sq_messagequeue_map.find(id);
        if (sq_messagequeue_map.end() == pos) {
            return kNullHandler;
        }
        handler_wrapper* handler = new handler_wrapper(_handler, _recvbroadcast, _messagequeue, make_seq());
        pos->second.lst_handler.emplace_back(handler);
        return handler->reg;
    }
    
    void uninstall_message_handler(const message_handler_t& _handlerid) {
        if (0 == _handlerid.queue || 0 == _handlerid.seq) return;
        
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        const message_queue_t& id = _handlerid.queue;
        
        auto pos = sq_messagequeue_map.find(id);
        if (sq_messagequeue_map.end() == pos) return;
        
        message_queue_content& content = pos->second;
        
        for (auto it = content.lst_handler.begin(); it != content.lst_handler.end(); ++it)
        {
            if (_handlerid == (*it)->reg) {
                delete *it;
                content.lst_handler.erase(it);
                break;
            }
        }
    }
    
    message_post_t post_message(const message_handler_t& _handlerid, const message_t& _message, const message_timing& _timing) {
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        const message_queue_t& id = _handlerid.queue;
        
        auto pos = sq_messagequeue_map.find(id);
        if (sq_messagequeue_map.end() == pos) {
            return kNullPost;
        }
        
        message_queue_content& content = pos->second;
        if (content.lst_msg.size() >= MAX_MQ_SIZE) {
            return kNullPost;
        }
        
        message_wrapper* messagewrapper = new message_wrapper(_handlerid, _message, _timing, make_seq());
        
        content.lst_msg.push_back(messagewrapper);
        content.breaker->notify(lock);
        return messagewrapper->postid;
    }
    
    message_post_t singleton_message(bool _replace, const message_handler_t& _handlerid, const message_t& _message, const message_timing& _timing)
    {
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        const message_queue_t& id = _handlerid.queue;
        
        auto pos = sq_messagequeue_map.find(id);
        if (sq_messagequeue_map.end() == pos) return kNullPost;
        
        message_queue_content& content= pos->second;
        
        message_post_t post_id;
        
        for (auto it = content.lst_msg.begin(); it != content.lst_msg.end(); ++it)
        {
            if ((*it)->postid.reg == _handlerid && (*it)->message == _message) {
                if (_replace) {
                    post_id = (*it)->postid;
                    delete *it;
                    content.lst_msg.erase(it);
                    break;
                } else {
                    return (*it)->postid;
                }
            }
        }
        
        if (content.lst_msg.size() >= MAX_MQ_SIZE) {
            return kNullPost;
        }
        
        message_wrapper* messagewrapper = new message_wrapper(_handlerid, _message, _timing, 0!= post_id.seq ? post_id.seq : make_seq());
        content.lst_msg.emplace_back(messagewrapper);
        content.breaker->notify(lock);
        return messagewrapper->postid;
    }
    
    message_post_t broadcast_message(const message_queue_t& _messagequeueid, const message_t& _message, const message_timing& _timing)
    {
        std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
        
        const message_queue_t& id = _messagequeueid;
        
        auto pos = sq_messagequeue_map.find(id);
        if (sq_messagequeue_map.end() == pos) {
            return kNullPost;
        }
        
        message_queue_content& content = pos->second;
        if (content.lst_msg.size() >= MAX_MQ_SIZE) {
            return kNullPost;
        }
        
        message_handler_t reg;
        reg.queue = _messagequeueid;
        reg.seq = 0;
        message_wrapper* messagewrapper =  new message_wrapper(reg, _message, _timing, make_seq());
        content.lst_msg.emplace_back(messagewrapper);
        content.breaker->notify(lock);
        return messagewrapper->postid;
    }
}
