//
//  driver_mocker.hpp
//  transmit
//
//  Created by zhiou on 2019/8/12.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include "bytestream.hpp"
#include "cipher.hpp"
#include "eventbus.hpp"
#include "sm2.hpp"
#include "transmit_def.hpp"
#include <future>
#include <stdexcept>
#include <string>

namespace excelsecu {

class driver_mocker : public event_handler {
public:
  bool is_ble() const { return true; }

  bool is_mock() const { return true; }

public:
  static driver_mocker &instance() {
    std::call_once(m_once_flag, driver_mocker::create_instance);
    return *driver_mocker::m_instance;
  }

public:
  std::string target_serialnumber() const { return m_serialnumber; }

  void set_target(const std::string &serial_number) {
    if (serial_number == m_serialnumber)
      return;
    m_serialnumber = serial_number;
  }

  bool is_connected() const { return true; }

  bytestream send(const bytestream &apdu) {
    // TODO: 各种指令
    bytestream repo;
    if (apdu.mem()[0x01] == 0x84) // 取随机数
    {
      repo = bytestream(apdu.mem()[0x04], true);
    } else if (apdu.startWith("8033000004B4020010")) // 非首次认证
    {
      repo = bytestream("\x00", 0x01);
      repo += bytestream(0x10, true);
    } else if (apdu.startWith("8033000043b44101")) // 首次认证
    {
      auto pubkey = apdu.tail(0x40);

      bytestream rand("11223344556677881122334455667788");
      bytestream spkey("88776655443322118877665544332211");
      bytestream sid("09876543");
      bytestream compare_number("31323334");

      auto encrypted_data =
          sm2::encrypt(rand + spkey + sid + compare_number, pubkey);

      bytestream four_byte(0x04);
      bytestream dev_cert(132);
      bytestream dev_sign(0x40);

      repo = four_byte + dev_cert + encrypted_data + dev_sign;
    } else if (apdu.startWith("803300002FB52D")) //认证
    {
      bytestream random("11223344556677881122334455667788");
      bytestream spkey("88776655443322118877665544332211");
      auto rand2 = random.split(0x08, 0x08);
      auto rand2_randinv = rand2 + (~rand2);
      auto skey = cipher::encrypt(cipher::type::tdes112, spkey, rand2_randinv);

      auto enc_rand = apdu.split(12, 8);

      auto rand = cipher::decrypt(cipher::type::tdes112, skey, enc_rand);

      if (random == rand)
        return bytestream("6D00");

    } else if (apdu.startWith("80334B4E4402010008")) // handshake
    {
      bytestream pubkey = apdu.tail(0x40);
      bytestream dev_cert(0x88);
      bytestream skeyid(0x04, true);
      bytestream skey(0x10, true);
      bytestream sign(0x40);
      auto enc_skey = sm2::encrypt(skeyid + skey, pubkey);
      repo = dev_cert + enc_skey + sign;
    } else if (apdu.startWith("00A4040008B000000000")) {
      m_selected_app = apdu.tail(3).hex_str();
    } else if (apdu.startWith("804A000000")) {
      if (m_selected_app == "010102") { // hdwallet
        repo = bytestream("0101020101070420190621");
      } else if (m_selected_app == "010202") { // manager
        repo = bytestream("0102020101041620190820");
      } else if (m_selected_app == "010302") { // backup
        repo = bytestream("0103020101020120190517");
      } else if (m_selected_app == "020002") { // btc
        repo = bytestream("0200020101060720190807");
      } else if (m_selected_app == "023C02") { // eth
        repo = bytestream("023C020101041020190806");
      } else if (m_selected_app == "02C202") { // eos
        repo = bytestream("02C2020101091520190815");
      }
    }
    else if (apdu.startWith("804C00000D03")) {
      repo = bytestream(
          "01010203040506070801020304050607080102030405060708010203040506070801"
          "02030405060708010203040506070801020304050607080102030405060708");
    }
    else if (apdu.startWith("80760000")) {
      std::string account_name = "eos_account";
      repo = bytestream(account_name.c_str(), account_name.length());
    }
    else if (apdu.startWith("8048")) {
        repo = bytestream("15D95B794F5D0E7F71333BA3922AD0B91D3048348558690E952C81520AF53C645409F5C2CBF2F059E47B852D82533CF902DB7643E3669FFB000C135951AFD8019B0C812BBC931128D63DB3EDE20758E53857E9AC6E10176916135B82F60C2EA2068E40145F8F7C7B78A964684B8755C221896415F5251D163E2C9030D033B334FF");
    }
    return repo += bytestream("9000");
  }

  bool disconnect() {
    event next;
    next.msg_type = event_msg::disconnected;
    eventbus::instance().post(next);
    return true;
  }

public:
  void handle(event &e) override final {
    if (e.msg_type != event_msg::begin)
      return;

    event next;
    next.msg_type = event_msg::connected;
    eventbus::instance().post(next);
  }

private:
  // singleton
  driver_mocker(const driver_mocker &);
  driver_mocker &operator=(const driver_mocker &);

  driver_mocker() { eventbus::instance().add_handler<event>(this); }

  ~driver_mocker() { eventbus::instance().remove_handler<event>(this); }

  static void create_instance() {
    static driver_mocker instance;
    m_instance = &instance;
  }

private:
  std::string m_serialnumber;
  std::string m_selected_app;

private:
  // singleton
  static std::once_flag m_once_flag;
  static driver_mocker *m_instance;
};

std::once_flag driver_mocker::m_once_flag;
driver_mocker *driver_mocker::m_instance = nullptr;
} // namespace excelsecu
