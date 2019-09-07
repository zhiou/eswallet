//
//  error.h
//  transmit
//
//  Created by zhiou on 2019/8/21.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "EsError.h"
/*
 错误码只使用EsError里的ERROR_COMMON前缀的错误码的语义，前两字节代表模块ID，
 后两字节代表错误类型
 COS返回的错误码直接使用不做转换，软件层和硬件层的错误码分别处理。
 各模块内按各自的模块ID生成自己的错误码。
 应用层根据错误码语义字节进行处理，这样就最多只需要处理两种语义，比如无效参数最多出现软件和硬件两种。
 */

namespace excelsecu {

using cos_err_desp_map = std::unordered_map<std::string, std::string>;

static cos_err_desp_map cos_err_desp = {
    // from cos
    {"9000", "正确"},
    {"61XX", "正常结束，仍有XX个有效数据可取"},
    {"6283", "选择的文件已失效"},
    {"6285", "选择的文件已终止"},
    {"6300", "没有信息被给出，验证失败"},
    {"63CX", "剩余尝试次数"},
    {"6581", "写EEPROM失败"},
    {"6600", "当前安全环境不能被设置和修改"},
    {"6700", "数据长度错误"},
    {"67XX", "错误的长度Le；XX指示准确长度"},
    {"6882", "不支持的安全报文"},
    {"6883", "希望结束此命令链"},
    {"6884", "不支持命令链"},
    {"6981", "文件类型不匹配"},
    {"6982", "安全状态不满足"},
    {"6983", "密钥已经被锁住"},
    {"6984", "引用的数据无效"},
    {"6985", "使用条件不满足"},
    {"6986", "命令不被允许(没有当前EF)"},
    {"6987", "缺少安全报文数据对象"},
    {"6988", "安全报文数据项不正确"},
    {"69FF", "有数据返回，返回数据长度为P3"},
    {"6A80", "数据域参数不正确"},
    {"6A81", "功能不支持"},
    {"6A82", "没有找到文件"},
    {"6A83", "没有找到记录"},
    {"6A84", "没有足够的空间"},
    {"6A85", "Lc与TLV结构不一致"},
    {"6A86", "P1，P2参数不正确"},
    {"6A88", "引用的数据未找到"},
    {"6A89", "标识符已存在"},
    {"6A8A", "指定的应用已打开"
             "管理指令Tag错误，仅用于内部测试"},
    {"6A8B", "协商密钥错误，不支持的算法"
             "指定的应用不存在"},
    {"6A8C", "引用的会话密钥不存在"},
    {"6A8D", "数据错误(密钥句柄与EncryptInit命令指定的不符)"},
    {"6A90", "已有打开的应用，当前设备不支持同时打开多个应用"},
    {"6A91", "指定的容器未找到"
             "修改PIN失败，恢复原有值"},
    {"6A92", "同名文件已存在"},
    {"6A93", "文件不存在"},
    {"6A94", "引用的容器未找到"},
    {"6A95", "容器中没有对应的密钥对"},
    {"6A96", "指定类型的证书不存在"},
    {"6A97", "SM2解密失败，密文信息不对"},
    {"6A98", "正确执行，但验证签名失败"},
    {"6A99", "不支持的会话密钥算法标识"},
    {"6A9A", "非对称加密失败"
             "线路保护协商密钥SM2加密出错"},
    {"6A9B", "非对称解密失败"
             "SM2解密哈希值校验出错"},
    {"6A9C", "没有做自主认证"
             "蓝牙盾没有认证特征密钥"},
    {"6A9D", "不支持的摘要算法标识"},
    {"6A9E", "还有更多数据可取"},
    {"6AA6", "NFC设备正忙"},
    {"6B00", "参数错误(偏移地址超出了EF文件长度)"},
    {"6B01", "无效的私钥"},
    {"6B02", "模乘出错"},
    {"6B05", "会话密钥已满"},
    {"6CXX", "错误的长度Le；XX指示准确长度"},
    {"6D00", "不正确的INS"},
    {"6E00", "不正确的CLA"},
    {"6F00", "没有精确的诊断"},
    {"6F01", "音频支持插入指令的二代签名中间状态码"},
    {"6F02", "NFC卡盾二次拍卡的时候，没空处理接收的指令，只能回此码告诉上层卡盾"
             "没准备好"},
    {"6F03", "等待按键时，蓝牙断开连接"},
    {"6F04", "音频支持插入指令的二代签名中间状态码，当需要2次按键确认时，第一次"
             "按键后要返回的操作码"},
    {"6F05", "自主认证失败"},
    {"6F06", "默认密码不允许操作"},
    {"6FD0", "电量低"},
    {"6FE0", "RSA计算错误"},
    {"6FE5", "公私钥不匹配"},
    {"6FE1", "不知道什么意思"},
    {"6FE3", "不知道什么意思"},
    {"6FE4", "不知道什么意思"},
    {"6FE7", "网银正在使用，无法连接"},
    {"6FE8", "不需要断开蓝牙连接"},
    {"6FE9", "线路保护密钥错误"},
    {"6FEA", "算法自检失败"},
    {"6FEB", "交易报文出错"},
    {"6FEC", "更新保护密钥序列号错误"},
    {"6FED", "私钥属性不匹配"},
    {"6FEE", "出厂配置错误"},
    {"6FEF", "取响应超时错误"},
    {"6FF0", "设备未完成出厂配置"},
    {"6FF1", "生成密钥对错误"},
    {"6FF2", "超时断开连接错误"},
    {"6FF3", "上翻"},
    {"6FF4", "下翻"},
    {"6FF5", "安全记录文件错误"},
    {"6FF6", "找不到安全记录的起始数据块"},
    {"6FF7", "没有按键产生"},
    {"6FF8", "取消操作"},
    {"6FF9", "操作超时"},
    {"6FFA", "插入指令错误"},
    {"6FFB", "通讯超时或通讯数据格式错误"},
    {"6FFC", "文件系统逻辑错误"},
    {"6FFD", "备份错误"},
    {"6FFE", "按键故障"},
    {"6FFF", "没有协商密钥"},

    {"9000", "操作成功"},
    {"9002", "写Flash超过范围"},
    {"9060", "设备正忙，请等待。和以前的60是一个效果"},
    {"9082", "提示中间件需要按键确认——修改PIN"},
    {"9083", "提示中间件需要按键确认——删除证书"},
    {"9085", "提示中间件需要按键确认——初始化"},
    {"908A", "提示中间件需要按键确认——二代签名"},
    {"908B", "提示中间件需要按键确认——生成密钥对"},
    {"908C", "提示中间件需要按键确认——解锁PIN"},
    {"908E", "提示中间件需要发点阵数据"},
    {"909X", "提示中间件需要按键确认——密码重试次数剩余X次"},

    {"91XX", "正常结束，XX为STK的反馈信息"},
    {"9300", "STK忙，命令无法执行"},
    {"93XX", "获取Flash容量出错，xx为chipid"},
    {"9303", "应用被永久锁定"},
    {"9400", "没有当前文件"},
    {"9401", "不允许修改成当前密码"},
    {"9402", "偏移超出范围"},
    {"9403", "密钥索引不支持"},
    {"9404", "文件没有找到，记录没有找到"},
    {"9408", "文件类型不匹配"},
    {"9501", "公钥错误"},
    {"9502", "SM2加密，计算k*G出错"},
    {"9503", "SM2解密，C1不在曲线"},
    {"9802", "口令没有初始化"},
    {"9804", "访问条件不满足，认证不成功"},
    {"9808", "口令状态不满足"},
    {"9810", "口令失效"},
    {"9840", "认证被锁定"},
    {"9850", "已经达到记录最大值，不能执行INCREASE"},
    {"9FXX", "正常结束，仍有XX个有效数据可取"},
    {"9EXX", "SIM数据下载出错"},
    {"99FF", "cos内部切换连接方式"},

    {"6304", "SM9验签失败"},
    {"8009", "调用SM9算法库失败"},

    // FROM Common
    {"0001", "等待互斥体失败"},
    {"0002", "主机内存不足"},

    {"0003", "数据错误"},
    {"0004", "数据长度错误"},
    {"0005", "缓冲区不足"},
    {"0006", "验签失败"},

    {"0007", "无效参数"},
    {"0008", "没找到"},
    {"0009", "共享内存偏移错误"},
    {"000A", "共享内存长度错误"},

    {"000B", "等待超时"},
    {"000C", "连接错误"},
    {"000E", "没有收到实际数据"},

    {"000D", "已经到了链表的尾部"},
    {"000F", "条件状态不满足"},
    {"0010", "没有设备"},
    {"0011", "设备过多"},
    {"0012", "已存在"},
    {"0013", "功能不支持"},             // 可用于不支持蓝牙等错误
    {"0014", "访问被拒绝，如没有权限"}, // 可用于未开启蓝牙权限等错误
    {"0015", "打开失败"},
    {"0016", "执行失败"},
    {"0017", "未打开"},
    {"0018", "数据有误"},     // 同0003， 不要使用
    {"0019", "返回长度有误"}, // 同0004， 不要使用
    {"001A", "写失败"},
    {"001B", "读失败"},
    {"001C", "无效句柄"},
    {"001D", "没有初始化"},
};

class cos_code {
public:
  explicit cos_code(uint16_t code) : m_value(0x00) {
    char code_char[0x05] = {0};
    sprintf(code_char, "%02X%02X", (code & 0xFF00) >> 8, code & 0xFF);
    parse(std::string(code_char));
  }

  explicit cos_code(const std::string &code) : m_value(0x00) {
    parse(std::string(code));
  }

  cos_code(const cos_code &acode) {
    m_cos_code = acode.m_cos_code;
    m_value = acode.m_value;
  }

  bool operator==(const cos_code &c) const {
    return code_equal(m_cos_code, c.m_cos_code);
  }

  const std::string &code_str() const { return m_cos_code; }

  uint16_t code() const { // TODO - process XX
    uint16_t temp = 0x0000;
    for (size_t i = 0; i < m_cos_code.length(); i += 2) {
      std::string byte = m_cos_code.substr(i, 2);
      if (byte[0] == 'X') {
        byte[0] = '0';
      }
      if (byte[1] == 'X') {
        byte[1] = '0';
      }
      unsigned char chr = (unsigned char)(int)strtol(byte.c_str(), NULL, 16);
      temp <<= 8;
      temp |= chr;
    }
    return temp | m_value;
  }

  uint8_t value() const { return m_value; }

  std::string hash_str() const { return m_cos_code; }

private:
  void parse(const std::string &new_code) { // 拆分cos_code
    for (auto iter = std::begin(get_desp_map());
         iter != std::end(get_desp_map()); ++iter) {
      auto key_str = iter->first;

      if (code_equal(key_str, new_code)) {

        size_t pos = 0;

        pos = key_str.find('X');
        while (pos != std::string::npos) {
          m_value <<= 4;
          m_value |= (new_code[pos] - '0');
          pos = key_str.find('X', pos + 1);
        }

        m_cos_code = key_str;
      }
    }
  }

  bool code_equal(const std::string &code1, const std::string &code2) const {
    if (code1.length() != code2.length()) {
      return false;
    } // 长度不同

    if (code1 == code2) {
      return true;
    } // 完全匹配

    size_t len = code1.length();
    for (int i = 0; i < len; ++i) {
      if (code1[i] != code2[i] // 某字节不同，且不是X替换符
          && code1[i] != 'X' && code2[i] != 'X') {
        return false;
      }
    }
    return true;
  }

public:
  std::string m_cos_code; // 可能带有占位符
  uint8_t m_value;        // 如果带有占位符，则为XX 的值

  static cos_err_desp_map &get_desp_map() { return cos_err_desp; }
};

class es_error : public std::exception {
public:
  es_error(int code,
           int module_id =
               0xE060) // 用于COS返回的错误码, module_id 默认为COS的moduleID
      : code_(cos_code(code)), module_(module_id), msg_("") {
    if (cos_code::get_desp_map().find(code_.code_str()) !=
        std::end(cos_code::get_desp_map())) {
      msg_ = cos_code::get_desp_map()[code_.code_str()];
    }
  }

  es_error(const std::string &code,
           int module_id =
               0xE060) // 用于COS返回的错误码, module_id 默认为COS的moduleID
      : code_(std::move(cos_code(code))), module_(module_id), msg_("") {

    if (cos_code::get_desp_map().find(code_.code_str()) !=
        std::end(cos_code::get_desp_map())) {
      msg_ = cos_code::get_desp_map()[code_.code_str()];
    }
  }

  es_error(const cos_code &code,
           int module_id =
               0xE060) // 用于COS返回的错误码, module_id 默认为COS的moduleID
      : code_(code), module_(module_id), msg_("") {
    if (cos_code::get_desp_map().find(code_.code_str()) !=
        std::end(cos_code::get_desp_map())) {
      msg_ = cos_code::get_desp_map()[code_.code_str()];
    }
  }

  es_error(const std::string &msg, int code,
           int module_id = 0xE060) // 用于未定义的错误码
      : msg_(std::move(msg)), code_(code), module_(module_id) {}

  const char *what() const noexcept override { return msg_.c_str(); }

  const uint32_t code() const noexcept {
    return ((uint32_t)module_) << 16 | code_.code();
  }

  const cos_code &raw_code() const { return code_; }

private:
  std::string msg_;
  cos_code code_;
  uint16_t module_;
};

constexpr int MODULE_ID_COMMON = 0xE060;
constexpr int MODULE_ID_DRIVER = 0xE061;
constexpr int MODULE_ID_COS = 0xE062;
constexpr int MODULE_ID_TRANSMIT = 0xE063;

#define tsm_err(A, C) es_error(A, C, MODULE_ID_TRANSMIT)
#define cmn_err(A, C) es_error(A, C & 0xFFFF, MODULE_ID_COMMON)
#define drv_err(A, C) es_error(A, C & 0xFFFF, MODULE_ID_DRIVER)
#define cos_err(C) es_error(C);
} // namespace excelsecu
