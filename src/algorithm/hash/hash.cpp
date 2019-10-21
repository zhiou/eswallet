//
//  sm3.cpp
//  transmit
//
//  Created by zhiou on 2019/8/16.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#include <algorithm/hash/hash.hpp>
#include "EsCommon.h"
#include <array>

extern "C" {

#if __BIG_ENDIAN__ || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) ||\
__ARMEB__ || __THUMBEB__ || __AARCH64EB__ || __MIPSEB__
#define WORDS_BIGENDIAN        1
#endif
#define DETERMINISTIC          1
#define USE_BASIC_CONFIG       1
#define ENABLE_MODULE_RECOVERY 1

#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wconditional-uninitialized"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include "../../../secp256k1/src/basic-config.h"
#include "../../../secp256k1/src/secp256k1.c"
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

};

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
