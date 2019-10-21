//
//  validate.h
//  transmit
//
//  Created by zhiou on 2019/7/1.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <string>

namespace excelsecu {
class configure;
bool is_valid(const std::string &sn);
bool is_valid(const configure &config);
} // namespace excelsecu
