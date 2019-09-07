//
//  eslog.hpp
//  transmit
//
//  Created by zhiou on 2019/8/19.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "basic_file_sink.hpp"
#include "logdef.h"
#include "logger.hpp"
#include "registry.hpp"

namespace excelsecu {
struct synchronous_factory {
  template <typename Sink, typename... SinkArgs>
  static std::shared_ptr<Logger> create(std::string logger_name,
                                        SinkArgs &&... args) {
    auto sink = std::make_shared<Sink>(std::forward<SinkArgs>(args)...);
    auto new_logger =
        std::make_shared<Logger>(std::move(logger_name), std::move(sink));
    registry::instance().initialize_logger(new_logger);
    return new_logger;
  }
};
using default_factory = synchronous_factory;

template <typename Factory = default_factory>
inline std::shared_ptr<Logger> basic_file_logger(const std::string &logger_name,
                                                 const std::string &filename,
                                                 bool truncate = false) {
  return Factory::template create<sinks::basic_file_sink>(logger_name,
                                                          filename);
}

inline void set_default_logger(std::shared_ptr<Logger> default_logger) {
  registry::instance().set_default_logger(std::move(default_logger));
}

inline void enable_file_logger(const std::string &file_name,
                               bool truncate = false) {
  auto file_logger =
      basic_file_logger("basic_file_logger", file_name, truncate);
  set_default_logger(file_logger);
}

inline std::shared_ptr<Logger> default_logger() {
  return registry::instance().default_logger();
}

template <typename... Args>
inline void log(Priority lvl, const char *fmt, const Args &... args) {
  default_logger()->log(lvl, std::string(fmt), args...);
}

template <typename... Args>
inline void debug(const char *fmt, const Args &... args) {
  default_logger()->debug(fmt, args...);
}

template <typename... Args>
inline void info(const char *fmt, const Args &... args) {
  default_logger()->info(fmt, args...);
}

template <typename... Args>
inline void warn(const char *fmt, const Args &... args) {
  default_logger()->warn(fmt, args...);
}

template <typename... Args>
inline void error(const char *fmt, const Args &... args) {
  default_logger()->error(fmt, args...);
}

template <typename T> inline void log(Priority lvl, const T &msg) {
  default_logger()->log(lvl, msg);
}

template <typename T> inline void debug(const T &msg) {
  default_logger()->debug(msg);
}

template <typename T> inline void info(const T &msg) {
  default_logger()->info(msg);
}

template <typename T> inline void warn(const T &msg) {
  default_logger()->warn(msg);
}

template <typename T> inline void error(const T &msg) {
  default_logger()->error(msg);
}

#define ES_LOGGER_CALL(logger, level, format, ...)                             \
  do {                                                                         \
    logger->log(level, format, ##__VA_ARGS__);                                 \
  } while (0)

#if !defined(LOG_ACTIVE_LEVEL)
#define LOG_ACTIVE_LEVEL LOG_LEVEL_INFO
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_DEBUG
#define ES_LOGGER_DEBUG(logger, format, ...)                                   \
  ES_LOGGER_CALL(logger, Priority::debug, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...)                                                 \
  ES_LOGGER_DEBUG(excelsecu::default_logger(),                                 \
                  std::string("[%s %d] ") + format, __FUNCTION__, __LINE__,    \
                  ##__VA_ARGS__)
#else
#define ES_LOGGER_DEBUG(logger, ...) (void)0
#define LOG_DEBUG(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_INFO
#define ES_LOGGER_INFO(logger, format, ...)                                    \
  ES_LOGGER_CALL(logger, Priority::info, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)                                                  \
  ES_LOGGER_INFO(excelsecu::default_logger(), format, ##__VA_ARGS__)
#else
#define ES_LOGGER_INFO(logger, ...) (void)0
#define LOG_INFO(, ...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_WARN
#define ES_LOGGER_WARN(logger, format, ...)                                    \
  ES_LOGGER_CALL(logger, Priority::warn, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)                                                  \
  ES_LOGGER_WARN(excelsecu::default_logger(), format, ##__VA_ARGS__)
#else
#define ES_LOGGER_WARN(logger, ...) (void)0
#define LOG_WARN(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_ERROR
#define ES_LOGGER_ERROR(logger, format, ...)                                   \
  ES_LOGGER_CALL(logger, Priority::error, format, __FUNCTION__, __LINE__,      \
                 ##__VA_ARGS__)
#define LOG_ERROR(format, ...)                                                 \
  ES_LOGGER_ERROR(excelsecu::default_logger(),                                 \
                  std::string("[%s %d] ") + format, ##__VA_ARGS__)
#else
#define ES_LOGGER_ERROR(logger, ...) (void)0
#define LOG_ERROR(...) (void)0
#endif

} // namespace excelsecu
