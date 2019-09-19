//
//  sink.hpp
//  transmit
//
//  Created by zhiou on 2019/8/19.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <string>

#include "logdef.h"

namespace excelsecu {
namespace sinks {
class sink {
public:
  virtual ~sink() = default;
  virtual void log(const std::string &msg) = 0;
  virtual void flush() = 0;

  void set_priority(Priority log_priority);

  Priority priority() const;

protected:
  // sink log level - default is all
  Priority mPriority{Priority::info};
};
} // namespace sinks
} // namespace excelsecu
