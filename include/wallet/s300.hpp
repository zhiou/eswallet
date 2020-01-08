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
#include <sstream>

#include <error.hpp>
#include <json/json.hpp>
#include <algorithm/hash/hash.hpp>
#include <spdlog/spdlog.h>
#include "path/path.hpp"
#include <wallet/wallet.hpp>

#include "address/address.hpp"

#include "btc/btc.hpp"
#include "eth/eth.hpp"
#include "eos/eos.hpp"
#include "fcbuffer/fcbuffer.hpp"

#include "eth/rlp/rlp.hpp"
#include "ITransmit.h"

namespace excelsecu {

namespace wallet {

using json = nlohmann::json;


class s300 {
public:
    s300()
    : all_enc(false), m_cur_app_id(""){}
    
public:
    
    json init() {
        select(applet::hdwallet);
        
        std::string wallet_id = "";
        try {
            auto apdu = bytestream("8070000000");
            auto resp = send(apdu);
            wallet_id = "0000" + resp.hex_str();
        } catch (const es_error &e) {
            SPDLOG_ERROR("get wallet id failed. %s", e.what());
            throw e;
        }
        
        return {{"walletId", wallet_id}};
    }
    
    void select(wallet::applet app) {
        //      try {
        //          auto apdu = bytestream("0084000008");
        //          auto resp = send(apdu, true);
        //      } catch (const es_error &e) {
        //          SPDLOG_ERROR("get random failed %s", e.what());
        //      }
        auto app_id = get_app_id(app);
        if (app_id == m_cur_app_id) {
            return;
        }
        
        try {
            auto apdu = bytestream("00A4040008B000000000" + app_id);
            auto resp =
            send(apdu); // 如果返回值不为9000，或者发送失败将会抛出异常
            m_cur_app_id = app_id;
        } catch (const es_error &e) {
            SPDLOG_ERROR("select applet failed %s", e.what());
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
            SPDLOG_ERROR("get wallet info failed %s", e.what());
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
            auto resp = send(apdu, false);
            pubkey = resp.hex_str();
        } catch (const es_error &e) {
            SPDLOG_ERROR("get pubkey failed, path = " + path);
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
            
            auto resp = send(apdu);
            address = resp.utf8_str();
            
            // 如果是BTC测试网，需要增加6F开头
            if (coin_type == coin::btc && m_btc_net == btc_testnet3) {
                auto addr_buf = address::to_buffer(address);
                addr_buf = bytestream("6F") + addr_buf;
                address = address::from_buffer(coin_type, addr_buf);
            }
        } catch (const es_error &e) {
            SPDLOG_ERROR("get address failed, reason %s ", e.what());
        }
        return address;
    }
    
    json get_derive_data(coin coin_type, const std::string &path) {
        if (coin_type != coin::btc) {
            SPDLOG_ERROR("coin type don't support to derive data");
            throw tsm_err("coin type don't support to derive data",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        
        json dd;
        
        try {
            auto apdu = bytestream("804C00000D03") + path::to_buffer(path);
            auto resp = send(apdu);
            
            dd["publicKey"] = resp.split(0, 33).hex_str();
            dd["chainCode"] = resp.split(33, 32).hex_str();
            
        } catch (const es_error &e) {
            SPDLOG_ERROR("get derive data failed, reason %s ", e.what());
        }
        return dd;
    }
    
    json get_address(coin coin_type, std::string pubkey, std::string chain_code,
                     std::string addr_type, size_t from_index, size_t to_index) {
        if (coin_type != coin::btc) {
            SPDLOG_ERROR("coin type don't support to get address");
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
            SPDLOG_ERROR("coin type don't support to get account name");
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
            auto resp = send(apdu);
            return resp.utf8_str();
        } catch (const es_error &e) {
            SPDLOG_ERROR("get account name failed, reason %s ", e.what());
        }
        return "";
    }
    
    void get_default_permission(coin coin_type, size_t account_index) {
        if (coin_type != coin::eos) {
            SPDLOG_ERROR("coin type don't support to get default permission");
            throw tsm_err("coin type don't support to get default permission",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        
        uint32_t account = (uint32_t)(0x80000000 | account_index);
        
        auto apdu = bytestream("807400000400000000");
        apdu.append(account);
        
        try {
            send(apdu);
        } catch (const es_error &e) {
            SPDLOG_ERROR("get default permission failed, reason %s ", e.what());
        }
    }
    
    json get_permission(coin coin_type, uint32_t account_index) {
        if (coin_type != coin::eos) {
            SPDLOG_ERROR("coin type don't support to get default permission");
            throw tsm_err("coin type don't support to get default permission",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        
        size_t offset = 0;
        std::vector<bytestream> permissions;
        while (true) {
            auto apdu = bytestream("807800000100");
            apdu[5] = offset;
            auto resp = send(apdu);
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
            SPDLOG_ERROR("coin type don't support to add permission");
            throw tsm_err("coin type don't support to add permission",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        
        try {
            
            auto actor = fcbuffer::encode_name(pm_info["address"]);
            auto actorBytes = bytestream().append(actor).little_ending();
            auto name = fcbuffer::encode_name(pm_info["type"]);
            auto nameBytes = bytestream().append(name).little_ending();
            auto path = path::to_buffer(pm_info["path"]);
            
            auto apdu = bytestream("8070000000");
            apdu += actorBytes;
            apdu += nameBytes;
            apdu += path;
            apdu[0x04] = apdu.length() - 0x05;
            auto resp = send(apdu);
        } catch (const es_error &e) {
            SPDLOG_ERROR("add permission failed %s ", e.what());
        }
    }
    
    void remove_permission(coin coin_type, const json &pm_info) {
        if (coin_type != coin::eos) {
            SPDLOG_ERROR("coin type don't support to add permission");
            throw tsm_err("coin type don't support to add permission",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        
        try {
            auto actor = fcbuffer::encode_name(pm_info["address"]);
            auto actorBytes = bytestream().append(actor).little_ending();
            auto name = fcbuffer::encode_name(pm_info["type"]);
            auto nameBytes = bytestream().append(name).little_ending();
            auto path = path::to_buffer(pm_info["path"]);
            
            auto apdu = bytestream("8072000000");
            apdu += actorBytes;
            apdu += nameBytes;
            apdu += path;
            apdu[0x04] = apdu.length() - 0x05;
            
            auto resp = send(apdu);
        } catch (const es_error &e) {
            SPDLOG_ERROR("add permission failed %s ", e.what());
        }
    }
    
    void import_key(coin coin_type, json key_info) {
        if (coin_type != coin::eos) {
            SPDLOG_ERROR("coin type don't support to add permission");
            throw tsm_err("coin type don't support to add permission",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        try {
            select(coin_type);
            
            uint8_t index = key_info["accountIndex"];
            uint32_t account_index = 0x80000000 | index;
            auto prikey_buf = address::parse_eos_private_key(key_info["key"]);
            auto name_encoded = fcbuffer::encode_name(key_info["address"]);
            auto name_encoded_bytes = bytestream().append(name_encoded).little_ending();
            auto type_encoded = fcbuffer::encode_name(key_info["type"]);
            auto type_encoded_bytes = bytestream().append(type_encoded).little_ending();
            
            auto apdu = bytestream("8070010000");
            auto payload = bytestream();
            payload += (name_encoded_bytes);
            payload += (type_encoded_bytes);
            payload.append(account_index);
            payload += prikey_buf;
            apdu += payload;
            
            apdu[4] = payload.length();
            send(apdu);
        } catch (const es_error &e) {
            SPDLOG_ERROR("import key failed %s ", e.what());
        }
    }
    
    void remove_key(coin coin_type, json key_info) {
        if (coin_type != coin::eos) {
            SPDLOG_ERROR("coin type don't support to add permission");
            throw tsm_err("coin type don't support to add permission",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        
        try {
            select(coin_type);
            
            uint8_t index = key_info["accountIndex"];
            uint32_t account_index = 0x80000000 | index;
            auto pubkey_buf = address::to_buffer(key_info["publicKey"]);
            auto name_encoded = fcbuffer::encode_name(key_info["address"]);
            auto name_encoded_bytes = bytestream().append(name_encoded).little_ending();
            auto type_encoded = fcbuffer::encode_name(key_info["type"]);
            auto type_encoded_bytes = bytestream().append(type_encoded).little_ending();
            
            auto apdu = bytestream("8072010000");
            auto payload = bytestream();
            payload += (name_encoded_bytes);
            payload += (type_encoded_bytes);
            payload.append(account_index);
            payload += pubkey_buf;
            apdu += payload;
            
            apdu[4] = payload.length();
            send(apdu);
        } catch (const es_error &e) {
            SPDLOG_ERROR("remove key failed %s ", e.what());
        }
    }
    
    void set_amount_limit(coin coin_type, uint64_t amount_limit) {
        if (coin_type != coin::eos) {
            SPDLOG_ERROR("coin type don't support to add permission");
            throw tsm_err("coin type don't support to add permission",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        try {
            auto apdu = bytestream("807A000000");
            auto limit = bytestream().append(amount_limit).reverse();
            apdu += limit;
            apdu[4] = 0x08;
            
            auto resp = send(apdu);
        } catch (const es_error &e) {
            SPDLOG_ERROR("set amount limit failed %s ", e.what());
        }
    }
    
    void add_token(coin coin_type, json token) {
        if (coin_type != coin::eth) {
            SPDLOG_ERROR("coin type don't support to add token");
            throw tsm_err("coin type don't support to add token",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        
        try {
            // 8070 0000 len nameLength[1] name[nameLength] decimals[1] address[20]
            std::string token_name = token["name"];
            size_t token_name_len = token_name.length();
            uint8_t decimals = token["decimals"];
            std::string address = token["address"];
            
            auto apdu = bytestream("807000000000");
            apdu[4] = token_name_len + 22;
            apdu[5] = token_name_len;
            
            apdu += bytestream(token_name.c_str(), token_name_len);
            apdu.append(decimals);
            
            apdu += address::to_buffer(address);
            
            send(apdu);
        } catch (const es_error &e) {
            SPDLOG_ERROR("add token failed %s ", e.what());
        }
    }
    
    void remove_token(coin coin_type, json token) {
        if (coin_type != coin::eth) {
            SPDLOG_ERROR("coin type don't support to add token");
            throw tsm_err("coin type don't support to add token",
                          ERROR_COMMON_FUNCTION_NOT_SUPPORT);
        }
        try {
            // 8072 0000 len address[20]
            std::string address = token["address"];
            auto apdu = bytestream("8072000014") + address::to_buffer(address);
            send(apdu);
        } catch (const es_error &e) {
            SPDLOG_ERROR("add token failed %s ", e.what());
        }
    }
    
    json sign_transaction(coin coin_type, json tx) {
        
        auto build_sign = [&](const bytestream &path,
                              const bytestream &change_path,
                              const bytestream &msg)
        {
            
            auto payload = bytestream();
            payload.append((uint8_t)0xC0);
            payload.append((uint8_t)(path.length()/4));
            payload += path;
            
            payload.append((uint8_t)0xC1);
            payload.append((uint8_t)(change_path.length()/4));
            payload += change_path;
            
            payload.append((uint8_t)0xC2);
            payload.append((uint8_t)(msg.length() >> 8));
            payload.append((uint8_t)msg.length());
            payload += msg;
            return payload;
        };
        
        auto send_sign = [&](const bytestream &data, bool is_compressed)
        {
            uint8_t compress_change = 0x08;
            
            if (data.length() <= 0xFF)
            {
                auto apdu_head = bytestream("8048030000");
                if (is_compressed)
                    apdu_head[3] |= compress_change;
                
                apdu_head[4] = data.length();
                auto apdu = apdu_head + data;
                return send(apdu);
            }
            else
            {
                auto remain_len = data.length();
                
                while (true) {
                    if (remain_len <= 0xFF) {
                        auto apdu_head = bytestream("8048020000");
                        apdu_head[3] |= compress_change;
                        apdu_head[4] = remain_len;
                        auto offset = data.length() - remain_len;
                        
                        auto apdu = apdu_head + data.split(offset, remain_len);
                        return send(apdu);
                    } else if (remain_len == data.length()) {
                        auto apdu_head = bytestream("80480100FF");
                        auto apdu = apdu_head + data.split(0, 0xFF);
                        send(apdu);
                    } else {
                        auto apdu_head = bytestream("80480000FF");
                        apdu_head[3] |= compress_change;
                        auto offset = data.length() - remain_len;
                        
                        auto apdu = apdu_head + data.split(offset, 0xFF);
                        send(apdu);
                    }
                    remain_len -= 0xFF;
                }
            }
        };
        
        auto parse_sign_resp = [&](coin coin_type, const bytestream &resp)
        {
            bool is_eos = (coin_type == coin::eos);
            int offset = is_eos ? 1 : 0;
            int remain = is_eos ? resp[0] : 0;
            
            auto r = resp.split(offset, 0x20);
            auto s = resp.split(0x20 + offset, 0x20);
            auto pubkey = resp.split(0x40 + offset, 0x40);
            auto v = resp[0x80 + offset];
            return std::make_tuple(remain, v, r, s, pubkey);
        };
        
        auto sign_btc = [&](coin coin_type, json tx)
        {
            auto make_basic_script = [&](json tx)
            {
                std::vector<json> inputs;
                for (auto input : tx["inputs"])
                {
                    inputs.emplace_back(
                                        json{
                        {"hash", input["txId"]},
                        {"index", input["index"]},
                        {"scriptSig", input["script"]},
                        {"sequence", 0xFFFFFFFD}
                    }
                                        );
                }
                
                std::vector<json> outputs;
                for (auto output : tx["outputs"])
                {
                    auto script_pubkey =
                    address::make_output_script(coin_type, output["address"]);
                    try {
                        //                auto newout = json;
                        outputs.emplace_back(json{
                            {"amount", output["value"]},
                            {"scriptPubKey", script_pubkey}
                        });
                    }
                    catch(std::exception & e) {
                        std::cout << e.what() << std::endl;
                    }
                }
                return json{{"version", 1},
                    {"inputs", inputs},
                    {"outputs", outputs},
                    {"lockTime", 0}};
            };
            
            auto make_presign_script = [&](int i, json& bs)
            {
                
                bytestream tx;
                tx += bytestream("01000000");
                
                
                // inputs serializer
                std::vector<json> inputs = bs["inputs"];
                tx.append((unsigned char) inputs.size());
                for (int j = 0; j < inputs.size(); j++) {
                    if (j != i) inputs[j]["scriptSig"] = "";
                }
                
                for (auto input: inputs) {
                    auto hash = input["hash"].get<std::string>();
                    tx += bytestream(hash).little_ending(); // 需要改为小头序
                    auto idx = bytestream();
                    idx.append((unsigned int)input["index"]);
                    tx += idx.little_ending();
                    std::string script = input["scriptSig"];
                    tx.append((unsigned char)(script.length()/2));
                    tx += bytestream(script);  // 签名前，script置为空
                    auto seq = bytestream();
                    seq.append((unsigned int)input["sequence"]);
                    tx += seq.little_ending();
                }
                //output serializer
                std::vector<json> outputs = bs["outputs"];
                tx.append((unsigned char) outputs.size());
                for (auto output: outputs) {
                    uint64_t amount = output["amount"];
                    auto amountHex = bytestream();
                    amountHex.append((uint64_t) amount).little_ending();
                    tx += amountHex;
                    
                    std::string out_script = output["scriptPubKey"];
                    auto script_hex = bytestream(out_script);
                    tx.append((unsigned char)script_hex.length());
                    tx += script_hex;
                }
                tx.append((unsigned int)bs["lockTime"]);
                tx.append((unsigned int)0x01000000); //transaction type
                return tx;
            };
            
            auto make_script_sign = [&](const bytestream &r,
                                        const bytestream &s,
                                        const bytestream &pubkey)
            {
                
                bool upperR = (r[0] >= 0x80);
                
                auto script_sign = bytestream();
                
                size_t sign_len = 0x22 + 0x22 + (upperR ? 0x01 : 0x00);
                
                script_sign.append((uint8_t)(0x03 + sign_len));
                script_sign.append((uint8_t)0x30);
                script_sign.append((uint8_t)sign_len);
                
                script_sign.append((uint8_t)0x02);
                script_sign.append((uint8_t)(upperR ? 0x21 : 0x20));
                if (upperR)
                    script_sign.append((uint8_t)0x00);
                script_sign += r;
                
                script_sign.append((uint8_t)0x02);
                script_sign.append((uint8_t)0x20);
                script_sign += s;
                // hash type
                script_sign.append((uint8_t)0x01);
                // pubkey, compress type
                script_sign.append((uint8_t)0x21);
                script_sign.append((uint8_t)((pubkey[63] % 2 == 0) ? 0x02 : 0x03));
                script_sign += pubkey.split(0, 0x20);
                
                return script_sign;
            };
            
            auto basic_script = make_basic_script(tx);
            auto signed_tx = basic_script;
            std::string change_path = tx["changePath"];
            auto change_path_buffer = change_path.empty()
            ? bytestream()
            : path::to_buffer(tx["changePath"]);
            
            std::vector<json> inputs = tx["inputs"];
            for (int i = 0; i < inputs.size(); ++i) {
                auto input = inputs[i];
                auto path_buffer = path::to_buffer(input["path"]);
                auto pre_sign_script = make_presign_script(i, basic_script);
                auto apdu_data =
                build_sign(path_buffer, change_path_buffer, pre_sign_script);
                auto response = send_sign(apdu_data, true);
                auto sign_resp = parse_sign_resp(coin_type, response);
                auto script_sign =
                make_script_sign(std::get<2>(sign_resp),
                                 std::get<3>(sign_resp),
                                 std::get<4>(sign_resp));
                
                signed_tx["inputs"][i]["scriptSig"] = script_sign.hex_str();
            }
            
            bytestream stx;
            stx += bytestream("01000000");
            
            // inputs serializer
            std::vector<json> sinputs = signed_tx["inputs"];
            stx.append((unsigned char) sinputs.size());
            for (auto input: sinputs) {
                auto hash = input["hash"].get<std::string>();
                stx += bytestream(hash).little_ending();
                auto idx = bytestream();
                idx.append((unsigned int)input["index"]);
                stx += idx.little_ending();
                std::string script = input["scriptSig"];
                stx.append((unsigned char)(script.length()/2));
                stx += bytestream(script);  // 签名前，script置为空
                auto seq = bytestream();
                seq.append((unsigned int)input["sequence"]);
                stx += seq.little_ending();
            }
            //output serializer
            std::vector<json> outputs = signed_tx["outputs"];
            stx.append((unsigned char) outputs.size());
            for (auto output: outputs) {
                uint64_t amount = output["amount"];
                auto amountHex = bytestream();
                amountHex.append((uint64_t) amount).little_ending();
                stx += amountHex;
                
                std::string out_script = output["scriptPubKey"];
                auto script_hex = bytestream(out_script);
                stx.append((unsigned char)script_hex.length());
                stx += script_hex;
            }
            stx.append((unsigned int)signed_tx["lockTime"]);
            auto tid = sha256::hash(sha256::hash(stx));
            return json{
                {"id", tid.reverse().hex_str()},
                {"hex", stx.hex_str()}
            };
        };
        
        auto sign_eth = [&](const std::string &net_type, json tx)
        {
            auto chain_id = eth::get_chain_id(net_type);
            json vrs = {
                {"V", chain_id},
                {"R", "00"},
                {"S", "00"}
            };
            auto unsigned_tx = eth::rlp_encoding(tx, vrs);
            
            std::cout << unsigned_tx.hex_str() << std::endl;
            
            auto apdu =
            build_sign(path::to_buffer(tx["input"]["path"]), bytestream(""), unsigned_tx);
            auto resp = send_sign(apdu, false);
            auto rest = parse_sign_resp(coin::eth, resp);
            
            auto v = std::get<1>(rest);
            auto r = std::get<2>(rest).hex_str();
            auto s = std::get<3>(rest).hex_str();
            json signed_vrs = {
                {"V", v},
                {"R", r},
                {"S", s}
            };
            
            auto raw_tx = eth::rlp_encoding(tx, signed_vrs);
            
            auto tx_id = keccak256::hash(raw_tx);
            return json{
                {"id", tx_id.hex_str()},
                {"hex", raw_tx.hex_str()}
            };
        };
        
        auto sign_eos = [&](const std::string &net_type, json tx) {
            auto chain_id = eos::get_chain_id(net_type);
            
            auto rawTx = fcbuffer::serialize(tx);
            auto packedContextFreeData = bytestream(0x20);
            auto signBuf = chain_id + rawTx + packedContextFreeData;
            auto signedTx = json {
                {"compression", "none"},
                {"packedContextFreeData", ""},
                {"packed_trx", rawTx.hex_str()},
                {"signatures", {}
                }
            };
            std::vector<std::string> signatures;
            while (true) {
                auto resp = send_sign(signBuf, false);
                auto rest = parse_sign_resp(coin::eos, resp);
                auto remain = std::get<0>(rest);
                
                auto v = std::get<1>(rest);
                auto r = std::get<2>(rest).hex_str();
                auto s = std::get<3>(rest).hex_str();
                auto i = v + 4 + 27;
                
                auto buffer = bytestream();
                buffer.append((uint8_t)i);
                buffer += r;
                buffer += s;
                
                auto checkBuffer = buffer + bytestream("K1", 2);
                auto check = ripemd160::hash(checkBuffer).split(0,4);
                auto sign = buffer + check;
                auto signature = EncodeBase58(sign.mem());
                signatures.push_back("SIG_K1_" + signature);
                std::cout << "stgnatures:" << "SIG_K1_" + signature << std::endl;
                if (remain == 0) break;
            }
            signedTx["signatures"] = json(signatures);
            auto txId = sha256::hash(rawTx).hex_str();
            return json {
                {"txId", txId},
                {"signedTx", signedTx}
            };
        };
        
        if (coin_type == coin::eth) {
            return sign_eth(wallet::eth_main, tx);
        }
        else if (coin_type == coin::eos) {
            return sign_eos(wallet::eos_main, tx);
        }
        return sign_btc(coin_type, tx);
    }
    
private:
    json pase_pmdata(coin coin_type,
                     uint32_t account_index,
                     const bytestream &pm)
    {
        uint8_t type = pm[0];
        
        std::string data;
        if (type == 0)
        { // type[1] actor[8] name[8] path[20]
            uint32_t pm_account_index = pm.readUInt32(1 + 8 + 8 + 4 * 2);
            if (pm_account_index != 0x80000000 + account_index)
            {
                return json();
            }
            auto path = pm.split(17, 20);
            data = path::from_buffer(data);
        }
        else if (type == 1)
        {
            // type[1] actor[8] name[8] account[4] publicKey[33]
            uint32_t pm_account_index = pm.readUInt32(1 + 8 + 8);
            if (pm_account_index != 0x80000000 + account_index)
            {
                return json();
            }
            auto pubkey = pm.split(21, 33);
            data = address::from_buffer(coin_type, pubkey);
        }
        else
        {
            SPDLOG_WARN("getPermissions unknown type");
        }
        auto actor_name = fcbuffer::decode_name(pm.split(1, 8).reverse().hex_str());
        auto name = fcbuffer::decode_name(pm.split(9, 8).reverse().hex_str());
        return {
            {"type", (int)type},
            {"actor", actor_name},
            {"name", name},
            {"data", data}
        };
    }
    
private:
    bytestream m_selected_app;
        bytestream send(const bytestream &apdu, bool encrypt = true) {
            std::cout << "apdu:" << std::endl;
            std::cout << apdu.hex_str() << std::endl;
           // TODO: 各种指令
           bytestream repo;
           if (apdu.mem()[0x01] == 0x84) // 取随机数
           {
             repo = bytestream(apdu.mem()[0x04], true);
           }
           else if (apdu.startWith("00A4040008B000000000")) {
             m_selected_app = apdu.tail(3).hex_str();
           }
            else if (apdu.startWith("804A000000")) {
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
           else if (apdu.startWith("8048")) { //BTC
               if (m_selected_app == "020002") { // btc
                 repo = bytestream("690FC37056A358C3A5B03F0375FF846CC9D48DB4BFF1AA782250D55812D213CE1B52FC5EBF811F2DEF0717AED8F851EE63B4491D7E9B084F3D92F0FA061137409B7EF9A41F5DCF7C1EA17CFC2493C24F5204B2E11BA21662192749226C0210B2553039087F380BEAB40FF933F045D06C7AB918F909B122F6FB9B48A07327203AFF");
               } else if (m_selected_app == "023C02") { // eth
                 repo = bytestream("27915d275bb821af7c0ad622145571e964b6521b60f3de0ded56f3dff36afbf0412e897a21167e67e14520ca30f6e2b492ac0a37e4b537bb20092aff23cf9ef7")+ bytestream(0x40).append((uint8_t)0x2C);
               } else if (m_selected_app == "02C202") { // eos
                 repo = bytestream("0015d9175f1cc6e2f53b8af2970ce1409873e2f015b8d79034516e065a46086d524050e4625490bfb0e08d79acccbbd8e14c54a170f41670c0b7e7210d93154af6")+ bytestream(0x40).append((uint8_t)0x00);
               }
           }
    
           return repo += bytestream("9000");
         }
//    bytestream send(const bytestream& apdu, bool encrypt = false) {
//        static uint8_t recv[6864] = {0};
//        uint32_t recvLen = sizeof(recv);
//        SPDLOG_INFO("apdu:" + apdu.hex_str());
//        uint32_t ret = Transmit(apdu.bytes(), (uint32_t)apdu.length(), recv, &recvLen, encrypt);
//        std::stringstream ss;
//        ss << "send failed:" << std::setw(8) << std::hex << ret << std::endl;
// 
//        if (ret) { SPDLOG_INFO(ss.str()); throw tsm_err("send apdu failed", ret); }
//        auto resp = bytestream(recv, recvLen);
//        SPDLOG_INFO("resp:" + resp.hex_str());
//        return resp;
//    }
    
    // get applet id
    std::string get_app_id(wallet::applet applet)
    {
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
                SPDLOG_WARN("unkown applet type for app ID");
                return "";
        }
    }
    
    json get_version_info(const std::string name, applet app)
    {
        select(app);
        
        json vi;
        vi["name"] = name;
        vi["installed"] = false;
        
        try {
            auto apdu = bytestream("804A000000");
            auto resp = send(apdu, false);
            
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
            SPDLOG_ERROR(name + "get version failed");
        }
        return vi;
    }
    
    std::vector<json> get_applet_versions()
    {
        std::vector<std::string>
        app_names{"HDWallet", "Manager", "Backup", "BTC",      "ETH",     "EOS"};
        
        std::vector<applet>
        applet_types{applet::hdwallet, applet::manager, applet::backup, applet::btc, applet::eth, applet::eos};
        
        std::vector<json> versions;
        
        for (int i = 0; i < app_names.size(); ++i)
        {
            auto version = get_version_info(app_names[i], applet_types[i]);
            versions.emplace_back(version);
        }
        return versions;
    }
    
private:
    std::string m_cur_app_id;
    bool all_enc;
    
    std::string m_btc_net;
    std::string m_eth_net;
    std::string m_eos_net;
};
} // namespace wallet
} // namespace excelsecu
