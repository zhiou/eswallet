//
//  sm3.cpp
//  transmit
//
//  Created by zhiou on 2019/8/16.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#include "hash.hpp"
#include "EsCommon.h"
#include <array>

namespace excelsecu {
namespace sm3 {
bytestream hash(const bytestream &hash_msg) {
  std::array<unsigned char, 0x20> digest{};
  EsHashData(ES_HASH_ALG_SM3, (unsigned char *)hash_msg.bytes(),
             (u4)hash_msg.length(), digest.data());
  return bytestream(digest.data(), digest.size());
}
} // namespace sm3

namespace sha256 {
bytestream hash(const bytestream &hash_msg) {
  std::array<unsigned char, 0x20> digest{};
  EsHashData(ES_HASH_ALG_SHA256, (unsigned char *)hash_msg.bytes(),
             (u4)hash_msg.length(), digest.data());
  return bytestream(digest.data(), digest.size());
}
} // namespace sha256

namespace ripemd160 {
bytestream hash(const bytestream &hash_msg) {
  std::array<unsigned char, 20> digest{};
  EsHashData(ES_HASH_ALG_RIPEMD160, (unsigned char *)hash_msg.bytes(),
             (u4)hash_msg.length(), digest.data());
  return bytestream(digest.data(), digest.size());
}
} // namespace ripemd160

namespace keccak256 {
bytestream hash(const bytestream &hash_msg) {
  std::array<unsigned char, 0x20> digest{};
  EsHashData(ES_HASH_ALG_KECCAK256, (unsigned char *)hash_msg.bytes(),
             (u4)hash_msg.length(), digest.data());
  return bytestream(digest.data(), digest.size());
}
} // namespace keccak256

} // namespace excelsecu
