//
//  bit_pony.hpp
//  transmit
//
//  Created by zhiou on 2019/8/22.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <string>
#include <unordered_map>

namespace excelsecu {
namespace wallet {
namespace bit_pony {

enum class format {
  binary,
  json,
  validate,
};

enum class type {
  tx_in,
  tx_out,
  block_header,
  tx,
  block,
};

using data = std::unordered_map<std::string, std::string>;

class bit_pony {
  bit_pony(type t, format f, data &d)
      : m_is_valid(false), m_type(t), m_fmt(f), m_data(std::move(d)) {
    switch (m_fmt) {
    case format::binary:
      parse();
      break;
    case format::json:
      validate();
      build();
      break;
    case format::validate:
      validate();
      break;
    default:
      break;
    }
  }

private:
  void validate() {}

  bool parse() { return true; }

  bool build() { return true; }

private:
  bool m_is_valid;
  format m_fmt;
  data m_data;
  type m_type;
};

} // namespace bit_pony
} // namespace wallet
} // namespace excelsecu
