//
//  cipher.hpp
//  transmit
//
//  Created by zhiou on 2019/7/29.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <string>
#include "bytestream.hpp"

namespace excelsecu {
    
    namespace cipher {
        
        enum class type {
            tdes112,
            sm4
        };

        bytestream encrypt(type t, const bytestream& key , const bytestream& plain);
            
        bytestream decrypt(type t, const bytestream& key, const bytestream& cipher);
        
        bytestream mac(type t, const bytestream& key, const bytestream& plain);
    }
}

