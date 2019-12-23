//
//  eos.hpp
//  Wallet
//
//  Created by zhouhuang on 2019/12/21.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once
#include "../wallet.hpp"
#include "bytestream.hpp"

namespace excelsecu {
namespace eos {
    
bytestream get_chain_id(const std::string &net_type)
{
  if (net_type == wallet::eos_main) {
    return bytestream("aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906");
  } else if (net_type == wallet::eos_jungle) {
    return bytestream("e70aaab8997e1dfce58fbfac80cbbb8fecec7b99cf982a9444273cbc64c41473");
  } else if (net_type == wallet::eos_kylin) {
    return bytestream("5fff1dae8dc8e2fc4d5b23b2c7665c97f9e9d8edf2b6485a86ba311c25639191");
  } else if (net_type == wallet::eos_sys) {
    return bytestream("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f");
  }
    return bytestream();
}
    
} // namespace eos
} // namespace excelsecu

