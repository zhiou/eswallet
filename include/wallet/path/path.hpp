//
//  path.hpp
//  transmit
//
//  Created by zhiou on 2019/8/22.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "bytestream.hpp"
#include "utility.hpp"

#include <sstream>
#include <string>

namespace excelsecu {
namespace wallet {
namespace path {

bytestream to_buffer(const std::string &path)
{
  auto parts = utility::split(path, '/');

  auto begin = std::begin(parts);
  if (parts[0] == "m")
    begin++;

  bytestream buffer;
  for (auto iter = begin; iter != std::end(parts); ++iter) {
    uint32_t value = 0x00000000;
    auto str = *iter;
    if (str.back() == '\'') {
      str = str.substr(0, iter->length() - 1);
      value |= 0x80000000;
    }
    uint32_t num = stoi(*iter);
    value |= num;
    buffer.append(value);
  }
  return buffer;
}

std::string make_path(const std::vector<uint32_t> &indexes)
{
    std::string path = "m";
    for (auto index: indexes) {
        path += "/";
        bool hard = ((index & 0x80000000) > 0);
        if (hard) index -= 0x80000000;
        path += std::to_string(index);
        if (hard) path += "'";
    }
    return path;
}
//TODO
std::string from_buffer(const bytestream &buffer) {
    size_t level = buffer.length() / 4;
    std::vector<uint32_t> indexes;
    for (int i = 0; i < level; ++i) {
        indexes.push_back(buffer.readUInt32(i*4));
    }
    return make_path(indexes);
}
} // namespace path
} // namespace wallet
} // namespace excelsecu
