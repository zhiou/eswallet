//
//  cipher.hpp
//  transmit
//
//  Created by zhiou on 2019/7/29.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "bytestream.hpp"
#include "../pkcs/pubkey.hpp"
#include <string>
#include "EsCommon.h"
#include "error.hpp"

namespace excelsecu {

namespace cipher {

enum class type { tdes112, sm4 };


#pragma mark - 非对称加密及验签
static inline bytestream encrypt(const pubkey &pub, const bytestream& plain) {
    
    u4 estimateCipherLen = pub.size() == 0x40 ? (u4)plain.length() + 0x60 : (u4)pub.size();
    u4 acturalCipherLen = estimateCipherLen;
    auto cipher_buf = bytestream(estimateCipherLen);
    
    
    u4 u4Result = EsAsymEnc(pub.to_blkAsymKey(), (const void*)plain.bytes(), (u4)plain.length(), (void*)cipher_buf.mem().data(), (u4*)&acturalCipherLen);
    if (u4Result) { throw tsm_err("asym encrypt failed", u4Result); }
    
    cipher_buf.drop_tail(estimateCipherLen - acturalCipherLen);
    return cipher_buf;
}

static inline bool verify(const pubkey &pub, const bytestream& hash, const bytestream& raw_sign) {
    bool is_sm2 = pub.size() == 0x40;
    
    if (is_sm2) {
        return EsSm2Verify(pub.data(), pub.data() + 0x20, hash.bytes(), raw_sign.bytes(), raw_sign.bytes() + 0x20) == 0;
    } else {
        u4 estimateSignMsgLen = 0x400;
        u4 actualSignMsgLen = estimateSignMsgLen;
        auto sign = bytestream(estimateSignMsgLen);
        
        u4 u4Result = EsAsymEnc(pub.to_blkAsymKey(), raw_sign.bytes(), raw_sign.length(), sign.mem().data(), &actualSignMsgLen);
        if (u4Result) { throw tsm_err("asym verify failed", u4Result);  }
        
        u4Result = EsPaddingRemove(sign.mem().data(), actualSignMsgLen, PADDING_TYPE_PKCS_1, pub.size(), sign.mem().data(), &actualSignMsgLen);
        if (u4Result) { throw tsm_err("asym verify failed", u4Result);  }
        
        sign.drop_tail(estimateSignMsgLen - actualSignMsgLen);
        return sign == hash;
    }
}

bytestream encrypt(type t, const bytestream &key, const bytestream &plain);

bytestream decrypt(type t, const bytestream &key, const bytestream &cipher);

bytestream mac(type t, const bytestream &key, const bytestream &plain);
} // namespace cipher
} // namespace excelsecu
