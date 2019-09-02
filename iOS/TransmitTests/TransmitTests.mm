//
//  TransmitTests.m
//  TransmitTests
//
//  Created by fire on 2019/07/31.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "bytestream.hpp"

#import "device.hpp"
#import "transmit.hpp"
#import "ble_driver.hpp"
#import "authenticator.hpp"
#import "configure.hpp"
#import "audio_driver.hpp"
#import "driver.hpp"
#import "hash.hpp"
#import "sm2.hpp"
#import "cipher.hpp"
#import "driver_mocker.hpp"
#import "eslog.hpp"
#import "basic_file_sink.hpp"
#import "wallet.hpp"
#import "json.hpp"
#import "path.hpp"
#import "fcbuffer.hpp"
#import "s300.hpp"

#import <array>
#import <regex>
using namespace excelsecu;

@interface TransmitTests : XCTestCase

@end

@implementation TransmitTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.

}

- (void)testBytestream {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
    
    bytestream bs0;
    std::cout << "bs0:" << std::endl << bs0 << std::endl;
    
    bytestream bs1(10);
    std::cout << "bs1:" << std::endl << bs1 << std::endl;
    
    bytestream bs2(10, true);
    std::cout << "bs2:" << std::endl << bs2 << std::endl;
    
    bytestream bs3("11223344556677889900");
    std::cout << "bs3:" << std::endl << bs3 << std::endl;
    
    bytestream bs4(bs3.mem());
    std::cout << "bs4:" << std::endl << bs4 << std::endl;
    
    bytestream bs5;
    
//    bs5.append<ebyte>(0x01);
    
    bs5.append((uint16_t)0x0203);
    
    bs5.append((uint32_t)0x04050607);
    
    bs5.append((uint64_t)0x0809000102030405);
    
    bs5.append((unsigned short)20);
    
    std::cout << "bs5:" << std::endl << bs5 << std::endl;
    
    bytestream bs6(bs5);
     std::cout << "bs6(bs5):" << std::endl << bs6 << std::endl;
    
    bytestream bs7 = bs5;
     std::cout << "bs7 = bs5:" << std::endl << bs7 << std::endl;
    
    bs7 += bs5;
    std::cout << "bs7 += bs5:" << std::endl << bs7 << std::endl;
    
    bytestream bs8 = bs6 + bs7;
    std::cout << "bs8 = bs6 + bs7:" << std::endl << bs8 << std::endl;
    
    bytestream bs9 = ~bs8;
    std::cout << "bs9 = ~bs8:" << std::endl << bs9 << std::endl;
    
    bytestream bs10 = bs8 ^ bs9;
    std::cout << "bs10 = bs8 ^ bs9:" << std::endl << bs10 << std::endl;
    
    bs10 ^= bs8;
    std::cout << "bs10 ^= bs8:" << std::endl << bs10 << std::endl;
    
    std::cout << "bs10 hex_str:" << std::endl << bs10.hex_str() << std::endl;
    
//    XCTAssert(bs10 == bs9, @"bs10 == bs9");
    if(bs10 == bs9) std::cout << "bs10 == bs9" << std::endl;
    
    if(bs10 == "ffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffffffffffffffffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffffffffffffffffffffffffffffffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffffffffffffffffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfa")
        
        std::cout << "bs10==ffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffffffffffffffffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffffffffffffffffffffffffffffffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfaffffffffffffffffeeddccbbaa99887766fefdfcfbfaf9f8f7f6fffefdfcfbfa" << std::endl;
    XCTAssert(bs10 == bs9);
    
    auto bs11 = bs10.tail(2);
    
    std::cout << "bs11 = bs10.tail(2) = " << bs11 << std::endl;
    
}

- (void)testHash {
    bytestream bs1("11223344");
    auto sm3_digest = sm3::hash(bs1);
    std::cout << "sm3:" << std::endl << sm3_digest << std::endl;
    
    auto sm3_digest_mapper = bs1.map<bytestream>([](const bytestream& msg){
        return sm3::hash(msg);
    });
    std::cout << "sm3_mapper:" << std::endl << sm3_digest << std::endl;
}

- (void)testDevice {
    
    configure::current().set_host_name("周煌的iphone");
    
    using mock_handshake = handshake<driver_mocker, authenticator<driver_mocker, configure>, configure>;
    using mock_device = device<transmit<driver_mocker, mock_handshake>>;
    mock_device mocker;
    
    mocker.connect("hehe");
    auto apdu_get_random = bytestream("0084000008");
    auto repo = mocker.send(apdu_get_random);
    std::cout << "repo = " << repo << std::endl;
}

- (void)testSm2 {
    
    auto sm2_keypair = sm2::gen_keypair();
    
    std::cout << "prikey = " << sm2_keypair.first << std::endl;
    std::cout << "pubkey = " << sm2_keypair.second << std::endl;
    
    bytestream plain("1122334411223344112233441122334411223344112233441122334411223344");
    
    bytestream cipher = sm2::encrypt(plain, bytestream(sm2_keypair.second));
    std::cout << "cipher = " << cipher << std::endl;
    
    bytestream decrypted = sm2::decrypt(cipher, bytestream(sm2_keypair.first));
    std::cout << "plain = " << decrypted << std::endl;
    
    XCTAssert(decrypted == plain);
    
    bytestream test_decrypted = sm2::decrypt(bytestream("52d68335007df20b5ad71c6ed98266ccaec79b67238913c41ebd6de7096c96ac0917b2c09dbf84c9e2d44f0a5cdf1c0a6dff43f2f3e2434a567b7f25ee6a84d66ec44cc7d7eac7bfecc4dfd37bd555b29392692323dedaf2a0bd6b0666b6d2a8617c2afe3e78c46bab962976456f506fee9d28b1e8e790b876e946f137c539db73c6ac851a406b04"), bytestream("fdefbd92fff7ffd67f9faf055fdfd954dfffb74757fdbc55dfefd9febffddd2a"));
    
    
    bytestream digest = sm3::hash(plain);
    bytestream sign = sm2::sign(digest, sm2_keypair.first);
    
    std::cout << "sign = " << sign << std::endl;
    
    bool sucess = sm2::verify(digest, sign, sm2_keypair.second);
    
    XCTAssert(sucess, "sm2 verify sucess");
}

- (void)test3des112 {
    auto key = bytestream("11223344556677881122334455667788");
    auto plain = bytestream("12345678123456781234567812345678");
    auto cipher = cipher::encrypt(cipher::type::tdes112,key, plain);
    auto result = cipher::decrypt(cipher::type::tdes112,key, cipher);
    
    std::cout << "cipher = " << cipher << std::endl;
    std::cout << "plain = " << result << std::endl;
    
    XCTAssert(plain == result);
}

-(void)testLogger {
//    auto log = registry::instance().default_logger();
    
//    file_logger->info("%s %d", __FUNCTION__, __LINE__);
    LOG_DEBUG("hello %s %d times!", "world", 3000);
    enable_file_logger("eslog.txt");
    LOG_DEBUG("hello %s %d times!", "world", 3000);
    
    LOG_DEBUG("complete");
}

- (void)testError {

    try {
        throw es_error("6FFB");
    }catch (es_error &e) {
        std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what() << std::endl;
    }

    try {
        throw es_error(0x6FFA);
    }catch (es_error &e) {
         std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what() << std::endl;
    }

    try {
        throw es_error("9000");
    }catch (es_error &e) {
        std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what() << std::endl;
    }
    
    try {
        throw es_error("63C5");
    }catch (es_error &e) {
        std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what() << std::endl;
    }
    
    try {
        throw es_error(0x63C3);
    }catch (es_error &e) {
        std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what() << std::hex << std::setw(2) << (int) e.raw_code().value() << std::endl;
    }
    
    try {
        throw es_error("6C88");
    }catch (es_error &e) {
        std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what()<< std::hex << std::setw(2) << (int)e.raw_code().value() << std::endl;
    }
    
    try {
        throw es_error(0x91FB);
    }catch (es_error &e) {
        std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what() << std::hex << std::setw(2) << (int)e.raw_code().value()<< std::endl;
    }
    
    try {
        throw es_error("user defined error", 0x6FFB, 0xE061);
    } catch (es_error &e) {
           std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what() << std::endl;
    }
    
    try {
        throw es_error("invalid param", 0x0007, 0xE063);
    } catch (es_error &e) {
        std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what() << std::endl;
    }
    
    try {
        throw tsm_err("cert not find", ERROR_COMMON_NOT_FOUND);
    }catch (es_error &e) {
        std::cout << "code = 0x" << std::hex << std::setw(8) << e.code() << "\nreason = " << e.what() << std::endl;
    }
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

- (void)testWallet {
    XCTAssert(wallet::is_btc("btc"));
    XCTAssert(!wallet::is_btc("kfc"));
    
    XCTAssert(wallet::is_eth("eth"));
    XCTAssert(!wallet::is_eth("fth"));
    
    XCTAssert(wallet::is_eos("eos"));
    XCTAssert(!wallet::is_eos("eth"));
}

- (void)testJson {
    using json = nlohmann::json;
    
    json j = { {"walletID", "020001"} };
    std::cout << j << std::endl;
    
    json jj = { {"jj", j}, {"include", true } };
    std::cout << jj << std::endl;
}

- (void)testPath {
    auto buf = excelsecu::wallet::path::to_buffer("m/44'/60'/1'/0/0");
    std::cout << buf << std::endl;
}

- (void)testFcBuffer {
    std::string name = "omotjik";
    auto encoded = wallet::fcbuffer::encode_name(name);
    std::cout << encoded << std::endl;

    auto decoded = wallet::fcbuffer::decode_name(encoded);
     std::cout << decoded << std::endl;
}


- (void)testS300 {
    
    configure::current().set_host_name("周煌的iphone");
    
    using mock_handshake = handshake<driver_mocker, authenticator<driver_mocker, configure>, configure>;
    using mock = wallet::s300<transmit<driver_mocker, mock_handshake>>;
    mock s300_mocker;
    
    s300_mocker.connect("hehe");
//    auto apdu_get_random = bytestream("0084000008");
//    auto repo = mocker.send(apdu_get_random);
//    std::cout << "repo = " << repo << std::endl;
    
    s300_mocker.init();
    
    auto ret = s300_mocker.get_wallet_info();
    std::cout << ret << std::endl;
 
    s300_mocker.get_pubkey(wallet::coin::btc, "m/44'/1'/0'/0/0");
    
    s300_mocker.get_address(wallet::coin::btc, "m/44'/1'/0'/0/0");
    
    ret = s300_mocker.get_derive_data(wallet::coin::btc, "m/44'/1'/0'/0/0");
    std::cout << ret << std::endl;
    
    // key
    auto account_name = s300_mocker.get_account_name(wallet::coin::eos, 0, "m/44'/1'/0'/0/0");
    std::cout << account_name << std::endl;
    
    s300_mocker.get_default_permission(wallet::coin::eos, 0);
    
//    auto permissions = s300_mocker.get_permission(wallet::coin::eos, 0);
}
@end
