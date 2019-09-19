//
//  message_queue.cpp
//  transmit
//
//  Created by zhiou on 2019/8/28.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#include "message_queue.hpp"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <list>
#include <map>

namespace excelsecu {

#define MAX_MQ_SIZE 5000

static unsigned int make_seq() {
  static unsigned int s_seq = 0;
  return ++s_seq;
}

static message_queue_t tid2mqid(thread_tid _tid) { return *(long *)&(*_tid); }

struct message_wrapper {
  message_wrapper(const message_handler_t &_handlerid,
                  const message_t &_message, const message_timing &_timing,
                  unsigned int _seq)
      : message(_message), timing(_timing) {
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
  handler_wrapper(const message_handler &_handler, bool _recvbroadcast,
                  const message_queue_t &_messagequeueid, unsigned int _seq)
      : handler(_handler), recvbroadcast(_recvbroadcast) {
    reg.seq = _seq;
    reg.queue = _messagequeueid;
  }

  message_handler_t reg;
  message_handler handler;
  bool recvbroadcast;
};

struct runloop_info {
  runloop_info() : runing_message(nullptr) {
    runing_cond = std::make_shared<std::condition_variable>();
  }

  std::shared_ptr<std::condition_variable> runing_cond;
  message_post_t runing_message_id;
  message_t *runing_message;
  std::list<message_handler_t> runing_handler;
};

class Cond : public runloop_cond {
public:
  Cond(){};

public:
  virtual const std::type_info &type() const { return typeid(Cond); }

  virtual void wait(std::unique_lock<std::mutex> &_lock, long _millisecond) {
    auto to = std::chrono::milliseconds(_millisecond);
    cond_.wait_for(_lock, to);
  }

  virtual void notify(std::unique_lock<std::mutex> &_lock) {
    cond_.notify_all();
  }

private:
  Cond(const Cond &);
  Cond &operator=(const Cond &);

private:
  std::condition_variable cond_;
};

struct message_queue_content {
  message_queue_content() : breakflag(false) {}

  message_handler_t invoke_reg;
  bool breakflag;
  std::shared_ptr<runloop_cond> breaker;
  std::list<message_wrapper *> lst_msg;
  std::list<handler_wrapper *> lst_handler;
  std::list<runloop_info> lst_runloop_info;

private:
  message_queue_content(const message_queue_content &);
  message_queue_content &operator=(const message_queue_content &);
};

static std::mutex &messagequeue_map_mutex() {
  static std::mutex *mutex = new std::mutex;
  return *mutex;
}

#define sg_messagequeue_map_mutex messagequeue_map_mutex()

static std::map<message_queue_t, message_queue_content> &messagequeue_map() {
  static auto *mq_map = new std::map<message_queue_t, message_queue_content>;
  return *mq_map;
}
#define sq_messagequeue_map messagequeue_map()

static std::string
dump_message(const std::list<message_wrapper *> &_message_list) {
  return "";
}

std::string dump_mq(const message_queue_t &_msg_queue_id) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _msg_queue_id;

  auto pos = sq_messagequeue_map.find(id);
  if (pos == std::end(sq_messagequeue_map)) {
    return "";
  }
  message_queue_content &content = pos->second;
  return dump_message(content.lst_msg);
}

message_queue_t current_thread_message_queue() {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  message_queue_t id = tid2mqid(pthread_self());

  if (sq_messagequeue_map.end() == sq_messagequeue_map.find(id)) {
    id = k_invalid_queue_id;
  }

  return id;
}

message_queue_t tid_to_message_queue(thread_tid tid) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);

  message_queue_t id = tid2mqid(tid);

  if (sq_messagequeue_map.end() == sq_messagequeue_map.find(id))
    id = k_invalid_queue_id;

  return id;
}

uint64_t message_queue_to_tid(message_queue_t _id) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  message_queue_t &id = _id;

  if (sq_messagequeue_map.end() == sq_messagequeue_map.find(id))
    return 0;

  return (uint64_t)id;
}

void wait_for_running_lock_end(const message_post_t &_msg) {
  if (handler2queue(post2handler(_msg)) == current_thread_message_queue())
    return;

  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = handler2queue(post2handler(_msg));

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return;
  message_queue_content &content = pos->second;

  if (content.lst_runloop_info.empty())
    return;

  auto find_it = std::find_if(
      content.lst_runloop_info.begin(), content.lst_runloop_info.end(),
      [&_msg](const runloop_info &_v) { return _msg == _v.runing_message_id; });

  if (find_it == content.lst_runloop_info.end())
    return;

  std::shared_ptr<std::condition_variable> running_cond = find_it->runing_cond;
  running_cond->wait(lock);
}

void wait_for_running_lock_end(const message_queue_t &_msg_queue) {
  if (_msg_queue == current_thread_message_queue())
    return;

  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _msg_queue;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return;
  message_queue_content &content = pos->second;

  if (content.lst_runloop_info.empty())
    return;

  if (kNullPost == content.lst_runloop_info.front().runing_message_id)
    return;

  std::shared_ptr<std::condition_variable> running_cond =
      content.lst_runloop_info.front().runing_cond;
  running_cond->wait(lock);
}

void wait_for_running_lock_end(const message_handler_t &_handler) {
  if (handler2queue(_handler) == current_thread_message_queue())
    return;

  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = handler2queue(_handler);

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return;
  message_queue_content &content = pos->second;

  if (content.lst_runloop_info.empty())
    return;

  for (auto &i : content.lst_runloop_info) {
    for (auto &x : i.runing_handler) {
      if (_handler == x) {
        std::shared_ptr<std::condition_variable> runing_cond = i.runing_cond;
        runing_cond->wait(lock);
        return;
      }
    }
  }
}

void break_message_queue_runloop(const message_queue_t &_messagequeueid) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _messagequeueid;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return;

  pos->second.breakflag = true;
  pos->second.breaker->notify(lock);
}

message_handler_t
install_message_handler(const message_handler &_handler, bool _recvbroadcast,
                        const message_queue_t &_messagequeue) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _messagequeue;
  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos) {
    return kNullHandler;
  }
  handler_wrapper *handler =
      new handler_wrapper(_handler, _recvbroadcast, _messagequeue, make_seq());
  pos->second.lst_handler.emplace_back(handler);
  return handler->reg;
}

void uninstall_message_handler(const message_handler_t &_handlerid) {
  if (0 == _handlerid.queue || 0 == _handlerid.seq)
    return;

  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _handlerid.queue;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return;

  message_queue_content &content = pos->second;

  for (auto it = content.lst_handler.begin(); it != content.lst_handler.end();
       ++it) {
    if (_handlerid == (*it)->reg) {
      delete *it;
      content.lst_handler.erase(it);
      break;
    }
  }
}

message_post_t post_message(const message_handler_t &_handlerid,
                            const message_t &_message,
                            const message_timing &_timing) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _handlerid.queue;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos) {
    return kNullPost;
  }

  message_queue_content &content = pos->second;
  if (content.lst_msg.size() >= MAX_MQ_SIZE) {
    return kNullPost;
  }

  message_wrapper *messagewrapper =
      new message_wrapper(_handlerid, _message, _timing, make_seq());

  content.lst_msg.push_back(messagewrapper);
  content.breaker->notify(lock);
  return messagewrapper->postid;
}

message_post_t singleton_message(bool _replace,
                                 const message_handler_t &_handlerid,
                                 const message_t &_message,
                                 const message_timing &_timing) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _handlerid.queue;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return kNullPost;

  message_queue_content &content = pos->second;

  message_post_t post_id;

  for (auto it = content.lst_msg.begin(); it != content.lst_msg.end(); ++it) {
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

  message_wrapper *messagewrapper =
      new message_wrapper(_handlerid, _message, _timing,
                          0 != post_id.seq ? post_id.seq : make_seq());
  content.lst_msg.emplace_back(messagewrapper);
  content.breaker->notify(lock);
  return messagewrapper->postid;
}

message_post_t broadcast_message(const message_queue_t &_messagequeueid,
                                 const message_t &_message,
                                 const message_timing &_timing) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);

  const message_queue_t &id = _messagequeueid;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos) {
    return kNullPost;
  }

  message_queue_content &content = pos->second;
  if (content.lst_msg.size() >= MAX_MQ_SIZE) {
    return kNullPost;
  }

  message_handler_t reg;
  reg.queue = _messagequeueid;
  reg.seq = 0;
  message_wrapper *messagewrapper =
      new message_wrapper(reg, _message, _timing, make_seq());
  content.lst_msg.emplace_back(messagewrapper);
  content.breaker->notify(lock);
  return messagewrapper->postid;
}

static int64_t compute_wait_time(const message_wrapper &_wrap) {
  int64_t wait_time = 0;

  if (kImmediately == _wrap.timing.type) {
    wait_time = 0;
  } else if (kAfter == _wrap.timing.type) {
    int64_t time_cost = utility::get_tick_span(_wrap.record_time);
    wait_time = _wrap.timing.after - time_cost;
  } else if (kPeriod == _wrap.timing.type) {
    int64_t time_cost = utility::get_tick_span(_wrap.record_time);

    if (kAfter == _wrap.period_status) {
      wait_time = _wrap.timing.after - time_cost;
    } else if (kPeriod == _wrap.period_status) {
      wait_time = _wrap.timing.period - time_cost;
    }
  }
  return 0 < wait_time ? wait_time : 0;
}

message_post_t faster_message(const message_handler_t &_handlerid,
                              const message_t &_message,
                              const message_timing &_timing) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _handlerid.queue;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return kNullPost;

  message_queue_content &content = pos->second;

  message_wrapper *messagewrapper =
      new message_wrapper(_handlerid, _message, _timing, make_seq());

  for (auto it = content.lst_msg.begin(); it != content.lst_msg.end(); ++it) {
    if ((*it)->postid.reg == _handlerid && (*it)->message == _message) {
      if (compute_wait_time(**it) < compute_wait_time(*messagewrapper)) {
        delete messagewrapper;
        return (*it)->postid;
      }
      messagewrapper->postid = (*it)->postid;
      delete (*it);
      content.lst_msg.erase(it);
      break;
    }
  }

  if (content.lst_msg.size() >= MAX_MQ_SIZE) {
    delete messagewrapper;
    return kNullPost;
  }

  content.lst_msg.push_back(messagewrapper);
  content.breaker->notify(lock);
  return messagewrapper->postid;
}

bool wait_message(const message_post_t &_message, long _timeoutInMs) {
  bool is_in_mq =
      handler2queue(post2handler(_message)) == current_thread_message_queue();

  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = handler2queue(post2handler(_message));
  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return false;
  message_queue_content &content = pos->second;

  auto find_it = std::find_if(content.lst_msg.begin(), content.lst_msg.end(),
                              [&_message](const message_wrapper *const &_v) {
                                return _message == _v->postid;
                              });

  if (find_it == content.lst_msg.end()) {
    auto find_it = std::find_if(content.lst_runloop_info.begin(),
                                content.lst_runloop_info.end(),
                                [&_message](const runloop_info &_v) {
                                  return _message == _v.runing_message_id;
                                });

    if (find_it != content.lst_runloop_info.end()) {
      if (is_in_mq)
        return false;

      std::shared_ptr<std::condition_variable> running_cond =
          find_it->runing_cond;
      if (_timeoutInMs < 0) {
        running_cond->wait(lock);
      } else {
        auto ret = running_cond->wait_for(
            lock, std::chrono::milliseconds(_timeoutInMs));
        return ret == std::cv_status::no_timeout;
      }
    }
  } else {
    if (is_in_mq) {
      lock.unlock();
      runloop([&_message]() {
        message_queue_content &content =
            sq_messagequeue_map[handler2queue(post2handler(_message))];
        return content.lst_msg.end() ==
               std::find_if(content.lst_msg.begin(), content.lst_msg.end(),
                            [&_message](const message_wrapper *const &_v) {
                              return _message == _v->postid;
                            });
      })
          .Run();
    } else {
      if (!((*find_it)->wait_end_cond))
        (*find_it)->wait_end_cond = std::make_shared<std::condition_variable>();

      std::shared_ptr<std::condition_variable> wait_end_cond =
          (*find_it)->wait_end_cond;
      if (_timeoutInMs < 0) {
        wait_end_cond->wait(lock);
      } else {
        auto ret = wait_end_cond->wait_for(
            lock, std::chrono::milliseconds(_timeoutInMs));
        return ret == std::cv_status::no_timeout;
      }
    }
  }
  return true;
}

bool found_message(const message_post_t &_message) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = handler2queue(post2handler(_message));

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return false;

  message_queue_content &content = pos->second;
  if (content.lst_runloop_info.empty())
    return false;

  auto find_it = std::find_if(content.lst_runloop_info.begin(),
                              content.lst_runloop_info.end(),
                              [&_message](const runloop_info &_v) {
                                return _message == _v.runing_message_id;
                              });

  if (find_it != content.lst_runloop_info.end()) {
    return true;
  }
  for (auto it = content.lst_msg.begin(); it != content.lst_msg.end(); ++it) {
    if (_message == (*it)->postid) {
      return true;
    }
  }

  return false;
}

bool cancel_message(const message_post_t &_postid) {
  if (0 == _postid.reg.queue || 0 == _postid.seq)
    return false;

  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _postid.reg.queue;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos) {
    return false;
  }

  message_queue_content &content = pos->second;

  for (auto it = content.lst_msg.begin(); it != content.lst_msg.end(); ++it) {
    if (_postid == (*it)->postid) {
      delete (*it);
      content.lst_msg.erase(it);
      return true;
    }
  }
  return false;
}

void cancel_message(const message_handler_t &_handlerid) {

  // 0==_handlerid.seq for BroadcastMessage
  if (0 == _handlerid.queue)
    return;

  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _handlerid.queue;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos) {
    //        assert2(false, "%lu", id);
    return;
  }

  message_queue_content &content = pos->second;

  for (auto it = content.lst_msg.begin(); it != content.lst_msg.end();) {
    if (_handlerid == (*it)->postid.reg) {
      delete (*it);
      it = content.lst_msg.erase(it);
    } else {
      ++it;
    }
  }
}

void cancel_message(const message_handler_t &_handlerid,
                    const message_title_t &_title) {

  // 0==_handlerid.seq for BroadcastMessage
  if (0 == _handlerid.queue)
    return;

  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _handlerid.queue;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos) {
    return;
  }

  message_queue_content &content = pos->second;

  for (auto it = content.lst_msg.begin(); it != content.lst_msg.end();) {
    if (_handlerid == (*it)->postid.reg && _title == (*it)->message.title) {
      delete (*it);
      it = content.lst_msg.erase(it);
    } else {
      ++it;
    }
  }
}

const message_t &running_message() {
  message_queue_t id = tid2mqid(pthread_self());
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos) {
    return kNullMessage;
  }

  message_t *running_msg = pos->second.lst_runloop_info.back().runing_message;
  return running_msg ? *running_msg : kNullMessage;
}

message_post_t running_message_id() {
  message_queue_t id = tid2mqid(pthread_self());
  return running_message_id(id);
}

message_post_t running_message_id(const message_queue_t &_id) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);

  auto pos = sq_messagequeue_map.find(_id);
  if (sq_messagequeue_map.end() == pos) {
    return kNullPost;
  }

  message_queue_content &content = pos->second;
  return content.lst_runloop_info.back().runing_message_id;
}

static void async_invoke_handler(const message_post_t &_id,
                                 message_t &_message) {
  (*linb::any_cast<std::shared_ptr<async_invoke_function>>(_message.body1))();
}

message_handler_t install_async_handler(const message_queue_t &id) {
  return install_message_handler(async_invoke_handler, false, id);
}

static message_queue_t
create_message_queue_info(std::shared_ptr<runloop_cond> &_breaker,
                          thread_tid _tid) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);

  message_queue_t id = tid2mqid(_tid);

  if (sq_messagequeue_map.end() == sq_messagequeue_map.find(id)) {
    message_queue_content &content = sq_messagequeue_map[id];
    handler_wrapper *handler =
        new handler_wrapper(&async_invoke_handler, false, id, make_seq());
    content.lst_handler.emplace_back(handler);
    content.invoke_reg = handler->reg;
    if (_breaker) {
      content.breaker = _breaker;
    } else {
      content.breaker = std::make_shared<Cond>();
    }
  }
  return id;
}

static void release_message_queue_info() {
  message_queue_t id = tid2mqid(pthread_self());

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() != pos) {
    message_queue_content &content = pos->second;

    for (auto it = std::begin(content.lst_msg); it != std::end(content.lst_msg);
         ++it) {
      delete (*it);
    }

    for (auto it = std::begin(content.lst_handler);
         it != std::end(content.lst_handler); ++it) {
      delete (*it);
    }

    sq_messagequeue_map.erase(id);
  }
}

void runloop::Run() {
  message_queue_t id = tid2mqid(pthread_self());
  ;
  if (id != 0) {
    std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
    sq_messagequeue_map[id].lst_runloop_info.emplace_back(runloop_info());
  }

  while (true) {
    std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
    message_queue_content &content = sq_messagequeue_map[id];
    content.lst_runloop_info.back().runing_message_id = kNullPost;
    content.lst_runloop_info.back().runing_message = nullptr;
    content.lst_runloop_info.back().runing_handler.clear();
    content.lst_runloop_info.back().runing_cond->notify_all();

    if (duty_func_)
      duty_func_();

    if ((content.breakflag || (breaker_func_ && breaker_func_()))) {
      content.lst_runloop_info.pop_back();
      if (content.lst_runloop_info.empty()) {
        release_message_queue_info();
      }
      break;
    }

    int64_t wait_time = 10 * 60 * 1000;
    message_wrapper *messagewrapper = nullptr;
    bool delmessage = true;

    for (auto it = content.lst_msg.begin(); it != content.lst_msg.end(); ++it) {
      if (kImmediately == (*it)->timing.type) {
        messagewrapper = *it;
        content.lst_msg.erase(it);
        break;
      } else if (kAfter == (*it)->timing.type) {
        int64_t time_cost = utility::get_tick_span((*it)->record_time);

        if ((*it)->timing.after <= time_cost) {
          messagewrapper = *it;
          content.lst_msg.erase(it);
          break;
        } else {
          wait_time = std::min(wait_time, (*it)->timing.after - time_cost);
        }
      } else if (kPeriod == (*it)->timing.type) {
        if (kAfter == (*it)->period_status) {
          int64_t time_cost = utility::get_tick_span((*it)->record_time);

          if ((*it)->timing.after <= time_cost) {
            messagewrapper = *it;
            (*it)->record_time = utility::get_tick_count();
            (*it)->period_status = kPeriod;
            delmessage = false;
            break;
          } else {
            wait_time = std::min(wait_time, (*it)->timing.after - time_cost);
          }
        } else if (kPeriod == (*it)->period_status) {
          int64_t time_cost = utility::get_tick_span((*it)->record_time);

          if ((*it)->timing.period <= time_cost) {
            messagewrapper = *it;
            (*it)->record_time = utility::get_tick_count();
            delmessage = false;
            break;
          } else {
            wait_time = std::min(wait_time, (*it)->timing.period - time_cost);
          }
        } else {
          assert(false);
        }
      } else {
        assert(false);
      }
    }

    if (nullptr == messagewrapper) {
      content.breaker->wait(lock, std::chrono::milliseconds(wait_time).count());
      continue;
    }

    std::list<handler_wrapper> fit_handler;
    for (auto it = content.lst_handler.begin(); it != content.lst_handler.end();
         ++it) {
      if (messagewrapper->postid.reg == (*it)->reg ||
          ((*it)->recvbroadcast && messagewrapper->postid.reg.is_broadcast())) {
        fit_handler.emplace_back(**it);
        content.lst_runloop_info.back().runing_handler.emplace_back((*it)->reg);
      }
    }

    content.lst_runloop_info.back().runing_message_id = messagewrapper->postid;
    content.lst_runloop_info.back().runing_message = &messagewrapper->message;
    int64_t anr_timeout = messagewrapper->message.anr_timeout;
    lock.unlock();

    messagewrapper->message.execute_time = utility::get_tick_count();
    for (auto it = fit_handler.begin(); it != fit_handler.end(); ++it) {
      (*it).handler(messagewrapper->postid, messagewrapper->message);
    }

    if (delmessage) {
      delete messagewrapper;
    }
  }
}

std::shared_ptr<runloop_cond> runloop_cond::current_cond() {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  message_queue_t id = tid2mqid(pthread_self());

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() != pos) {
    message_queue_content &content = pos->second;
    return content.breaker;
  } else {
    return std::shared_ptr<runloop_cond>();
  }
}

message_queue_creater::message_queue_creater(bool _iscreate,
                                             const char *_msg_queue_name)
    : message_queue_creater(std::shared_ptr<runloop_cond>(), _iscreate,
                            _msg_queue_name) {}

message_queue_creater::message_queue_creater(
    std::shared_ptr<runloop_cond> _breaker, bool _iscreate,
    const char *_msg_queue_name)
    : thread_(std::bind(&::excelsecu::message_queue_creater::__thread_runloop,
                        this),
              _msg_queue_name),
      message_queue_id_(k_invalid_queue_id), breaker_(_breaker) {
  if (_iscreate)
    create_message_queue();
}

message_queue_creater::~message_queue_creater() { cancel_and_wait(); }

void message_queue_creater::__thread_runloop() {
  std::unique_lock<std::mutex> lock(message_queue_mutex_);
  lock.unlock();

  runloop().Run();
}

message_queue_t message_queue_creater::get_message_queue() {
  return message_queue_id_;
}

message_queue_t message_queue_creater::create_message_queue() {
  std::unique_lock<std::mutex> lock(message_queue_mutex_);

  if (thread_.is_runing())
    return message_queue_id_;
  //
  if (0 != thread_.start()) {
    return k_invalid_queue_id;
  }
  message_queue_id_ = create_message_queue_info(breaker_, thread_.tid());
  //
  return message_queue_id_;
}

void message_queue_creater::cancel_and_wait() {
  std::unique_lock<std::mutex> lock(message_queue_mutex_);

  if (k_invalid_queue_id == message_queue_id_)
    return;

  break_message_queue_runloop(message_queue_id_);
  message_queue_id_ = k_invalid_queue_id;
  lock.unlock();
  if (ThreadUtil::current_thread_id() != thread_.tid()) {
    thread_.join();
  }
}

message_queue_t message_queue_creater::create_new_message_queue(
    std::shared_ptr<runloop_cond> _breaker, thread_tid _tid) {
  return (create_message_queue_info(_breaker, _tid));
}

message_queue_t message_queue_creater::create_new_message_queue(
    std::shared_ptr<runloop_cond> _breaker, const char *_messagequeue_name) {

  SpinLock *sp = new SpinLock;
  Thread thread(std::bind(&__thread_new_runloop, sp), _messagequeue_name, true);
  //    thread.outside_join();

  sp->lock();
  if (0 != thread.start()) {
    (*sp).unlock();
    delete sp;
    return k_invalid_queue_id;
  }

  message_queue_t id = create_message_queue_info(_breaker, thread.tid());
  return id;
}

message_queue_t message_queue_creater::create_new_message_queue(
    const char *_messagequeue_name) {
  return create_new_message_queue(std::shared_ptr<runloop_cond>(),
                                  _messagequeue_name);
}

void message_queue_creater::release_new_message_queue(
    message_queue_t _messagequeue_id) {

  if (k_invalid_queue_id == _messagequeue_id)
    return;

  break_message_queue_runloop(_messagequeue_id);
  wait_for_running_lock_end(_messagequeue_id);
  ThreadUtil::join((thread_tid)_messagequeue_id);
}

void message_queue_creater::__thread_new_runloop(SpinLock *_sp) {
  (*_sp).unlock();
  delete _sp;

  runloop().Run();
}

message_queue_t get_def_message_queue() {
  static message_queue_creater *s_defmessagequeue = new message_queue_creater;
  return s_defmessagequeue->create_message_queue();
}

message_queue_t get_def_task_queue() {
  static message_queue_creater *s_deftaskqueue = new message_queue_creater;
  return s_deftaskqueue->create_message_queue();
}

message_handler_t
def_async_invoke_handler(const message_queue_t &_messagequeue) {
  std::unique_lock<std::mutex> lock(sg_messagequeue_map_mutex);
  const message_queue_t &id = _messagequeue;

  auto pos = sq_messagequeue_map.find(id);
  if (sq_messagequeue_map.end() == pos)
    return kNullHandler;

  message_queue_content &content = pos->second;
  return content.invoke_reg;
}

scope_register::scope_register(const message_handler_t &_reg)
    : m_reg(new message_handler_t(_reg)) {}

scope_register::~scope_register() {
  cancel();
  delete m_reg;
}

const message_handler_t &scope_register::get() const { return *m_reg; }

void scope_register::cancel() const {
  uninstall_message_handler(*m_reg);
  cancel_message(*m_reg);
}
void scope_register::cancel_and_wait() const {
  cancel();
  wait_for_running_lock_end(*m_reg);
}
} // namespace excelsecu
