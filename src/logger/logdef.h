//
//  logdef.h
//  transmit
//
//  Created by zhiou on 2019/8/19.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

namespace excelsecu {
enum class Priority { info, debug, warn, error };

#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_OFF 5
} // namespace excelsecu
