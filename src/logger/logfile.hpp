//
//  logfile.hpp
//  transmit
//
//  Created by zhiou on 2019/8/19.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

#include <sys/stat.h>
#include <sys/types.h>

#include "os.hpp"
#include "transmit_def.hpp"

namespace excelsecu {
    class logfile {
    public:
        explicit logfile() = default;
        
        logfile(const logfile &) = delete;
        logfile &operator=(const logfile &) = delete;
        
        ~logfile()
        {
            close();
        }
        
        void open(const std::string &filename, bool truncate = false) {
            close();
            auto mode = truncate ? "wb" : "ab";
            m_filename = filename;
     
            auto full_path = base_dir() + filename;
            m_fd = fopen(full_path.c_str(), mode);
            if (m_fd ){ return; }
            
            throw tsm_err("Failed opening file for writing", 0x6A82);
        }
        
        void reopen(bool truncate)
        {
            if (m_filename.empty())
            {
                throw tsm_err("reopen log file failed", 0x6A82);
            }
            open(m_filename, truncate);
        }
        
        void flush()
        {
            std::fflush(m_fd);
        }
        
        void close()
        {
            if (m_fd != nullptr)
            {
                std::fclose(m_fd);
                m_fd = nullptr;
            }
        }
        
        void write(const std::string &buf)
        {
            size_t msg_size = buf.length();
            auto data = buf.c_str();
            if (std::fwrite(data, 1, msg_size, m_fd) != msg_size)
            {
                throw tsm_err("Failed writing to file", 0x6A82);
            }
        }
        
        size_t size() const
        {
            if (m_fd == nullptr)
            {
                throw tsm_err("Cannot use size() on closed file", 0x6A82);
            }
            struct stat st;
            
            int fd = fileno(m_fd);
            if (::fstat(fd, &st) == 0)
            {
                return static_cast<size_t>(st.st_size);
            }
             throw tsm_err("Failed getting file size from fd", 0x6A82);
        }
        
        const std::string &filename() const
        {
            return m_filename;
        }
        
        static bool file_exists(const std::string &filename)
        {
            struct stat buffer;
            auto full_path = base_dir() + filename;
            return (::stat(full_path.c_str(), &buffer) == 0);
        }
    private:
        static std::string base_dir() {
            return os::log_base_dir();
        }
    private:
        std::string m_filename;
        std::FILE *m_fd{nullptr};
    };
}
