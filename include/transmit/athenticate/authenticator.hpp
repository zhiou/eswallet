//
//  authenticator.h
//  transmit
//
//  Created by zhiou on 2019/7/1.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <algorithm/cipher/cipher.hpp>
#include <eventbus.hpp>
#include <algorithm/sm/sm2.hpp>
#include <string>

#include "../transmit_def.hpp"

namespace excelsecu {

class configure;

template <class Driver, class Configure>
class authenticator : public event_handler {
public:
  authenticator() { eventbus::instance().add_handler<event>(this); }

  ~authenticator() { eventbus::instance().remove_handler<event>(this); }

public:
  // used in v5 spp only, and should be cleared after authenticate.
  std::string compare_code() const { return m_compare_code; }

  bytestream make_host_name() {
    int index = 0x20;

    auto config_hostname = Configure::current().host_name();
    if (config_hostname.length() == 0) {
      throw tsm_err("hostname should not be empty", ERROR_COMMON_INVALID_PARAM);
    }

    auto host_name =
        bytestream(config_hostname.c_str(), config_hostname.length());
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
    } else {
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

public:
  void handle(event &e) override final {
    if (e.msg_type != event_msg::will_authencate) {
      return;
    }
    std::thread([this]() {
      event next;
      bool success = true;

      try {
        authenticate();
      } catch (std::exception &e) {
        success = false;
      }

      next.msg_type = success ? event_msg::did_authencate : event_msg::failed;
      eventbus::instance().post(next);
    })
        .detach();
  }

private:
  std::string m_compare_code;
};

} // namespace excelsecu
