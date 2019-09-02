//
//  basic_sink.hpp
//  transmit
//
//  Created by zhiou on 2019/8/19.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <mutex>
#include <string>

#include "sink.hpp"

namespace excelsecu {
    namespace sinks {
        
    class base_sink: public sink
    {
    public:
        base_sink() = default;
        base_sink(const base_sink &) = delete;
        base_sink &operator=(const base_sink &) = delete;
        
        void log(const std::string &msg) final
        {
            std::lock_guard<std::mutex> lock(mutex_);
            sink_it_(msg);
        }
        
        void flush() final
        {
            std::lock_guard<std::mutex> lock(mutex_);
            flush_();
        }
        
    protected:
        virtual void sink_it_(const std::string &msg) = 0;
        virtual void flush_() = 0;

        std::mutex mutex_;
    };
    }
}
