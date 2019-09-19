//
//  basic_file_sink.hpp
//  transmit
//
//  Created by zhiou on 2019/8/19.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

//#include "eslog.hpp"
#include "basic_sink.hpp"
#include "logfile.hpp"

namespace excelsecu {
namespace sinks {
class basic_file_sink final : public base_sink {
public:
  explicit basic_file_sink(const std::string &filename) {
    m_logfile.open(filename);
  }

  const std::string &filename() const { return m_logfile.filename(); }

protected:
  void sink_it_(const std::string &msg) override { m_logfile.write(msg); }

  void flush_() override { m_logfile.flush(); }

private:
  logfile m_logfile;
};

} // namespace sinks
} // namespace excelsecu
