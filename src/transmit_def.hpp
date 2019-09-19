//
//  transmit_def.hpp
//  transmit
//
//  Created by zhiou on 2019/8/7.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "error.hpp"

namespace excelsecu {

enum class event_msg {
  begin, // notify all should prepare for connect
  connected,
  disconnected,    // notify all device is disconnected, security data should be
                   // cleared.
  will_authencate, // notify all authenticate will begin
  did_authencate,
  will_handshake, // notify all handshake will begin
  did_handshake,
  complete, // notify all transmit pipe has being built.
  failed,   // notify all some error happened,
  released  // 释放所有事件
};

}
