//
//  audio_driver.hpp
//  transmit
//
//  Created by zhiou on 2019/8/6.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#ifndef audio_driver_hpp
#define audio_driver_hpp

#include <mutex>
#include <string>

#include "EsDriver.h"

namespace excelsecu {

class audio_driver {

public:
  static bool connect(const std::string &sn);

  static bool disconnect();

  static bool isconnected();

  static bool send(unsigned char *pu1Send, unsigned int u4SendLen,
                   unsigned char *pu1Recv, unsigned int *pu4RecvLen);

  static void set_callback(FunPtlDevChange callback);
};
} // namespace excelsecu

#endif /* audio_driver_hpp */
