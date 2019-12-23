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
#include <sstream>

using json = nlohmann::json;

namespace excelsecu {
namespace eth {

//
static void rlp_encode_int(json j, pb_byte_t *new_bytes, pb_size_t *new_size) {
    uint64_t n = 0;
    if (j.is_number()) {
        n = j.get<uint64_t>();
        if (n == 0) {
            wallet_encode_element((pb_byte_t*)"", (pb_size_t)0,
            new_bytes, new_size, false);
            return ;
        }
        auto ele = itobs(n);
        wallet_encode_element((pb_byte_t*)ele.bytes(), (pb_size_t)ele.length(),
                              new_bytes, new_size, false);
        return ;
    }
    else if (j.is_string()) {
        auto str = j.get<std::string>();
       wallet_encode_element((pb_byte_t*)str.c_str(), (pb_size_t)str.length(),
       new_bytes, new_size, false);
    }
}

static bytestream rlp_encoding(json tx, json vrs) {
    EncodeEthereumSignTx new_msg;
    EncodeEthereumTxRequest new_tx;
    memset(&new_msg, 0, sizeof(new_msg));
    memset(&new_tx, 0, sizeof(new_tx));
    static char rawTx[256];
    uint64_t raw_tx_bytes[24];
    
    //JS SDK 里nonce 和 gasLimit 为0 时会当成空字符串处理，所以rlp编码为0x80, 但当数字处理应该是0x00, 导致s两边签名结果不一致
    auto nonce = tx["nonce"];
    rlp_encode_int(nonce, new_msg.nonce.bytes, &(new_msg.nonce.size));
    
    auto gasPrice = tx["gasPrice"];
    rlp_encode_int(gasPrice, new_msg.gas_price.bytes, &(new_msg.gas_price.size));
    
    auto gasLimit = tx["gasLimit"];
   rlp_encode_int(gasLimit, new_msg.gas_limit.bytes, &(new_msg.gas_limit.size));
    
    
    auto outputAddress = bytestream(tx["output"]["address"].get<std::string>());
    wallet_encode_element((pb_byte_t*)outputAddress.bytes(), (pb_size_t)outputAddress.length(), new_msg.to.bytes,
                          &(new_msg.to.size), false);
    
    auto outputValue = itobs(tx["output"]["value"].get<uint64_t>());
    wallet_encode_element((pb_byte_t*)outputValue.bytes(), (pb_size_t)outputValue.length(),
                          new_msg.value.bytes, &(new_msg.value.size), false);
    
    auto data = tx["data"].get<std::string>(); // s300 当做字符串处理，看正常时当成hexstring处理，目前与s300保持一致
    wallet_encode_element((pb_byte_t*)data.c_str(), (pb_size_t)data.length(), new_msg.data_initial_chunk.bytes,
                          &(new_msg.data_initial_chunk.size), false);
    
    auto v = vrs["V"].get<uint32_t>();
    wallet_encode_int(v, (pb_byte_t *)&(new_tx.signature_v));
    
    auto r = bytestream(vrs["R"].get<std::string>());
    wallet_encode_element((pb_byte_t *)r.bytes(), (pb_size_t)r.length(),
                          new_tx.signature_r.bytes, &(new_tx.signature_r.size), true);
    
    auto s = bytestream(vrs["S"].get<std::string>());
    wallet_encode_element((pb_byte_t *)s.bytes(), (pb_size_t)s.length(),
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
