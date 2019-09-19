//
//  sm2.cpp
//  transmit
//
//  Created by zhiou on 2019/8/16.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "bytestream.hpp"
#include <utility>

namespace excelsecu {
namespace sm2 {

std::pair<std::string, std::string> gen_keypair();

bytestream encrypt(const bytestream &plain, const bytestream &pubkey);

bytestream decrypt(const bytestream &cipher, const bytestream &prikey);

bytestream sign(const bytestream &digest, const bytestream &prikey);

bool verify(const bytestream &digest, const bytestream &sign,
            const bytestream &pubkey);

} // namespace sm2
} // namespace excelsecu
