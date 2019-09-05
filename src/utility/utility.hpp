//
//  utility.hpp
//  transmit
//
//  Created by zhiou on 2019/8/22.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <chrono>

namespace excelsecu {
    namespace utility {
        inline std::vector<std::string> split(const std::string& s, char delimiter)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(s);
                while (std::getline(tokenStream, token, delimiter))
                {
                    tokens.push_back(token);
                }
            return tokens;
        }
        
        inline uint64_t get_tick_count() {
            auto d = std::chrono::steady_clock::now().time_since_epoch();
            
            std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(d);
            return timestamp.count();
        }
        
        inline uint64_t get_tick_span(uint64_t _old_tick)
        {
            uint64_t cur_tick = get_tick_count();
            if (_old_tick > cur_tick) return 0;
            
            return cur_tick - _old_tick;
        }
    }
}




