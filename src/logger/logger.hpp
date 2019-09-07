//
//  logger.hpp
//  transmit
//
//  Created by zhiou on 2019/8/19.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>

#include "logdef.h"
#include "sink.hpp"

namespace excelsecu {
using sink_shared_ptr = std::shared_ptr<sinks::sink>;
class Logger {
public:
  std::string name() const { return m_name; }

public:
  Logger(const std::string &name, sink_shared_ptr single_sink)
      : m_name(name), m_sink_ptr(std::move(single_sink)) {}

  ~Logger() = default;

  template <typename... Args>
  void info(std::string format, const Args &... args) {
    log(Priority::info, format, args...);
  }

  template <typename... Args>
  void warn(std::string format, const Args &... args) {
    log(Priority::warn, format, args...);
  }

  template <typename... Args>
  void debug(std::string format, const Args &... args) {
    log(Priority::debug, format, args...);
  }

  template <typename... Args>
  void error(std::string format, const Args &... args) {
    log(Priority::error, format, args...);
  }

  template <typename... Args>
  void log(Priority priority, std::string format, const Args &... args) {
    if (!should_log(priority)) {
      return;
    }

    std::string log_msg;

    char buf[0x100];
    int len = snprintf(buf, sizeof(buf), (const char *)format.c_str(), args...);
    if (len < sizeof(buf)) {
      log_msg = std::string(buf);
    }
    if (len > sizeof(buf)) {
      auto buffer = std::unique_ptr<char>(new char[len + 1]);
      snprintf(buffer.get(), len + 1, (const char *)format.c_str(), args...);
      log_msg = std::string(buffer.get());
    }

    sink(log_msg);
  }

public:
  void set_level(Priority priority) { m_prioryty = priority; }

  static Priority default_level() { return Priority::info; }

  Priority level() const { return m_prioryty; }

private:
  bool should_log(Priority priority) { return true; }

  void sink(const std::string &log_msg) {

    if (!should_log(m_prioryty)) {
      return;
    }

    if (!m_sink_ptr.get()) {
      std::cout << log_msg << std::endl;
    } else {
      m_sink_ptr->log(log_msg);
      m_sink_ptr->flush();
    }
  }

private:
  std::string m_name;
  Priority m_prioryty{default_level()};
  sink_shared_ptr m_sink_ptr;
};
} // namespace excelsecu
