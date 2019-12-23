//
//  rlp.hpp
//  transmit
//
//  Created by zhouhuang on 2019/10/10.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "bytestream.hpp"
#include "RLP.h"

#include <json/json.hpp>

using json = nlohmann::json;

namespace excelsecu {
    namespace eth {
        
    static bytestream rlp_encoding(json tx, json vrs) {
        EncodeEthereumSignTx new_msg;
        EncodeEthereumTxRequest new_tx;
        memset(&new_msg, 0, sizeof(new_msg));
        memset(&new_tx, 0, sizeof(new_tx));
        static char rawTx[256];
        uint64_t raw_tx_bytes[24];
        
        auto nonce = tx["nonce"].get<std::string>();
        
        wallet_encode_element((pb_byte_t*)nonce.c_str(), nonce.length(),
                              new_msg.nonce.bytes, &(new_msg.nonce.size), false);
        
        auto gasPrice = itobs(tx["gasPrice"].get<uint64_t>());
        wallet_encode_element((pb_byte_t*)gasPrice.bytes(), gasPrice.length(),
                              new_msg.gas_price.bytes, &(new_msg.gas_price.size), false);
        
        auto gasLimit = tx["gasLimit"].get<std::string>();
        wallet_encode_element((pb_byte_t*)gasLimit.c_str(), gasLimit.length(),
                              new_msg.gas_limit.bytes, &(new_msg.gas_limit.size), false);
        
        auto outputAddress = bytestream(tx["output"]["address"].get<std::string>());
        wallet_encode_element((pb_byte_t*)outputAddress.bytes(), outputAddress.length(), new_msg.to.bytes,
                              &(new_msg.to.size), false);
        
        auto outputValue = itobs(tx["output"]["value"].get<uint64_t>());
        wallet_encode_element((pb_byte_t*)outputValue.bytes(), outputValue.length(),
                              new_msg.value.bytes, &(new_msg.value.size), false);
        
        auto data = tx["data"].get<std::string>(); // s300 当做字符串处理，看正常时当成hexstring处理，目前与s300保持一致
        wallet_encode_element((pb_byte_t*)data.c_str(), data.length(), new_msg.data_initial_chunk.bytes,
                              &(new_msg.data_initial_chunk.size), false);

        auto v = vrs["V"].get<uint32_t>();
        wallet_encode_int(v, (pb_byte_t *)&(new_tx.signature_v));
        
        auto r = bytestream(vrs["R"].get<std::string>());
        wallet_encode_element((pb_byte_t *)r.bytes(), r.length(),
                              new_tx.signature_r.bytes, &(new_tx.signature_r.size), true);
        
        auto s = bytestream(vrs["S"].get<std::string>());
        wallet_encode_element((pb_byte_t *)s.bytes(), s.length(),
                              new_tx.signature_s.bytes, &(new_tx.signature_s.size), true);

        int length = wallet_encode_list(&new_msg, &new_tx, raw_tx_bytes);
        int8_to_char((uint8_t *) raw_tx_bytes, length, rawTx);
        return bytestream(std::string(rawTx));
    }
//        static std::string to_binary(uint64_t x) {
//            if (x == 0) return "";
//            return to_binary(uint64_t(x/256)) + char(x % 256);
//        }
//
//        static bytestream encode_length(uint64_t len, uint64_t offset) {
//            if (len < 56) return bytestream{ (unsigned char)(len + offset) }; // 0x80 + len or 0xC0 + len
//            else if (len < pow(256, 8))
//            {
//                auto bl = to_binary(len);
//                return bytestream{ (unsigned char)(bl.length() + offset + 55) } + bytestream(bl.c_str(), bl.length());             // 0xB7 + len(bl) + bl or 0xF7 + len(bl) + bl
//            }
//            else {
//                throw std::runtime_error("input too long");
//            }
//        }
//
//        static bytestream rlp_encoding(const std::string &str) // ascii 字符串
//        {
//            if (str.length() == 1 && str[0] < 128)
//            {
//                return str;
//            }
//
//            auto prefix = encode_length(str.length(), 128);
//            return prefix + bytestream(str.c_str(), str.length());
//        }
//
//        static bytestream rlp_encoding(const bytestream &bs) // hexstring
//        {
//            if (bs.length() == 0x01 && bs[0] < 128) {
//                return bs;
//            }
//            auto prefix = encode_length(bs.length(), 128);
//            return prefix + bs;
//        }
//
//        static bytestream rlp_encoding(const std::vector<bytestream>& list)
//        {
//            bytestream ret;
//            for (auto bs: list) {
//                ret += bs;
//            }
//            auto prefix = encode_length(ret.length(), 0xC0);
//            return prefix + ret;
//        }
    }
}
