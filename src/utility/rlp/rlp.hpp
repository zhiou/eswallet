//
//  rlp.hpp
//  transmit
//
//  Created by zhouhuang on 2019/10/10.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "bytestream.hpp"

namespace excelsecu {
    namespace eth {
        
        static std::string to_binary(uint64_t x) {
            if (x == 0) return "";
            return to_binary(int(x/256)) + char(x % 256);
        }
        
        static bytestream encode_length(uint64_t len, uint64_t offset) {
            if (len < 56) return bytestream{ (unsigned char)(len + offset) }; // 0x80 + len or 0xC0 + len
            else if (len < pow(256, 8))
            {
                auto bl = to_binary(len);
                return bytestream{ (unsigned char)(bl.length() + offset + 55) } + bytestream(bl.c_str(), bl.length());             // 0xB7 + len(bl) + bl or 0xF7 + len(bl) + bl
            }
            else {
                throw std::runtime_error("input too long");
            }
        }

        static bytestream rlp_encoding(std::string &str) // ascii 字符串
        {
            if (str.length() == 1 && str[0] < 128)
            {
                return str;
            }
           
            auto prefix = encode_length(str.length(), 128);
            return prefix + bytestream(str.c_str(), str.length());
        }
        
        static bytestream rlp_encoding(const bytestream &bs) // hexstring
        {
            if (bs.length() == 0x01 && bs[0] < 128) {
                return bs;
            }
            auto prefix = encode_length(bs.length(), 128);
            return prefix + bs;
        }
        
        static bytestream rlp_encoding(const std::vector<bytestream>& list)
        {
            bytestream ret;
            for (auto bs: list) {
                ret += rlp_encoding(bs);
            }
            auto prefix = encode_length(ret.length(), 0xC0);
            return prefix + ret;
        }
    }
}
