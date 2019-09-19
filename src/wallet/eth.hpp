//
//  eth.hpp
//  transmit
//
//  Created by zhouhuang on 2019/9/9.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once
#include "wallet.hpp"

namespace excelsecu {
namespace eth {
    
int get_chain_id(const std::string &net_type)
{
  if (net_type == wallet::eth_main) {
    return 1;
  } else if (net_type == wallet::eth_rinkeby) {
    return 4;
  } else if (net_type == wallet::eth_ropsten) {
    return 3;
  }
    return -1;
}
    
} // namespace eth
} // namespace excelsecu
