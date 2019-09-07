//
//  s300.hpp
//  transmit
//
//  Created by zhiou on 2019/8/22.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "json.hpp"
#include "path.hpp"

#include "eslog.hpp"
#include "wallet.hpp"

#include "address.hpp"
#include "error.hpp"

#include "btc.hpp"
#include "fcbuffer.hpp"

namespace excelsecu {

namespace wallet {

using json = nlohmann::json;

template <class Transferable> class s300 {
public:
  s300()
      : all_enc(false), m_cur_app_id(""),
        m_trans(std::make_unique<Transferable>()) {}

public:
  bool connect(const std::string &sn) {

    try {
      m_trans->connect(sn);
    } catch (std::exception &e) {
      return false;
    }
    return true;
  }

  bool disconnect() {

    try {
      m_trans->disconnect();
    } catch (std::exception &e) {
      return false;
    }
    return true;
  }

  json init() {

    select(applet::hdwallet);

    std::string wallet_id = "";
    try {
      auto apdu = bytestream("8070000000");
      auto resp = m_trans->send(apdu);
      wallet_id = "0000" + resp.hex_str();
    } catch (const es_error &e) {
      LOG_ERROR("get wallet id failed." + e.what());
      throw e;
    }

    return {{"walletId", wallet_id}};
  }

  void select(wallet::applet app) {
    auto app_id = get_app_id(app);
    if (app_id == m_cur_app_id) {
      return;
    }

    try {
      auto apdu = bytestream("00A4040008B000000000" + app_id);
      auto resp =
          m_trans->send(apdu); // 如果返回值不为9000，或者发送失败将会抛出异常
      m_cur_app_id = app_id;
    } catch (const es_error &e) {
      LOG_ERROR("select applet failed" + e.what());
      throw e;
    }
  }

  void select(coin coin_type) {
    applet app;
    switch (coin_type) {
    case coin::btc:
      app = applet::btc;
      break;
    case coin::eth:
      app = applet::eth;
      break;
    case coin::eos:
      app = applet::eos;
      break;
    }
    select(app);
  }

  json get_wallet_info() {

    json wi;
    wi["sdk_version"] = wallet::sdk_version;

    try {
      auto applet_versions = get_applet_versions();
      std::string cos_ver = "";
      for (auto &version : applet_versions) {
        std::string ver = version["rawHex"];
        bool installed = version["installed"];
        if (installed) {
          cos_ver += (std::string("_") + ver);
        }
      }
      wi["cos_version"] = cos_ver;
      wi["applet_versions"] = applet_versions;
    } catch (const es_error &e) {
      LOG_ERROR("get wallet info failed" + e.what());
    }

    return wi;
  }

  std::string get_pubkey(coin coin_type, const std::string &path,
                         bool showing = false) {
    std::string flag = showing ? "02" : "00";

    select(coin_type);

    std::string pubkey;
    try {
      auto apdu = bytestream("804600" + flag + "1505") + path::to_buffer(path);
      auto resp = m_trans->send(apdu, false);
      pubkey = resp.hex_str();
    } catch (const es_error &e) {
      LOG_ERROR("get pubkey failed, path = " + path);
    }
    return pubkey;
  }

  void set_net(coin coin_type, const std::string &net_name) {
    switch (coin_type) {
    case coin::btc:
      m_btc_net = net_name;
      break;
    case coin::eth:
      m_eth_net = net_name;
      break;
    case coin::eos:
      m_eos_net = net_name;
      break;
    }
  }

  std::string get_address(coin coin_type, const std::string &path,
                          bool showing = false, bool storing = false) {
    uint8_t flag = 0;

    flag |= (storing ? 0x01 : 0x00);
    flag |= (showing ? 0x02 : 0x00);
    flag |= 0x04;
    flag |= ((coin_type != coin::eth) && 0x08);

    if (coin_type == coin::eos) {
      flag &= 0xFC;
    }

    select(coin_type);

    std::string address;
    try {
      auto apdu = bytestream("804600001505") + path::to_buffer(path);
      apdu[3] = flag;

      auto resp = m_trans->send(apdu);
      address = resp.utf8_str();

      // 如果是BTC测试网，需要增加6F开头
      if (coin_type == coin::btc && m_btc_net == btc_testnet3) {
        auto addr_buf = address::to_buffer(address);
        addr_buf = bytestream("6F") + addr_buf;
        address = address::from_buffer(coin_type, addr_buf);
      }
    } catch (const es_error &e) {
      LOG_ERROR("get address failed, reason = " + e.what());
    }
    return address;
  }

  json get_derive_data(coin coin_type, const std::string &path) {
    if (coin_type != coin::btc) {
      LOG_ERROR("coin type don't support to derive data");
      throw tsm_err("coin type don't support to derive data",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }

    json dd;

    try {
      auto apdu = bytestream("804C00000D03") + path::to_buffer(path);
      auto resp = m_trans->send(apdu);

      dd["publicKey"] = resp.split(0, 33).hex_str();
      dd["chainCode"] = resp.split(33, 32).hex_str();

    } catch (const es_error &e) {
      LOG_ERROR("get derive data failed, reason = " + e.what());
    }
    return dd;
  }

  json get_address(coin coin_type, std::string pubkey, std::string chain_code,
                   std::string addr_type, size_t from_index, size_t to_index) {
    if (coin_type != coin::btc) {
      LOG_ERROR("coin type don't support to get address");
      throw tsm_err("coin type don't support to get address",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }

    bool is_external = (addr_type == "external");
    json network = btc::get_addr_prefix(m_btc_net);
    return btc::derive_address(network["pubKeyHash"], pubkey, chain_code,
                               is_external, from_index, to_index);
  }

  std::string get_account_name(coin coin_type, size_t account_index,
                               std::string pm_data, bool showing = false,
                               bool storing = false) {
    if (coin_type != coin::eos) {
      LOG_ERROR("coin type don't support to get account name");
      throw tsm_err("coin type don't support to get account name",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }

    uint8_t flag = 0;

    flag |= (storing ? 0x01 : 0x00);
    flag |= (showing ? 0x02 : 0x00);

    auto apdu = bytestream("8076000000");

    bool is_key = (pm_data.compare(0, 7, "import_") == 0);

    size_t data_len = 0x00;

    if (is_key) {

      uint32_t account = (uint32_t)(0x80000000 | account_index);
      size_t start_index = pm_data.find("EOS") + 3;

      std::vector<unsigned char> vchRet;
      if (DecodeBase58(pm_data.substr(start_index), vchRet)) {
        auto data = bytestream();
        data.append(account);
        data += bytestream(vchRet).front(vchRet.size() - 4);
        apdu += data;
        data_len = data.length();
      } else {
        throw tsm_err("invalid pm data", ERROR_COMMON_INVALID_DATA);
      }

    } else {
      auto data = path::to_buffer(pm_data);
      apdu += data;
      data_len = data.length();
    }

    apdu[2] = is_key ? 0x01 : 0x00;
    apdu[3] = flag;
    apdu[4] = data_len;

    try {
      auto resp = m_trans->send(apdu);
      return resp.utf8_str();
    } catch (const es_error &e) {
      LOG_ERROR("get account name failed, reason = " + e.what());
    }
    return "";
  }

  void get_default_permission(coin coin_type, size_t account_index) {
    if (coin_type != coin::eos) {
      LOG_ERROR("coin type don't support to get default permission");
      throw tsm_err("coin type don't support to get default permission",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }

    uint32_t account = (uint32_t)(0x80000000 | account_index);

    auto apdu = bytestream("807400000400000000");
    apdu.append(account);

    try {
      m_trans->send(apdu);
    } catch (const es_error &e) {
      LOG_ERROR("get default permission failed, reason = " + e.what());
    }
  }

  json get_permission(coin coin_type, uint32_t account_index) {
    if (coin_type != coin::eos) {
      LOG_ERROR("coin type don't support to get default permission");
      throw tsm_err("coin type don't support to get default permission",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }

    size_t offset = 0;
    std::vector<bytestream> permissions;
    while (true) {
      auto apdu = bytestream("807800000100");
      apdu[5] = offset;
      auto resp = m_trans->send(apdu);
      size_t pm_size = resp[1];
      constexpr size_t pm_data_size = 54;
      for (int i = 0; i < pm_size; ++i) {
        permissions.emplace_back(
            resp.split(2 + i * pm_data_size, pm_data_size));
      }

      size_t remain_pm_size = resp[0];
      if (remain_pm_size == 0) {
        break;
      }
      offset += pm_size;
    }

    std::vector<json> pms;

    for (auto pm : permissions) {
      json pm_j = pase_pmdata(coin_type, account_index, pm);
      if (!pm_j.empty()) {
        pms.push_back(pm_j);
      }
    }
    return pms;
  }

  void add_permission(coin coin_type, const json &pm_info) {
    if (coin_type != coin::eos) {
      LOG_ERROR("coin type don't support to add permission");
      throw tsm_err("coin type don't support to add permission",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }

    try {
      auto actor = fcbuffer::encode_name(pm_info["address"]);
      auto name = fcbuffer::encode_name(pm_info["type"]);
      auto path = path::to_buffer(pm_info["path"]);

      auto apdu = bytestream("8070000000");
      apdu.append(actor);
      apdu.append(name);
      apdu += path;

      auto resp = m_trans->send(apdu);
    } catch (const es_error &e) {
      LOG_ERROR("add permission failed" + e.what());
    }
  }

  void remove_permission(coin coin_type, const json &pm_info) {
    if (coin_type != coin::eos) {
      LOG_ERROR("coin type don't support to add permission");
      throw tsm_err("coin type don't support to add permission",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }

    try {
      auto actor = fcbuffer::encode_name(pm_info["address"]);
      auto name = fcbuffer::encode_name(pm_info["type"]);
      auto path = path::to_buffer(pm_info["path"]);

      auto apdu = bytestream("8072000000");
      apdu.append(actor);
      apdu.append(name);
      apdu += path;

      auto resp = m_trans->send(apdu);
    } catch (const es_error &e) {
      LOG_ERROR("add permission failed" + e.what());
    }
  }

  void import_key(coin coin_type, json key_info) {
    if (coin_type != coin::eos) {
      LOG_ERROR("coin type don't support to add permission");
      throw tsm_err("coin type don't support to add permission",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }
    try {
      select(coin_type);

      uint8_t index = key_info["accountIndex"];
      uint32_t account_index = 0x80000000 | index;
      auto prikey_buf = address::parse_eos_private_key(key_info["key"]);
      auto name_encoded = fcbuffer::encode_name(key_info["address"]);
      auto type_encoded = fcbuffer::encode_name(key_info["type"]);

      auto apdu = bytestream("8070010000");
      auto payload = bytestream();
      payload.append(name_encoded);
      payload.append(type_encoded);
      payload.append(account_index);
      payload += prikey_buf;
      apdu += payload;

      apdu[4] = payload.length();
      m_trans->send(apdu);
    } catch (const es_error &e) {
      LOG_ERROR("import key failed" + e.what());
    }
  }

  void remove_key(coin coin_type, json key_info) {
    if (coin_type != coin::eos) {
      LOG_ERROR("coin type don't support to add permission");
      throw tsm_err("coin type don't support to add permission",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }

    try {
      select(coin_type);

      uint8_t index = key_info["accountIndex"];
      uint32_t account_index = 0x80000000 | index;
      auto prikey_buf = address::parse_eos_private_key(key_info["key"]);
      auto name_encoded = fcbuffer::encode_name(key_info["address"]);
      auto type_encoded = fcbuffer::encode_name(key_info["type"]);

      auto apdu = bytestream("8072010000");
      auto payload = bytestream();
      payload.append(name_encoded);
      payload.append(type_encoded);
      payload.append(account_index);
      payload += prikey_buf;
      apdu += payload;

      apdu[4] = payload.length();
      m_trans->send(apdu);
    } catch (const es_error &e) {
      LOG_ERROR("remove key failed" + e.what());
    }
  }

  void set_amount_limit(coin coin_type, uint64_t amount_limit) {
    if (coin_type != coin::eos) {
      LOG_ERROR("coin type don't support to add permission");
      throw tsm_err("coin type don't support to add permission",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }
    try {
      auto apdu = bytestream("807A000000");
      apdu.append(amount_limit);
      apdu[4] = 0x08;

      auto resp = m_trans->send(apdu);
    } catch (const es_error &e) {
      LOG_ERROR("set amount limit failed" + e.what());
    }
  }

  void add_token(coin coin_type, json token) {
    if (coin_type != coin::eth) {
      LOG_ERROR("coin type don't support to add token");
      throw tsm_err("coin type don't support to add token",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }

    try {
      // 8070 0000 len nameLength[1] name[nameLength] decimals[1] address[20]
      std::string token_name = token["name"];
      size_t token_name_len = token_name.length();
      uint8_t decimals = token["decimals"];
      std::string address = token["address"];

      auto apdu = bytestream("80700000");
      apdu[4] = token_name_len + 23;
      apdu[5] = token_name_len;

      apdu += bytestream(token_name.c_str(), token_name_len);
      apdu.append(decimals);

      apdu += address::to_buffer(address);

      m_trans->send(apdu);
    } catch (const es_error &e) {
      LOG_ERROR("add token failed" + e.what());
    }
  }

  void remove_token(coin coin_type, json token) {
    if (coin_type != coin::eth) {
      LOG_ERROR("coin type don't support to add token");
      throw tsm_err("coin type don't support to add token",
                    ERROR_COMMON_FUNCTION_NOT_SUPPORT);
    }
    try {
      // 8072 0000 len address[20]
      std::string address = token["address"];
      auto apdu = bytestream("8072000020") + address::to_buffer(address);
      m_trans->send(apdu);
    } catch (const es_error &e) {
      LOG_ERROR("add token failed" + e.what());
    }
  }

  json sign_transaction(coin coin_type, json tx) {

    auto build_sign = [&](const bytestream &path, const bytestream &change_path,
                          const bytestream &msg) {
      auto data_length =
          2 + path.length() + 2 + change_path.length() + 3 + msg.length();

      auto payload = bytestream(data_length);

      int index = 0;
      payload[index++] = 0xC0;
      payload[index++] = path.length() / 4;
      payload += path;
      index += path.length();

      payload[index++] = 0xC1;
      payload[index++] = change_path.length() / 4;
      payload += change_path;
      index += change_path.length();

      payload[index++] = 0xC2;
      payload[index++] = msg.length() >> 8;
      payload[index++] = msg.length();
      payload += msg;
      index += change_path.length();

      return payload;
    };

    auto send_sign = [&](const bytestream &data, bool is_compressed) {
      uint8_t compress_change = 0x08;

      if (data.length() <= 0xFF) {
        auto apdu_head = bytestream("8048030000");
        if (is_compressed)
          apdu_head[3] |= compress_change;

        apdu_head[4] = data.length();
        auto apdu = apdu_head + data;
        return m_trans->send(apdu);
      } else {
        auto remain_len = data.length();

        while (true) {
          if (remain_len <= 0xFF) {
            auto apdu_head = bytestream("8048020000");
            apdu_head[3] |= compress_change;
            apdu_head[4] = remain_len;
            auto offset = data.length() - remain_len;

            auto apdu = apdu_head + data.split(offset, remain_len);
            return m_trans->send(apdu);
          } else if (remain_len == data.length()) {
            auto apdu_head = bytestream("80480100FF");
            auto apdu = apdu_head + data.split(0, 0xFF);
            m_trans->send(apdu);
          } else {
            auto apdu_head = bytestream("80480000FF");
            apdu_head[3] |= compress_change;
            auto offset = data.length() - remain_len;

            auto apdu = apdu_head + data.split(offset, 0xFF);
            m_trans->send(apdu);
          }
          remain_len -= 0xFF;
        }
      }
    };

    auto parse_sign_resp = [&](coin coin_type, const bytestream &resp) {
      bool is_eos = (coin_type == coin::eos);
      int offset = is_eos ? 1 : 0;
      int remain = is_eos ? resp[0] : 0;

      auto r = resp.split(offset, 0x20);
      auto s = resp.split(0x20 + offset, 0x20);
      auto pubkey = resp.split(0x40 + offset, 0x40);
      auto v = resp[0x80 + offset];
      return std::make_tuple(remain, v, r, s, pubkey);
    };

    return json();
  }

private:
  json pase_pmdata(coin coin_type, uint32_t account_index,
                   const bytestream &pm) {
    uint8_t type = pm[0];

    std::string data;
    if (type == 0) { // type[1] actor[8] name[8] path[20]
      uint32_t pm_account_index = pm.readUInt32(1 + 8 + 8 + 4 * 2);
      if (pm_account_index != account_index) {
        return json();
      }
      auto path = pm.split(17, 20);
      data = path::from_buffer(data);
    } else if (type == 1) { // type[1] actor[8] name[8] account[4] publicKey[33]
      uint32_t pm_account_index = pm.readUInt32(1 + 8 + 8);
      if (pm_account_index != account_index) {
        return json();
      }
      auto pubkey = pm.split(21, 33);
      data = address::from_buffer(coin_type, pubkey);
    } else {
      LOG_WARN("getPermissions unknown type");
    }
    auto actor_name = fcbuffer::decode_name(pm.split(1, 8).hex_str());
    auto name = fcbuffer::decode_name(pm.split(9, 8).hex_str());
    return {{"type", (int)type},
            {"actor", actor_name},
            {"name", name},
            {"data", data}};
  }

private:
  // get applet id
  std::string get_app_id(wallet::applet applet) {
    switch (applet) {
    case wallet::applet::hdwallet:
      return "010102";
    case wallet::applet::manager:
      return "010202";
    case wallet::applet::backup:
      return "010302";
    case wallet::applet::btc:
      return "020002";
    case wallet::applet::eth:
      return "023C02";
    case wallet::applet::eos:
      return "02C202";
    default:
      LOG_WARN("unkown applet type for app ID");
      return "";
    }
  }

  json get_version_info(const std::string name, applet app) {
    select(app);

    json vi;
    vi["name"] = name;
    vi["installed"] = false;

    try {
      auto apdu = bytestream("804A000000");
      auto resp = m_trans->send(apdu, false);

      auto version = resp.split(4, 3).hex_str();

      vi["installed"] = true;
      vi["rawHex"] = resp.hex_str();
      vi["appletId"] = resp.split(0, 3).hex_str();
      vi["packageId"] = resp.split(0, 3).hex_str() + "01";
      vi["isTestApplet"] = resp[3] == 1;
      vi["version"] = version;
      vi["date"] = resp.split(7, 4).hex_str();
      vi["app_type"] = app;

    } catch (const es_error &e) {
      LOG_ERROR(name + "get version failed");
    }
    return vi;
  }

  std::vector<json> get_applet_versions() {
    std::vector<std::string> app_names{"HDWallet", "Manager", "Backup",
                                       "BTC",      "ETH",     "EOS"};
    std::vector<applet> applet_types{applet::hdwallet, applet::manager,
                                     applet::backup,   applet::btc,
                                     applet::eth,      applet::eos};
    std::vector<json> versions;

    for (int i = 0; i < app_names.size(); ++i) {
      auto version = get_version_info(app_names[i], applet_types[i]);
      versions.emplace_back(version);
    }
    return versions;
  }

private:
  std::unique_ptr<Transferable> m_trans;
  std::string m_cur_app_id;
  bool all_enc;

  std::string m_btc_net;
  std::string m_eth_net;
  std::string m_eos_net;
};
} // namespace wallet
} // namespace excelsecu
