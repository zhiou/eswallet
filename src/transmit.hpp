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
#include <memory>

#include "eventbus.hpp"
#include "transmit_def.hpp"
#include "bytestream.hpp"
#include "EsError.h"

namespace excelsecu {
    
//  提供线保的数据收发类
    template<class Driver, class Handshake = void>
    class transmit: public event_handler
    {

    public:
        transmit() {
            eventbus::instance().add_handler<event>(this);
            
            Driver::instance();
            
            if (!std::is_void<Handshake>::value) {
                std::unique_ptr<Handshake> ptr(new Handshake());
                m_handshake = std::move(ptr);
            }
        }
        
        ~transmit() {
            eventbus::instance().remove_handler<event>(this);
        }
    public:
        void connect(const std::string &sn)
        {
            Driver::instance().set_target(sn);
            
            event next;
            next.msg_type = event_msg::begin;
            eventbus::instance().post(next);
        }
        
        void disconnect()
        {
            if (!Driver::instance().disconnect()) {
                throw tsm_err("driver disconnect failed", ERROR_COMMON_NO_DEVICE);
            }
        }
        
        bytestream send(const bytestream& apdu, bool security = true) noexcept(false)
        {
            bytestream resp;
            
            if (std::is_void<Handshake>::value || !security)
            {
                resp = Driver::instance().send(apdu);
            }
            else
            {
                auto enc_apdu = m_handshake->encrypt(apdu);
                bytestream enc_repo = Driver::instance().send(enc_apdu);
                resp = m_handshake->decrypt(enc_repo);
            }
            
            auto error_code = resp.tail(2).hex_str();
            if (error_code != "9000") {
                throw cos_err(error_code);
            }
            
            return resp.drop_tail(2);
        }
    public:
        void handle(event &e) override final { // TODO: use state machine to manage states
            event next;
            
            switch (e.msg_type) {
                case event_msg::connected:
                {
                    next.msg_type = Driver::instance().is_ble() ? event_msg::will_authencate : event_msg::will_handshake;
                    break;
                }
                case event_msg::did_authencate:
                {
                    next.msg_type = event_msg::will_handshake;
                    break;
                }
                case event_msg::did_handshake:
                {
                    next.msg_type = event_msg::complete;
                    break;
                }
                case event_msg::disconnected:
                {
                    
                }
                default:
                    return;
            }
            
            eventbus::instance().post(next);
        }
    private:
        std::unique_ptr<Handshake> m_handshake;
    };
    
    
    template<class Driver>
    class transmit<Driver>: public event_handler
    {
        
    public:
        transmit() {
            Driver::instance();
            
            eventbus::instance().add_handler<event>(this);
        }
        
        ~transmit() {
            eventbus::instance().remove_handler<event>(this);
        }
    public:
        
        void connect(const std::string &sn)
        {
            Driver::instance().set_target(sn);
            
            event next;
            next.msg_type = event_msg::begin;
            eventbus::instance().post(next);
        }
        
        void disconnect()
        {
            if (!Driver::instance().disconnect()) {
                throw tsm_err("driver disconnect failed", ERROR_COMMON_NO_DEVICE);
            }
        }
        
        bytestream send(const bytestream& apdu) noexcept(false)
        {
            if (!Driver::instance().is_connected()) {
                throw tsm_err("driver not connected", ERROR_COMMON_NO_DEVICE);
            }
 
            return Driver::instance().send(apdu);
        }
        
    public:
        void handle(event &e) override final { // TODO: use state machine to manage states
            event next;
            
            switch (e.msg_type) {
                case event_msg::connected:
                {
                    next.msg_type =event_msg::complete;
                    break;
                }
                case event_msg::disconnected:
                {
                    
                }
                default:
                    return;
            }
            
            eventbus::instance().post(next);
        }
    };
}

