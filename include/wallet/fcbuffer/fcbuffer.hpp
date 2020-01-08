//
//  fcbuffer.hpp
//  transmit
//
//  Created by zhiou on 2019/8/26.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <algorithm/base/base58.hpp>
#include <bytestream.hpp>
#include <error.hpp>
#include <json/json.hpp>
#include <algorithm/hash/hash.hpp>
#include <wallet/wallet.hpp>

#include <bitset>
#include <sstream>

using json = nlohmann::json;

namespace excelsecu {
namespace wallet {
namespace fcbuffer {


// 小端序
uint64_t encode_name(const std::string &name) {
    std::string charmap = ".12345abcdefghijklmnopqrstuvwxyz";
    
    auto charidx = [&](char ch) {
        size_t idx = charmap.find(ch);
        if (idx == std::string::npos) {
            throw tsm_err("invalid base32 character", ERROR_COMMON_INVALID_PARAM);
        }
        return idx;
    };
    
    if (name.length() > 12) {
        throw tsm_err("invalid name, length greater than 12",
                      ERROR_COMMON_INVALID_PARAM);
    }
    
    std::string bitstr = "";
    for (int i = 0; i <= 12; ++i) {
        auto c = i < name.length() ? charidx(name[i]) : 0;
        auto bits = std::bitset<5>(c);
        bitstr += bits.to_string();
    }
    uint64_t value = std::bitset<64>(bitstr).to_ullong();
    return value;
}

bytestream encode(const std::string &name) {
    auto result = bytestream();
    result.append(encode_name(name));
    return result.little_ending();
}

//小端序
std::string decode_name(const std::string &code) {
    std::string charmap = ".12345abcdefghijklmnopqrstuvwxyz";
    
    auto hex = std::stoull(code, nullptr, 16);
    
    std::bitset<64> bytes(hex);
    std::string bits_str = bytes.to_string();
    
    std::string name = "";
    for (int i = 0; i < 12; ++i) {
        std::bitset<5> idx_bit(bits_str.substr(i * 5, 5));
        size_t idx = idx_bit.to_ulong();
        if (idx == 0)
            continue;
        name += charmap[idx];
    }
    
    return name;
}

//小端序
std::string decode_name(uint64_t code) {
    std::string charmap = ".12345abcdefghijklmnopqrstuvwxyz";
    
    auto hex = code;
    
    std::bitset<64> bytes(hex);
    std::string bits_str = bytes.to_string();
    
    std::string name = "";
    for (int i = 0; i < 12; ++i) {
        std::bitset<5> idx_bit(bits_str.substr(i * 5, 5));
        size_t idx = idx_bit.to_ulong();
        if (idx == 0)
            continue;
        name += charmap[idx];
    }
    return name;
}

bytestream serialize(const json tx) {
    bytestream result;
    auto expiration = itobs(tx["expiration"].get<uint32_t>()).little_ending();
    result += expiration;
    
    auto ref_block_num = itobs(tx["ref_block_num"].get<uint16_t>()).little_ending();
    result += ref_block_num;
    
    auto ref_block_prefix = itobs(tx["ref_block_prefix"].get<uint32_t>()).little_ending();
    result += ref_block_prefix;
    
    auto max_net_usage_words = itobs(tx["max_net_usage_words"].get<uint32_t>()).little_ending();
    result += max_net_usage_words;
    
    auto max_cpu_usage_ms = itobs(tx["max_cpu_usage_ms"].get<uint32_t>()).little_ending();
    result += max_cpu_usage_ms;
    
    auto delay_sec = itobs(tx["delay_sec"].get<uint32_t>()).little_ending();
    result += delay_sec;
    
    auto context_free_actions = tx["context_free_actions"];
    result.append((uint8_t)context_free_actions.size());
    for (auto action: context_free_actions) {
        auto account = action["account"].get<std::string>();
        result += encode(account);
        
        auto name = action["name"].get<std::string>();
        result += encode(name);
        
        auto authorizations = action["authorization"];
        for (auto auth: authorizations) {
            result += encode(auth["actor"].get<std::string>());
            result += encode(auth["permission"].get<std::string>());
        }
        auto data = bytestream(action["data"].get<std::string>());
        result.append((uint8_t)data.length());
        result += data;
    }
    
    auto actions = tx["actions"];
    result.append((uint8_t)actions.size());
    for (auto action: actions) {
        auto account = action["account"].get<std::string>();
        result += encode(account);
        
        auto name = action["name"].get<std::string>();
        result += encode(name);
        
        auto authorizations = action["authorization"];
        result.append((uint8_t)authorizations.size());
        for (auto auth: authorizations) {
            result += encode(auth["actor"].get<std::string>());
            result += encode(auth["permission"].get<std::string>());
        }
        
        auto data = bytestream(action["data"].get<std::string>());
        result.append((uint8_t)data.length());
        result += data;
    }
    auto transaction_extensions = tx["transaction_extensions"];
    result.append((uint8_t)transaction_extensions.size());
    return result;
}

} // namespace fcbuffer
} // namespace wallet
} // namespace excelsecu
