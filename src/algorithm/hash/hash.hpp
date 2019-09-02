//
//  sm3.h
//  transmit
//
//  Created by fire on 2019/08/04.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "bytestream.hpp"

namespace excelsecu {
    namespace sm3 {
        bytestream hash(const bytestream& msg);
    }
    
    namespace sha256 {
        bytestream hash(const bytestream& msg);
    }
    
    namespace ripemd160 {
        bytestream hash(const bytestream& msg);
    }
    
    namespace keccak256 {
        bytestream hash(const bytestream& msg);
    }
}
