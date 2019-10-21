//
//  Transmit_.hpp
//  Wallet
//
//  Created by zhouhuang on 2019/10/21.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <future>
#include <memory>
#include <stdexcept>
#include <string>

#include <bytestream.hpp>

#include <algorithm/cipher/cipher.hpp>
#include <spdlog/spdlog.h>
#include <algorithm/sm/sm2.hpp>

#include "driver/driver.hpp"

/*
 * Transmit 的作用是为中间件提供稳定的指令收发通道，它屏蔽了设备类型和通信协议，对上层仅暴露收发指令的方法
 * 设备的连接断开操作和状态由各平台自理，但在驱动连接上之后和驱动准备断开或者被动断开之前需要调用Transmit的on_connected 和 on_disconnected接口
 * 以供Transmit创建和释放资源，如秘钥协商和清理会话秘钥之类的工作
 */
namespace excelsecu {
template <class Driver, class Configure>
class Authenticate {
public:
    // should be called when driver connected;
    // if auth mode is v5 spp, it just prepare the apdu for next step, and return
    // the comparecode. if auth mode is pair, it need promote the input dialog for
    // paircode, so prepare returns a empty string. it throws exception when error
    // happened
    void authenticate() noexcept(false) {
      bytestream random;
      bytestream auth_apdu = bytestream("803300002FB52D");

      std::string auth_str = Configure::current().get_auth_str();
      std::unique_ptr<bytestream> auth_data(new bytestream(auth_str));

      if (auth_data && auth_data->length() > 0) {
        auto apdu = bytestream("8033000004B4020010");
        auto repo = Driver::instance().send(apdu);

        // 00 || rand(0x10)
        if (repo.length() < 0x11 || repo[0x00] != 0x00) {
          throw tsm_err("prepare authenticate failed, invalid response",
                        ERROR_COMMON_INVALID_DATA);
        }

        random = random.tail(repo.length() - 1);
        auth_apdu.append((uint8_t)0x00);
      }
      else {
        auto apdu = bytestream("8033000043b44101");

        auto sm2_keypair = sm2::gen_keypair();
        apdu += bytestream(sm2_keypair.second);

        auto repo = Driver::instance().send(apdu);

        // 0X000000 || devcert(132) || Enc(sm2pubkey,
        // rand+spkey+sid+comparenumber)(136) || devsign(0x40)
        if (repo.length() < 0x04 + 0x84 + 0x88 + 0x40) {
          throw tsm_err(
              "first time prepare authenticate failed, invalid response",
              ERROR_COMMON_INVALID_DATA);
        }

        auto feature_cipher = repo.split(0x88, 0x88);

        auto feature_plain = sm2::decrypt(feature_cipher, sm2_keypair.first);

        random = feature_plain.split(0, 0x10);
        auth_data.reset(new bytestream(
            feature_plain.split(0x10, 0x18))); // SPKey + SID + compare_number
        auth_apdu.append((uint8_t)0x01);
      }

      auth_apdu += auth_data->split(0x10, 0x04);

      auto rand2 = random.split(0x08, 0x08);
      auto rand2_randinv = rand2 + (~rand2);
      auto skey = cipher::encrypt(cipher::type::tdes112,
                                  auth_data->split(0, 0x10), rand2_randinv);
      auto enc_random =
          cipher::encrypt(cipher::type::tdes112, skey, random.split(0x00, 0x08));
      auth_apdu += enc_random;
      auth_apdu += make_host_name();

      auto repo = Driver::instance().send(auth_apdu);
      if (repo.tail(2) != "9000") {
        throw cos_err(repo.tail(2).hex_str());
      }

      Configure::current().save_auth_str(auth_data->hex_str());
    }
     
    // used in v5 spp only, and should be cleared after authenticate.
      std::string compare_code() const { return m_compare_code; }
private:
    // 截取字符串，使其长度不超过32字节，支持中文UTF8字符截取
      bytestream make_host_name(const std::string &ahost_name) {
        int index = 0x20;

        if (ahost_name.length() == 0) {
          throw tsm_err("hostname should not be empty", ERROR_COMMON_INVALID_PARAM);
        }

        auto host_name =
            bytestream(ahost_name.c_str(), ahost_name.length());
          
        if (host_name.length() > 0x20) { // 防止截取后出现乱码
          index = 0x1F;
          while (index > 0) {
            if ((host_name[index] & 0x80) == 0)
              break;
            if ((host_name[index] & 0xC0) == 0xC0)
              break;
            if ((host_name[index] & 0xC0) == 0)
              index--;
          }
          return host_name.split(0, index);
        }

        return host_name.padding_zero(0x20);
      }
    
private:
    std::string m_compare_code;
};

template <class Driver,
          class Authenticate,
          class Configure>
class HandShake {

public:
    void shake_hand() noexcept(false) {
      bytestream random;

      auto apdu = bytestream("80334B4E4402010008");

      auto sm2_keypair = sm2::gen_keypair();
      apdu += bytestream(sm2_keypair.second);

      auto repo = Driver::instance().send(apdu);

      if (repo.length() < 0x84 + 0x74 + 0x40) {
        throw tsm_err("first time prepare authenticate failed, invalid response",
                      0x6700);
      }

      auto skey_cipher = repo.split(0x84, 0x74);

      auto skey_plain = sm2::decrypt(skey_cipher, sm2_keypair.first);

      auto skey_id = skey_plain.front(4);
      auto skey = skey_plain.tail(0x10);

      auto auth_str = Configure::current().get_auth_str();

      if (auth_str.length() > 0) {
        auto session_key = cipher::decrypt(
            cipher::type::sm4, bytestream(auth_str).front(0x10), skey);
        m_session_context.reset(new SessionContext(skey_id, session_key));
      } else {
        m_session_context.reset(new SessionContext(skey_id, skey));
      }

      SPDLOG_DEBUG("skey_id: %s\n", skey_id.hex_str().c_str());
      SPDLOG_DEBUG("skey_id: %s\n", m_session_context->second.hex_str().c_str());
    }
    
    bytestream decrypt(const bytestream &repo) {
      if (m_session_context) {
        auto &session_key = m_session_context->second;
        // EncResp(u1PaddingNum Rsp sw1sw2 padding)
        auto plain = cipher::decrypt(cipher::type::sm4, session_key,
                                     repo.front(repo.length() - 0x02));

        return plain.split(1, plain.length() - plain.mem()[0] - 1);
      }
      return repo;
    }

    bytestream encrypt(const bytestream &apdu) {
      if (m_session_context) {
        auto &session_key = m_session_context->second;
        bool should_padding = (apdu.length() % 0x10 != 0);

        auto padding_len =
            should_padding ? (0x10 - (apdu.length() % 0x10)) : 0x00;

        auto apdu_padding =
            bytestream(apdu) + bytestream("80") + bytestream(padding_len - 1);

        auto apdu_cipher = cipher::encrypt(cipher::type::sm4, session_key,
                                           should_padding ? apdu_padding : apdu);

        auto apdu_sm = bytestream(3);
        apdu_sm.append((ebyte)padding_len);
        apdu_sm += m_session_context->first;
        apdu_sm += apdu_cipher;

        auto head = bytestream("8033534D");
        if (apdu_sm.length() > 0xFF) {
          head.append((ebyte)0x00);
          head.append((ebyte)(apdu_sm.length() >> 8)); // 2bytes
        }
        head.append((ebyte)(apdu_sm.length() & 0xFF));
        return head + apdu_sm;
      }
      return apdu;
    }
    
private:
    using SessionContext = std::pair<bytestream, bytestream>;

     std::unique_ptr<Authenticate> m_authenticator;
     std::unique_ptr<SessionContext> m_session_context;
};

template <class Driver,
template <typename D, typename A, typename C> class HandShake,
template <typename D, typename C> class Authenticate, class Configure>
class Transmit {
    public:
    // called when driver connected
    // @notice 不会立即进行握手，而是等到发送第一条加密指令时才会真正开始进行握手流程
    // 但会尝试进行认证，每次连接
    void on_connected() {
        if (!m_authenticate) return;
       
        authenticate_result = std::async(Authenticate<Driver, Configure>::authenticate, m_authenticate);
      
    }
    
    // called when driver disconnected
    // @notice clear session if needed
    void on_disconnected();
    
    
    // throw exceptions if error happened
    // @notice 如果开启begin_secure_session, encrypted参数不生效
    // @todo 对于特殊返回指令，如按键确认等的处理
    bytestream send(const bytestream& apdu, bool encrypted = true)
    {
        if (encrypted && !m_handshake) {
            // 握手前，等待认证完成
            try {
                authenticate_result.get();
            } catch(std::exception& e) {
                throw e;
            }
            
            // 如果需要握手则重新握手
            try {
                m_handshake->shake_hand();
            } catch(std::exception& e) {
                throw e;
            }
        }
        
        bytestream resp;

         if (!encrypted)
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
    
    // 当前是否所有指令加密
    bool is_secure_session();
    // 开启所有指令加密
    void begin_secure_session();
    // 关闭所有指令加密
    void end_secure_session();
private:

    HandShake<Driver, Authenticate<Driver, Configure>, Configure> m_handshake;
    Authenticate<Driver, Configure> m_authenticate;
    std::future<void> authenticate_result;
};

using BleDriver = driver<ble_driver>;
//using Authen = Authenticate<BleDriver, configure>;

using Wallet = Transmit<BleDriver, HandShake, Authenticate, configure>;
}
