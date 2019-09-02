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

namespace excelsecu {
    
    class padding_handler;
    
    
    class pubkey {
    public:
        const unsigned char* data() const;
        const size_t size() const;
    public:
        
        virtual bytestream encrypt(const bytestream& plain_text) const = 0;
        
        virtual bool verify(const bytestream& raw_sign) const = 0; // 签名裸数据验签， p7包验签由pkcs7_signature进行
    };
    
    class rsa_pubkey: public pubkey {
    public:
        bytestream get_N() const;
    public:
        rsa_pubkey(const bytestream& raw);

    };
    
    class sm2_pubkey: public pubkey {
        static constexpr unsigned int PUBLIC_KEY_SIZE            = 65;
        static constexpr unsigned int COMPRESSED_PUBLIC_KEY_SIZE = 33;
    public:
        bytestream get_X() const;
        bytestream get_Y() const;
    public:
        sm2_pubkey(const bytestream& raw); // 支持压缩公钥
    
    };
}
