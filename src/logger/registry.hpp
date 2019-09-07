//
//  registry.hpp
//  transmit
//
//  Created by zhiou on 2019/8/19.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "logdef.h"
#include "logger.hpp"

#include <memory>
#include <mutex>
#include <unordered_map>

namespace excelsecu {
class thread_pool;

class registry {
public:
  registry(const registry &) = delete;
  registry &operator=(const registry &) = delete;

  void rigister_logger(std::shared_ptr<Logger> new_logger) {
    std::lock_guard<std::mutex> lock(logger_map_mutex_);
    register_logger_(std::move(new_logger));
  }

  void initialize_logger(std::shared_ptr<Logger> new_logger) {
    std::lock_guard<std::mutex> lock(logger_map_mutex_);

    new_logger->set_level(level_);

    if (automatic_registration_) {
      register_logger_(std::move(new_logger));
    }
  }

  std::shared_ptr<Logger> get(const std::string &logger_name) {
    std::lock_guard<std::mutex> lock(logger_map_mutex_);
    auto found = loggers_.find(logger_name);
    return found == loggers_.end() ? nullptr : found->second;
  }

  std::shared_ptr<Logger> default_logger() {
    std::lock_guard<std::mutex> lock(logger_map_mutex_);
    return default_logger_;
  }

  void set_default_logger(std::shared_ptr<Logger> new_default_logger) {
    std::lock_guard<std::mutex> lock(logger_map_mutex_);
    // remove previous default logger from the map
    if (default_logger_ != nullptr) {
      loggers_.erase(default_logger_->name());
    }
    if (new_default_logger != nullptr) {
      loggers_[new_default_logger->name()] = new_default_logger;
    }
    default_logger_ = std::move(new_default_logger);
  }

  static registry &instance() {
    static registry s_instance;
    return s_instance;
  }

private:
  registry() {
    const char *default_logger_name = "";
    default_logger_ = std::make_shared<Logger>(default_logger_name, nullptr);
    loggers_[default_logger_name] = default_logger_;
  }

  void register_logger_(std::shared_ptr<Logger> new_logger) {
    auto logger_name = new_logger->name();
    if (loggers_.find(logger_name) != loggers_.end()) {
      return;
    }
    loggers_[logger_name] = std::move(new_logger);
  }

private:
  std::mutex logger_map_mutex_, flusher_mutex_;
  std::recursive_mutex tp_mutex_;
  std::unordered_map<std::string, std::shared_ptr<Logger>> loggers_;

  Priority level_ = Priority::info;

  std::shared_ptr<thread_pool> tp_;
  std::shared_ptr<Logger> default_logger_;
  bool automatic_registration_ = true;
};
} // namespace excelsecu
