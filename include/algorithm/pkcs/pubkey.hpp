//
//  pubkey.hpp
//  transmit
//
//  Created by zhiou on 2019/7/3.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <memory>
#include <mutex>

#include "bytestream.hpp"
#include "EsCommon.h"

namespace excelsecu {

class padding_handler;


class pubkey {
public:
  unsigned char *data() const;
  size_t size() const;
  const BlkAsymKey* to_blkAsymKey() const;
};


class rsa_pubkey : public pubkey {
public:
  bytestream get_N() const;

public:
  rsa_pubkey(const bytestream &raw);
};

class sm2_pubkey : public pubkey {
  static constexpr unsigned int PUBLIC_KEY_SIZE = 65;
  static constexpr unsigned int COMPRESSED_PUBLIC_KEY_SIZE = 33;

public:
  bytestream get_X() const;
  bytestream get_Y() const;

public:
  sm2_pubkey(const bytestream &raw); // 支持压缩公钥
};


template <class T>
struct is_sm2 {
    static const bool value = false;
};

template <>
struct is_sm2<sm2_pubkey> {
    static const bool value = true;
};


} // namespace excelsecu
