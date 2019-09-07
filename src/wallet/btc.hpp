//
//  btc.hpp
//  transmit
//
//  Created by zhiou on 2019/8/23.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "json.hpp"
#include "wallet.hpp"

namespace excelsecu {
namespace wallet {

using json = nlohmann::json;
namespace btc {

json get_addr_prefix(const std::string &net_type) {
  if (net_type == btc_main) {
    return {{"messagePrefix", char(0x18) + "Bitcoin Signed Message:\n"},
            {"bech32", "bc"},
            {"bip32", {{"public", 0x0488b21e}, {"private", 0x0488ade4}}},
            {"pubKeyHash", 0x00},
            {"scriptHash", 0x05},
            {"wif", 0x80}};
  } else if (net_type == btc_testnet3) {
    return {{"messagePrefix", char(0x18) + "Bitcoin Signed Message:\n"},
            {"bech32", "tb"},
            {"bip32", {{"public", 0x043587cf}, {"private", 0x04358394}}},
            {"pubKeyHash", 0x6f},
            {"scriptHash", 0xc4},
            {"wif", 0xef}};
  }
  return {};
}

// TODO
json derive_address(std::string version, std::string pubkey,
                    std::string chain_code, bool external, size_t from_index,
                    size_t to_index) {
  return {};
}
} // namespace btc
} // namespace wallet
} // namespace excelsecu
