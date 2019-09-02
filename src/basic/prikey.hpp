//
//  prikey.hpp
//  transmit
//
//  Created by zhiou on 2019/7/3.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "sm2.hpp"

namespace excelsecu {
    
    class bytestream;
    class pubkey;
    
    class prikey {
    public:
        prikey() = default;
    private:
        prikey(const prikey&);
        prikey& operator=(const prikey&);
    public:
        virtual bytestream sign(const bytestream& hash) const = 0;
        virtual bytestream decrypt(const bytestream& cipher_text) const = 0;
    };
    
//    // 私钥镜像，映射device内的私钥;
//    // should move to device group
//    class prikey_image: public prikey {
//    public:
//        prikey_image(unsigned int key_index);
//    };
    
    class rsa_prikey: public prikey {
    private:
        
    };
    
    class sm2_prikey: public prikey {
    public:
        sm2_prikey(const bytestream& raw):prikey() {
            auto temp = std::unique_ptr<bytestream>(new bytestream(raw));
            m_prikey = std::move(temp);
        }
        
        bytestream sign(const bytestream& hash) const override final {
            return sm2::sign(hash, *(this->m_prikey.get()));
        }
        
        bytestream decrypt(const bytestream& cipher) const override final {
            return sm2::decrypt(cipher, *(this->m_prikey.get()));
        }
        
    public:
        pubkey get_pubkey() const; // SM2 可从私钥计算出公钥
    private:
        std::unique_ptr<bytestream> m_prikey; // BitCoin 用了更安全的内存分配策略，可参考
    };
}
