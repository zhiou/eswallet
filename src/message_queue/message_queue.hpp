//
//  message_queue.hpp
//  transmit
//
//  Created by zhiou on 2019/8/28.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "any.hpp"
#include "spinlock.h"
#include "thread.hpp"
#include "utility.hpp"
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

namespace excelsecu {

using any = linb::any;

typedef uint64_t message_queue_t;
typedef std::function<void()> async_invoke_function;

const message_queue_t k_invalid_queue_id = 0;

struct message_handler_t {
  message_handler_t() : queue(k_invalid_queue_id), seq(0) {}
  bool operator==(const message_handler_t &_rhs) const {
    return queue == _rhs.queue && seq == _rhs.seq;
  }
  bool operator!=(const message_handler_t &_rhs) const {
    return !operator==(_rhs);
  }
  bool is_broadcast() const { return 0 == seq; }
  message_queue_t queue;
  unsigned int seq;
};

struct message_post_t {
  message_post_t() : seq(0) {}
  bool operator==(const message_post_t &_rhs) const {
    return reg == _rhs.reg && seq == _rhs.seq;
  }
  bool operator!=(const message_post_t &_rhs) const {
    return !operator==(_rhs);
  }
  message_handler_t reg;
  unsigned int seq;
};

struct message_title_t {
  message_title_t() : title(0) {}
  template <typename T>
  message_title_t(const T &_title) : title((uintptr_t)_title) {
    static_assert(sizeof(T) <= sizeof(uintptr_t), "");
  }

  bool operator==(const message_title_t &_rhs) const {
    return title == _rhs.title;
  }
  bool operator!=(const message_title_t &_rhs) const {
    return !operator==(_rhs);
  }
  uintptr_t title;
};

struct message_t {
  message_t() : title(0), anr_timeout(0), create_time(0), execute_time(0) {}
  message_t(const message_title_t &_title, const linb::any &_body1,
            const linb::any &_body2, const std::string _name)
      : title(_title), body1(_body1), body2(_body2),
        anr_timeout(10 * 60 * 1000), msg_name(_name),
        create_time(utility::get_tick_count()), execute_time(0) {}

  template <class F>
  message_t(const message_title_t &_title, const F &_func,
            const std::string &_name = "")
      : title(_title), body1(std::make_shared<async_invoke_function>()),
        body2(), anr_timeout(10 * 60 * 1000), msg_name(_name),
        create_time(utility::get_tick_count()), execute_time(0) {
    *linb::any_cast<std::shared_ptr<async_invoke_function>>(body1) = _func;
  }

  bool operator==(const message_t &_rhs) const { return title == _rhs.title; }

  std::string to_string() const {
    std::stringstream ss;
    ss << "msg_name:" << msg_name << ", create_time:" << create_time
       << ", execute_time:" << execute_time << std::endl;
    return ss.str();
  }

  message_title_t title;
  linb::any body1;
  linb::any body2;
  int64_t anr_timeout;

  std::string msg_name;
  uint64_t create_time;
  uint64_t execute_time;
};

struct message_timing {

  enum timing_t {
    kAfter,
    kPeriod,
    kImmediately,
  };

  message_timing(timing_t _timing, int64_t _after, int64_t _period)
      : type(_timing), after(_after), period(_period) {}

  message_timing(int64_t _after, int64_t _period)
      : type(timing_t::kPeriod), after(_after), period(_period) {}

  message_timing(int64_t _after)
      : type(timing_t::kAfter), after(_after), period(0) {}

  message_timing() : type(timing_t::kImmediately), after(0), period(0) {}

  timing_t type;
  int64_t after;
  int64_t period;
};

using message_timing_t = message_timing::timing_t;
const message_timing_t kAfter = message_timing::timing_t::kAfter;
const message_timing_t kPeriod = message_timing::timing_t::kPeriod;
const message_timing_t kImmediately = message_timing::timing_t::kImmediately;

typedef std::function<void(const message_post_t &_id, message_t &_message)>
    message_handler;

const message_timing kDefTiming;
const message_handler_t kNullHandler;
const message_post_t kNullPost;
const message_t kNullMessage;

inline const message_handler_t &post2handler(const message_post_t &_postid) {
  return _postid.reg;
}
inline const message_queue_t &handler2queue(const message_handler_t &_handler) {
  return _handler.queue;
}

message_queue_t current_thread_message_queue();
message_queue_t tid_tomessage_queue(thread_tid _tid);
uint64_t message_queue_to_tid(message_queue_t _id);

const message_t &running_message();

message_post_t running_message_id();
message_post_t running_message_id(const message_queue_t &_id);
message_queue_t get_def_message_queue();
message_queue_t get_def_task_queue();
message_handler_t def_async_invoke_handler(
    const message_queue_t &_msg_queue = current_thread_message_queue());

void wait_for_running_lock_end(const message_post_t &_msg);
void wait_for_running_lock_end(const message_handler_t &_handler);
void wait_for_running_lock_end(const message_queue_t &_msg_queue_id);
void break_message_queue_runloop(const message_queue_t &_msg_queue_id);

message_handler_t install_message_handler(
    const message_handler_t &_handler, bool _recv_broadcast = false,
    const message_queue_t &_message_queue_id = get_def_message_queue());
void uninstall_message_handler(const message_handler_t &_handler);

message_post_t post_message(const message_handler_t &_handler_id,
                            const message_t &_msg,
                            const message_timing &_timing = kDefTiming);
message_post_t singleton_message(bool _replace,
                                 const message_handler_t &_handlerid,
                                 const message_t &_msg,
                                 const message_timing &_timing = kDefTiming);
message_post_t broadcast_message(const message_queue_t &_msg_queue_id,
                                 const message_t &_msg,
                                 const message_timing &_timing = kDefTiming);
message_post_t faster_message(const message_handler_t &_handler_id,
                              const message_t &_msg,
                              const message_timing &_timing = kDefTiming);

bool wait_message(const message_post_t &_msg, long _timeout_in_ms = -1);
bool found_message(const message_post_t &_msg);

bool cancel_message(const message_post_t &_postid);
void cancel_message(const message_handler_t &_handler_id);
void cancel_message(const message_handler_t &_handler_id,
                    const message_title_t &_title);

std::string dump_message_queue(const message_queue_t &_msg_queue_id);

message_handler_t install_async_handler(const message_queue_t &_id);

template <class F>
message_post_t
async_invoke(const F &_func,
             const message_handler_t &_handlerid = def_async_invoke_handler(),
             const std::string &_msg_name = "default_name") {
  return post_message(_handlerid, message_t(0, _func, _msg_name));
}

template <class F>
message_post_t async_invoke_after(
    int64_t _after, const F &_func,
    const message_handler_t &_handlerid = def_async_invoke_handler(),
    const std::string &_msg_name = "default_name") {
  return post_message(_handlerid, message_t(0, _func, _msg_name),
                      message_timing(kAfter, _after, 0));
}

template <class F>
message_post_t async_invoke_period(
    int64_t _after, int64_t _period, const F &_func,
    const message_handler_t &_handlerid = def_async_invoke_handler(),
    const std::string &_msg_name = "default_name") {
  return post_message(_handlerid, message_t(0, _func, _msg_name),
                      message_timing(kPeriod, _after, _period));
}

template <class F>
message_post_t
async_invoke(const F &_func, const message_title_t &_title,
             const message_handler_t &_handlerid = def_async_invoke_handler(),
             const std::string &_msg_name = "default_name") {
  return post_message(_handlerid, message_t(_title, _func, _msg_name));
}

template <class F>
message_post_t async_invoke_after(
    int64_t _after, const F &_func, const message_title_t &_title,
    const message_handler_t &_handlerid = def_async_invoke_handler(),
    const std::string &_msg_name = "default_name") {
  return post_message(_handlerid, message_t(_title, _func, _msg_name),
                      message_timing(kAfter, _after, 0));
}

template <class F>
message_post_t async_invoke_period(
    int64_t _after, int64_t _period, const F &_func,
    const message_title_t &_title,
    const message_handler_t &_handlerid = def_async_invoke_handler(),
    const std::string &_msg_name = "default_name") {
  return post_message(_handlerid, message_t(_title, _func, _msg_name),
                      message_timing(kPeriod, _after, _period));
}

class runloop {
public:
  template <typename B>
  runloop(const B &_breaker_func) : breaker_func_(_breaker_func) {}

  template <typename B, typename D>
  runloop(const B &_breaker_func, const D &_duty_func)
      : breaker_func_(_breaker_func), duty_func_(_duty_func) {}

  runloop(){};

public:
  void Run();

private:
  runloop(const runloop &);
  runloop &operator=(const runloop &);

private:
  std::function<bool()> breaker_func_;
  std::function<void()> duty_func_;
};

class runloop_cond {
public:
  runloop_cond(){};
  virtual ~runloop_cond() {}

  static std::shared_ptr<runloop_cond> current_cond();

public:
  virtual const std::type_info &type() const = 0;
  virtual void wait(std::unique_lock<std::mutex> &_lock, long _millisecond) = 0;
  virtual void notify(std::unique_lock<std::mutex> &_lock) = 0;

private:
  runloop_cond(const runloop_cond &);
  runloop_cond &operator=(const runloop_cond &);
};

class message_queue_creater {
public:
  message_queue_creater(bool _is_create = false,
                        const char *_msg_queue_name = nullptr);
  message_queue_creater(std::shared_ptr<runloop_cond> _breaker,
                        bool _is_create = false,
                        const char *_msg_queue_name = nullptr);
  ~message_queue_creater();

  message_queue_t get_message_queue();
  message_queue_t create_message_queue();
  void cancel_and_wait();

  static message_queue_t
  create_new_message_queue(const char *_msg_queue_name = nullptr);
  static message_queue_t
  create_new_message_queue(std::shared_ptr<runloop_cond> _breaker,
                           const char *_msg_queue_name = nullptr);
  static message_queue_t
  create_new_message_queue(std::shared_ptr<runloop_cond> _breaker,
                           thread_tid _tid);
  static void release_new_message_queue(message_queue_t _message_queue_id);

private:
  message_queue_creater(const message_queue_creater &);
  message_queue_creater &operator=(const message_queue_creater &);

  void __thread_runloop();
  static void __thread_new_runloop(SpinLock *_sp);

private:
  Thread thread_;
  std::mutex message_queue_mutex_;
  message_queue_t message_queue_id_;
  std::shared_ptr<runloop_cond> breaker_;
};

template <typename R> class AsyncResult {
private:
  struct AsyncResultWrapper {
    AsyncResultWrapper()
        : result_holder(new R), result_valid(false), result(result_holder) {}

    AsyncResultWrapper(R *_result)
        : result_holder(nullptr), result_valid(false), result(_result) {}

    ~AsyncResultWrapper() {
      if (!result_valid && callback_function) {
        callback_function(*result, false);
      }

      if (result_holder) {
        delete result_holder;
      }
    }

    R *result_holder;

    std::function<R()> invoke_function;
    std::function<void(const R &, bool)> callback_function;

    bool result_valid;
    R *result;
  };

public:
  template <typename T>
  AsyncResult(const T &_func) : wrapper_(new AsyncResultWrapper()) {
    static_assert(std::is_same<typename std::result_of<T()>::type, R>::value,
                  "");
    wrapper_->invoke_function = _func;
  }

  template <typename T>
  AsyncResult(const T &_func, R *_result_holder)
      : wrapper_(new AsyncResultWrapper(_result_holder)) {
    static_assert(std::is_same<typename std::result_of<T()>::type, R>::value,
                  "");
    wrapper_->invoke_function = _func;
  }

  template <typename T, typename C>
  AsyncResult(const T &_func, const C &_callback)
      : wrapper_(new AsyncResultWrapper()) {
    static_assert(std::is_same<typename std::result_of<T()>::type, R>::value,
                  "");
    wrapper_->invoke_function = _func;
    wrapper_->callback_function = _callback;
  }

  template <typename T, typename C>
  AsyncResult(const T &_func, const C &_callback, R *_result_holder)
      : wrapper_(new AsyncResultWrapper(_result_holder)) {
    static_assert(std::is_same<typename std::result_of<T()>::type, R>::value,
                  "");
    wrapper_->invoke_function = _func;
    wrapper_->callback_function = _callback;
  }

  void operator()() const { Invoke(wrapper_->invoke_function()); }
  R &Result() const { return *(wrapper_->result); }
  operator bool() const { return wrapper_->result_valid; }

private:
  void Invoke(const R &_result) const {
    if (wrapper_->result)
      *(wrapper_->result) = _result;
    wrapper_->result_valid = true;

    if (wrapper_->callbcak_function)
      wrapper_->callback_function(_result, true);
  }

private:
  AsyncResult(const AsyncResult &);

private:
  std::shared_ptr<AsyncResultWrapper> wrapper_;
};

template <> class AsyncResult<void> {
private:
  struct AsyncResultWrapper {
    AsyncResultWrapper() : result_valid(false) {}
    ~AsyncResultWrapper() {
      if (!result_valid && callback_function) {
        callback_function(false);
      }
    }

    std::function<void()> invoke_function;
    std::function<void(bool)> callback_function;
    bool result_valid;
  };

public:
  template <typename T>
  AsyncResult(const T &_func, const void *_place_holder = nullptr)
      : wrapper_(new AsyncResultWrapper()) {
    static_assert(std::is_same<typename std::result_of<T()>::type, void>::value,
                  "");
    wrapper_->invoke_function = _func;
  }

  template <typename T, typename C>
  AsyncResult(const T &_func, const C &_callback,
              const void *_place_holder = nullptr)
      : wrapper_(new AsyncResultWrapper()) {
    static_assert(std::is_same<typename std::result_of<T()>::type, void>::value,
                  "");
    wrapper_->invoke_function = _func;
    wrapper_->callback_function = _callback;
  }

  void operator()() const {
    wrapper_->invoke_function();
    wrapper_->result_valid = true;

    if (wrapper_->callback_function) {
      wrapper_->callback_function(true);
    }
  }

  void Result() const {}
  operator bool() const { return wrapper_->result_valid; }

private:
  AsyncResult(const AsyncResult &);
  AsyncResult &operator=(const AsyncResult &);

private:
  std::shared_ptr<AsyncResultWrapper> wrapper_;
};

template <typename R> class AsyncResult<R &> {
private:
  struct AsyncResultWrapper {
    AsyncResultWrapper() : result_valid(false), result(nullptr) {}
    ~AsyncResultWrapper() {
      if (!result_valid && callback_function) {
        callback_function(*result, false);
      }
    }

    std::function<R &()> invoke_function;
    std::function<void(R &, bool)> callback_function;

    bool result_valid;
    R *result;
  };

public:
  template <typename T>
  AsyncResult(const T &_func, const void *_place_holder = NULL)
      : wrapper_(new AsyncResultWrapper()) {
    static_assert(std::is_same<typename std::result_of<T()>::type, R &>::value,
                  "");
    wrapper_->invoke_function = _func;
  }

  template <typename T, typename C>
  AsyncResult(const T &_func, const C &_callback,
              const void *_place_holder = nullptr)
      : wrapper_(new AsyncResultWrapper()) {
    static_assert(std::is_same<typename std::result_of<T()>::type, R &>::value,
                  "");
    wrapper_->invoke_function = _func;
    wrapper_->callback_function = _callback;
  }

  void operator()() const {
    wrapper_->result = &(wrapper_->invoke_function());
    wrapper_->result_valid = true;

    if (wrapper_->callback_function)
      wrapper_->callback_function(Result(), true);
  }

  R &Result() const { return *(wrapper_->result); }
  operator bool() const { return wrapper_->result_valid; }

private:
  AsyncResult &operator=(const AsyncResult &);

private:
  std::shared_ptr<AsyncResultWrapper> wrapper_;
};

template <typename R> class AsyncResult<const R &> {
private:
  struct AsyncResultWrapper {
    AsyncResultWrapper() : result_valid(false), result(nullptr) {}
    ~AsyncResultWrapper() {
      if (!result_valid && callback_function) {
        callback_function(*result, false);
      }
    }

    std::function<R &()> invoke_function;
    std::function<void(R &, bool)> callback_function;

    bool result_valid;
    R *result;
  };

public:
  template <typename T>
  AsyncResult(const T &_func, const void *_place_holder = NULL)
      : wrapper_(new AsyncResultWrapper()) {
    static_assert(
        std::is_same<typename std::result_of<T()>::type, const R &>::value, "");
    wrapper_->invoke_function = _func;
  }

  template <typename T, typename C>
  AsyncResult(const T &_func, const C &_callback,
              const void *_place_holder = nullptr)
      : wrapper_(new AsyncResultWrapper()) {
    static_assert(
        std::is_same<typename std::result_of<T()>::type, const R &>::value, "");
    wrapper_->invoke_function = _func;
    wrapper_->callback_function = _callback;
  }

  void operator()() const {
    wrapper_->result = &(wrapper_->invoke_function());
    wrapper_->result_valid = true;

    if (wrapper_->callback_function)
      wrapper_->callback_function(Result(), true);
  }

  const R &Result() const { return *(wrapper_->result); }
  operator bool() const { return wrapper_->result_valid; }

private:
  AsyncResult &operator=(const AsyncResult &);

private:
  std::shared_ptr<AsyncResultWrapper> wrapper_;
};

template <typename R>
R &wait_invoke(const AsyncResult<R> &_func,
               const message_handler_t &_handlerid = def_async_invoke_handler(),
               const std::string &_msg_name = "default_name") {
  if (current_thread_message_queue() == handler2queue(_handlerid)) {
    _func();
    return _func.Result();
  } else {
    wait_message(async_invoke(_func, _handlerid, _msg_name));
    return _func.Result();
  }
}

template <typename F>
typename std::result_of<F()>::type
wait_invoke(const F &_func,
            const message_handler_t &_handlerid = def_async_invoke_handler(),
            const std::string &_msg_name = "default_name") {
  if (current_thread_message_queue() == handler2queue(_handlerid)) {
    return _func();
  } else {
    typedef typename std::result_of<F()>::type R;
    AsyncResult<R> result(_func);
    wait_message(async_invoke(result, _handlerid, _msg_name));
    return result.Result();
  }
}

template <typename F, typename R>
R wait_invoke(const F &_func, R _ret, long _timeout = -1,
              const message_handler_t &_handlerid = def_async_invoke_handler(),
              const std::string &_msg_name = "default_name") {
  if (current_thread_message_queue() == handler2queue(_handlerid)) {
    return _func();
  } else {
    AsyncResult<R> result(_func);
    bool has_run =
        wait_message(async_invoke(result, _handlerid, _msg_name), _timeout);
    if (has_run) {
      return result.Result();
    } else {
      return _ret;
    }
  }
}

template <typename R>
message_post_t
async_invoke(const AsyncResult<R> &_func,
             const message_handler_t &_handlerid = def_async_invoke_handler(),
             const std::string &_msg_name = "default_name") {
  return post_message(_handlerid, message_t(0, _func, _msg_name));
}

class scope_register {
public:
  scope_register(const message_handler_t &_reg);
  ~scope_register();

  const message_handler_t &get() const;
  void cancel() const;
  void cancel_and_wait() const;

private:
  scope_register(const scope_register &);
  scope_register &operator=(const scope_register &);

private:
  message_handler_t *m_reg;
};
} // namespace excelsecu
