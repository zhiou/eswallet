//
//  transmit.hpp
//  transmit
//
//  Created by zhiou on 2019/7/1.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <string>
#include <future>
#include <stdexcept>
#include "bytestream.hpp"
#include "eventbus.hpp"

namespace excelsecu {
    
    template<typename T>
    struct is_ble_driver : public std::false_type{};
    
    template<>
    struct is_ble_driver<ble_driver>: public std::true_type{};

    enum class driver_type {
        ble,
        audio
    };
    
    static inline unsigned int state_callback(unsigned int, unsigned int state)
    {
        event next;
        next.msg_type = (state == 1) ? event_msg::connected : event_msg::disconnected;
        eventbus::instance().post(next);

        return 0;
    }
    
    template<class DriverType>
    class driver: public event_handler {
    public:
        bool is_ble() const {
            return is_ble_driver<DriverType>::value;
        }
        
        bool is_mock() const {
            return false;
        }
    public:
        static driver& instance() {
            std::call_once(m_once_flag, driver::create_instance);
            return *driver::m_instance;
        }
    public:
        
        std::string target_serialnumber() const {
            return m_serialnumber;
        }
        
        void set_target(const std::string &serial_number) {
            if (serial_number == m_serialnumber) return;
            m_serialnumber = serial_number;
        }
        
        bytestream send(const bytestream& apdu) {
            unsigned char recv[0x2000];
            unsigned int  recvLen = sizeof(recv);

            LOG_DEBUG("apdu: %s\n", apdu.hex_str());
            
            bool success = DriverType::send((ebyte*)apdu.bytes(), apdu.length(), recv, &recvLen);
            if (!success) { throw tsm_err("driver send apdu failed", ERROR_COMMON_NO_DATA_RECIEVED); }
            
            auto repo = bytestream(recv, recvLen);
            LOG_DEBUG("resp: %s\n", repo.hex_str());

            return repo;
        }
    public:
        void handle(event& e) override final {
            if (e.msg_type != event_msg::begin) return;
            
            std::thread([&]() {
                event next;
                if (!DriverType::connect(m_serialnumber)) { // em_driver_connected should be post by callback
                    next.msg_type = event_msg::failed;
                    eventbus::instance().post(next);
                }
            }).detach();
        }
    private:
        // singleton
        driver(const driver&);
        driver& operator=(const driver&);
        
        driver() {
            eventbus::instance().add_handler<event>(this);
            DriverType::set_callback(&state_callback);
        }
        
        ~driver() {
            eventbus::instance().remove_handler<event>(this);
            DriverType::set_callback(NULL);
        }
        
        static void create_instance() {
            static driver instance;
            m_instance = &instance;
        }
    private:
        std::string m_serialnumber;
        
    private:
        //singleton
        static std::once_flag m_once_flag;
        static driver* m_instance;
    };
    
    template<class DriverType> std::once_flag driver<DriverType>::m_once_flag;
    template<class DriverType> driver<DriverType>* driver<DriverType>::m_instance = nullptr;
}
