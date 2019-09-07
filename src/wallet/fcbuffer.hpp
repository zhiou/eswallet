//
//  fcbuffer.hpp
//  transmit
//
//  Created by zhiou on 2019/8/26.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "base58.hpp"
#include "bytestream.hpp"
#include "error.hpp"
#include "hash.hpp"
#include "wallet.hpp"

#include <bitset>
#include <sstream>

namespace excelsecu {
namespace wallet {
namespace fcbuffer {
// 小端序
uint64_t encode_name(const std::string &name) {
  std::string charmap = ".12345abcdefghijklmnopqrstuvwxyz";

  auto charidx = [&](char ch) {
    size_t idx = charmap.find(ch);
    if (idx == std::string::npos) {
      throw tsm_err("invalid base32 character", ERROR_COMMON_INVALID_PARAM);
    }
    return idx;
  };

  if (name.length() > 12) {
    throw tsm_err("invalid name, length greater than 12",
                  ERROR_COMMON_INVALID_PARAM);
  }

  std::string bitstr = "";
  for (int i = 0; i <= 12; ++i) {
    auto c = i < name.length() ? charidx(name[i]) : 0;
    auto bits = std::bitset<5>(c);
    bitstr += bits.to_string();
  }
  uint64_t value = std::bitset<64>(bitstr).to_ullong();
  return value;
}

//小端序
std::string decode_name(const std::string &code) {
  std::string charmap = ".12345abcdefghijklmnopqrstuvwxyz";

  auto hex = std::stoull(code, nullptr, 16);

  std::bitset<64> bytes(hex);
  std::string bits_str = bytes.to_string();

  std::string name = "";
  for (int i = 0; i < 12; ++i) {
    std::bitset<5> idx_bit(bits_str.substr(i * 5, 5));
    size_t idx = idx_bit.to_ulong();
    if (idx == 0)
      continue;
    name += charmap[idx];
  }

  return name;
}

//小端序
std::string decode_name(uint64_t code) {
  std::string charmap = ".12345abcdefghijklmnopqrstuvwxyz";

  auto hex = code;

  std::bitset<64> bytes(hex);
  std::string bits_str = bytes.to_string();

  std::string name = "";
  for (int i = 0; i < 12; ++i) {
    std::bitset<5> idx_bit(bits_str.substr(i * 5, 5));
    size_t idx = idx_bit.to_ulong();
    if (idx == 0)
      continue;
    name += charmap[idx];
  }

  return name;
}
} // namespace fcbuffer
} // namespace wallet
} // namespace excelsecu
