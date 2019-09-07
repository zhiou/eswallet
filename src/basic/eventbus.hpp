//
//  eventbus.hpp
//  transmit
//
//  Created by zhiou on 2019/7/8.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <chrono>
#include <iostream>
#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <typeindex>
#include <unordered_map>

#include "eslog.hpp"
#include "transmit_def.hpp"

namespace excelsecu {

// 由于eventbus全局，多设备会有问题，目前只考虑单设备，但后续会有蓝牙音频一起用的情况
struct event {
  event_msg msg_type; // restrict one handler or multiple handlers

  event() { msg_type = event_msg::begin; }

  event(const event &e) { msg_type = e.msg_type; }
};

class event_handler {
public:
  virtual void handle(event &e) = 0;
};

class eventbus {
public:
  static eventbus &instance() {
    std::call_once(eventbus::init_flag, eventbus::create_instance);
    return *eventbus::m_instance;
  }

public:
  template <class T> void add_handler(const event_handler *handler) {
    auto handler_list = this->m_handlers[typeid(T)];

    if (handler_list == nullptr) {
      handler_list = new std::list<event_handler *>();
      m_handlers[typeid(T)] = handler_list;
    }
    handler_list->push_back(const_cast<event_handler *>(handler));
  }

  template <class T> void remove_handler(const event_handler *handler) {
    auto handler_list = this->m_handlers[typeid(T)];

    if (handler_list == nullptr) {
      return;
    }
    handler_list->remove(const_cast<event_handler *>(handler));
  }

  void post(event &e) {
    assert(m_thread != nullptr);

    std::thread([&, e]() {
      LOG_DEBUG("post event type: %d", e.msg_type);
      std::unique_lock<std::mutex> lk(m_mutex);
      m_queue.emplace(e);
      m_cv.notify_one();
    })
        .detach();
  }

  void clear() {
    if (m_thread != nullptr) {
      event e;
      e.msg_type = event_msg::released;

      std::thread([&]() {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_queue.emplace(e);
        m_cv.notify_one();
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
      })
          .detach();
    }
  }

private:
  void worker_thread() {
    while (1) {
      // Wait for a message to be added to the queue
      {
        LOG_DEBUG("event bus work_thread waiting");
        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this] { return !this->m_queue.empty(); });
      }

      event e = m_queue.front();
      if (m_queue.empty()) {
        continue;
      }

      {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_queue.pop();
      }
      if (e.msg_type == event_msg::released) {
        std::unique_lock<std::mutex> lk(m_mutex);
        while (!m_queue.empty()) {
          m_queue.pop();
        }
      } else {
        auto handler_list = this->m_handlers[typeid(e)];

        if (handler_list == nullptr)
          return;

        for (auto &handler : *handler_list) {
          if (handler) {
            handler->handle(e);
          }
        }
      }
    }
  }

private:
  static void create_instance() {
    static eventbus instance;
    m_instance = &instance;
  }
  eventbus() {
    auto f = std::bind(&eventbus::worker_thread, this);
    m_thread = new std::thread(f);
  }
  ~eventbus() { clear(); }
  eventbus(const eventbus &);
  eventbus &operator=(const eventbus &);

private:
  using handler_list = std::list<event_handler *>;
  std::unordered_map<std::type_index, handler_list *> m_handlers;

  std::thread *m_thread;
  std::queue<event> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cv;
  // sigleton
  static std::once_flag init_flag;
  static eventbus *m_instance;
};

std::once_flag eventbus::init_flag;
eventbus *eventbus::m_instance = nullptr;
} // namespace excelsecu
