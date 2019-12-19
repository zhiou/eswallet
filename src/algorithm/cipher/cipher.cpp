//
//  cipher.cpp
//  transmit
//
//  Created by zhiou on 2019/7/29.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#include <bytestream.hpp>

#include <EsCommon/EsCommon.h>
#include <algorithm/cipher/cipher.hpp>

namespace excelsecu {
namespace cipher {
u4 get_alg_id(type t) {
  switch (t) {
  case type::tdes112:
    return ES_SYMM_ALG_3DES112;
  case type::sm4:
    return ES_SYMM_ALG_SM4;
  default:
    throw tsm_err("unsupported cipher algorithm", ERROR_COMMON_INVALID_PARAM);
  }
}

bytestream encrypt(type t, const bytestream &key, const bytestream &plain) {
  u4 cipher_len = (u4)plain.length() + 0x10;
  std::unique_ptr<ebyte> cipher(new ebyte[cipher_len]);

  u4 ret = EsSymmEncEcb(get_alg_id(t), key.bytes(), plain.bytes(),
                        (u4)plain.length(), cipher.get(), &cipher_len);
  if (ret) {
    throw tsm_err("TDES112 encrypt failed", ret);
  }

  return bytestream(cipher.get(), cipher_len);
}

bytestream decrypt(type t, const bytestream &key, const bytestream &cipher) {
  u4 plain_len = (u4)cipher.length();

  std::unique_ptr<ebyte> plain(new ebyte[plain_len]);

  u4 ret = EsSymmDecEcb(get_alg_id(t), key.bytes(), cipher.bytes(),
                        (u4)cipher.length(), plain.get(), &plain_len);
  if (ret) {
    throw tsm_err("TDES112 decrypt failed", ret);
  }

  return bytestream(plain.get(), plain_len);
}

bytestream mac(type t, const bytestream &key, const bytestream &plain) {
  u4 mac_len = 0x04;
  std::unique_ptr<ebyte> mac(new ebyte[mac_len]);

  u4 ret = EsSymmMac(
      ES_SYMM_ALG_SM4, key.bytes(),
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
      plain.bytes(), (u4)plain.length(), mac.get(), &mac_len);
  if (ret) {
    throw tsm_err("SM4 mac failed", ret);
  }

  return bytestream(mac.get(), mac_len);
}

} // namespace cipher
} // namespace excelsecu
