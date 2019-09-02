//
//  hasher.hpp
//  transmit
//
//  Created by zhiou on 2019/7/29.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <string>

namespace excelsecu {

    
    class hasher {
    public:
        virtual bytestream hash(const bytestream&) const = 0;
    };
}
