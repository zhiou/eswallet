//
//  configuration.hpp
//  transmit
//
//  Created by zhiou on 2019/7/1.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#ifndef configuration_hpp
#define configuration_hpp

#include <memory>
#include <string>

namespace excelsecu {
    class configure {
    public:
        
        static configure& current() {
            static configure s_current_config;
            return s_current_config;
        }
        
        // static config
        
        
        // runtime config
        std::string host_name() const{
            return m_host_name;
        }
        
        void set_host_name(const std::string &hostname) {
            m_host_name = hostname;
        }
        
        void save_auth_str(const std::string & auth_str) {
            m_auth_str = auth_str;
        }
        
        std::string get_auth_str() const {
            return m_auth_str;
        }
    private:
        std::string m_host_name;
        std::string m_auth_str;
    };
}

#endif /* configuration_hpp */
