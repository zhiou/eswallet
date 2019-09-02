//
//  adress.hpp
//  transmit
//
//  Created by zhiou on 2019/8/23.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "base58.hpp"
#include "bytestream.hpp"
#include "wallet.hpp"
#include "error.hpp"
#include "hash.hpp"


namespace excelsecu {
namespace wallet {
    namespace address{
        
        enum class type{
            external,
            change,
            p2pkh,
            p2sh,
            p2wpkh,
            p2wsh,
            p2pk,
        };
        
    bytestream to_buffer(const std::string &address)
    {
        if (address.length() > 3 && address.substr(0, 3) == "EOS")     //eos
        {
            std::vector<unsigned char> ret;
            if (DecodeBase58Check(address.substr(3), ret)) {
                return bytestream(ret);
            }
            throw tsm_err("invalid eos address", ERROR_COMMON_INVALID_DATA);
        }
        else if (address.length() > 2 && address.substr(0, 2) == "0x")  //eth
        {
            return bytestream(address.substr(2));
        }
        else { //bitcoin
            //p2pkh & p2sh supported
            if (address.find("1mn23") == 0) // 如果首字节找到1,m,n,2,3 中的任意一个
            {
                std::vector<unsigned char> ret;
                if (DecodeBase58Check(address, ret))
                {
                    if (ret.size() != 21)
                    {
                        throw tsm_err("btc p2pkh/p2sh address length unmatched", ERROR_COMMON_DATA_LEN);
                    }
                    return bytestream(ret).tail(20);
                }
                 throw tsm_err("invalid btc address", ERROR_COMMON_INVALID_DATA);
            }
        }
        throw tsm_err("unknown or unsupported address", ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }
    
    std::string to_eth_checksum_address(const bytestream& buffer)
    {
        auto hash_value = keccak256::hash(buffer);
        
        std::string checksum_addr = "0x";
        std::string addr = buffer.hex_str();
        
        for (int i = 0; i < addr.length(); ++i)
        {
            checksum_addr += (hash_value[i] > 7) ? toupper(addr[i]) : tolower(addr[i]);
        }
        return checksum_addr;
    }
    
    std::string from_buffer(coin coin_type, const bytestream &buffer)
    {
        if (coin_type == coin::eth && buffer.length() == 20)
        {
            return to_eth_checksum_address(buffer);
        }
        else if(coin_type == coin::btc && buffer.length() == 21)
        {
            return EncodeBase58Check(buffer.mem());
        }
        else if(coin_type == coin::eos && buffer.length() == 33)
        {
            auto hash_value = ripemd160::hash(buffer);
            auto address_check = bytestream(buffer) + hash_value.front(4);
            return "EOS" + EncodeBase58(address_check.mem());
        }
        throw tsm_err("unsuppored coin type or address", ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }
}
}
}