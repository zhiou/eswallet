//
//  wallet.hpp
//  transmit
//
//  Created by zhiou on 2019/8/22.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <string>

namespace excelsecu {
namespace wallet {

enum class coin {
  btc,
  eth,
  eos,
};

enum class applet {
  btc,
  eth,
  eos,
  hdwallet,
  manager,
  backup,
};

// net type
const std::string btc_main      = "btc_main";
const std::string btc_testnet3  = "btc_testnet3";
const std::string eth_main      = "eth_main";
const std::string eth_rinkeby   = "eth_rinkeby";
const std::string eos_main      = "eos_main";
const std::string eth_ropsten   = "eth_ropsten";
const std::string eos_jungle    = "eos_jungle";
const std::string eos_kylin     = "eos_kylin";
const std::string eos_sys       = "eos_sys";

bool is_btc(const std::string &coin_type_str) {
  return coin_type_str.substr(0, 3) == "btc";
}

bool is_eth(const std::string &coin_type_str) {
  return coin_type_str.substr(0, 3) == "eth";
}

bool is_eos(const std::string &coin_type_str) {
  return coin_type_str.substr(0, 3) == "eos";
}

static std::string sdk_version = "0.0.1";
} // namespace wallet
} // namespace excelsecu
