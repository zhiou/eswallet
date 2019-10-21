//
//  handshake.hpp
//  transmit
//
//  Created by zhiou on 2019/7/1.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <algorithm/cipher/cipher.hpp>
#include <spdlog/spdlog.h>
#include <eventbus.hpp>
#include <algorithm/sm/sm2.hpp>

#include "../transmit_def.hpp"
#include "../athenticate/authenticator.hpp"

namespace excelsecu {

class session_key;
class configure;

template <class Driver, class Authenticator = void, class Configure = configure>
class handshake : public event_handler {
public:
  handshake() {
    eventbus::instance().add_handler<event>(this);

    if (!std::is_void<Authenticator>::value) {
      std::unique_ptr<Authenticator> ptr(new Authenticator());
      m_authenticator = std::move(ptr);
    }
  }

  ~handshake() { eventbus::instance().remove_handler<event>(this); }

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

public:
public:
  void handle(event &e) override final {
    if (e.msg_type != event_msg::will_handshake) {
      return;
    }

    std::thread([this]() {
      event next;
      bool success = true;

      try {
        shake_hand();
      } catch (std::exception &e) {
        success = false;
      }

      next.msg_type = success ? event_msg::did_handshake : event_msg::failed;
      eventbus::instance().post(next);
    })
        .detach();
  }

public:
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

  std::unique_ptr<Authenticator> m_authenticator;
  std::unique_ptr<SessionContext> m_session_context;
};
} // namespace excelsecu
