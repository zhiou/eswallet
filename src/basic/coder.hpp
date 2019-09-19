//
//  coder.hpp
//  transmit
//
//  Created by zhiou on 2019/7/3.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "bytestream.hpp"
#include <string>

namespace excelsecu {

class coder {
public:
  virtual std::string encode(const bytestream &) const = 0;
  virtual bytestream decode(const std::string &) const = 0;
};

class hex : public coder {
  std::string encode(const bytestream &bs) const override final;
};

class base64 : public coder {
  std::string encode(const bytestream &bs) const override final;
};
} // namespace excelsecu
