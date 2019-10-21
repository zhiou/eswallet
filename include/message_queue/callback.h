//
//  callback.h
//  transmit
//
//  Created by zhiou on 2019/8/30.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <functional>

#include "message_queue.hpp"

namespace excelsecu {

template <class T> class callback {
public:
  typedef std::function<void()> invoke_function;

  callback(const T &_func, message_handler_t _handler = kNullHandler)
      : cb_handler_(_handler), cb_func_(_func), valid_(true) {}

  void set(const T &_func, message_handler_t _handler = kNullHandler) {
    std::unique_lock<std::mutex> lock(mutex_);
    cb_handler_ = _handler;
    cb_func = _func;
    valid_ = true;
  }

  operator bool() {
    std::unique_lock<std::mutex> lock(mutex_);
    return valid_;
  }

  void invalidate() {
    std::unique_lock<std::mutex> lock(mutex_);
    valid_ = false;
  }

  template <typename... Args> void operator()(const Args &... rest) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (!valid_) {
      return;
    }

    std::function<void()> func = std::bind(cb_func_, rest...);
    if (kNullHandler == cb_handler_) {
      func();
      return;
    }
    async_invoke(func, cb_handler_);
  }

private:
  message_handler_t cb_handler_;
  T cb_func_;
  std::mutex mutex_;
  bool valid_;
}
} // namespace excelsecu
