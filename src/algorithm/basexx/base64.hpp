//
//  base64.hpp
//  transmit
//
//  Created by fire on 2019/08/05.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

