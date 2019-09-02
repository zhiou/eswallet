//
//  ble_driver.hpp
//  transmit
//
//  Created by zhiou on 2019/8/6.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#ifndef ble_driver_hpp
#define ble_driver_hpp

#include <string>
#include <functional>
#include "EsDriver.h"

namespace excelsecu {
    
    class ble_driver {
    public:
        static bool connect(const std::string &sn);
        
        static bool disconnect();
        
        static bool isconnected();
        
        static bool send(unsigned char* pu1Send, unsigned int u4SendLen, unsigned char* pu1Recv, unsigned int* pu4RecvLen);
        
        static void set_callback(FunPtlDevChange callback);
    };
}

#endif /* ble_driver_hpp */
