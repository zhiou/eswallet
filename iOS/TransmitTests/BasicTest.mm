//
//  TransmitTests.m
//  TransmitTests
//
//  Created by fire on 2019/07/31.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#import <XCTest/XCTest.h>
#import <bytestream.hpp>


#import <algorithm/hash/hash.hpp>
#import <algorithm/sm/sm2.hpp>
#import <algorithm/cipher/cipher.hpp>
#import <spdlog/spdlog.h>
#import <wallet/eth/rlp/rlp.hpp>

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
    
    bs11.append<4>(1024);
    std::cout << "bs11 =" << bs11 << std::endl;
    
}

- (void)testHash {
    bytestream bs1("11223344");
    auto sm3_digest = sm3::hash(bs1);
    std::cout << "sm3:" << std::endl << sm3_digest << std::endl;
    
    auto sm3_mapper = bs1.map<bytestream>([](const bytestream& msg){
        return sm3::hash(msg);
    });
    
    auto sm3_hash = bs1.map<bytestream>(sm3::hash);
    std::cout << "sm3_mapper:" << std::endl << sm3_digest << std::endl;
    
    XCTAssert(sm3_digest == "620DEC865F20797E81196CAECCB37C4559AE3C83A4DDC0685E5766FC7DF5FE78");
    XCTAssert(sm3_mapper == "620DEC865F20797E81196CAECCB37C4559AE3C83A4DDC0685E5766FC7DF5FE78");
    XCTAssert(sm3_hash == "620DEC865F20797E81196CAECCB37C4559AE3C83A4DDC0685E5766FC7DF5FE78");
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
    SPDLOG_DEBUG("hello %s %d times!", "world", 3000);
//    enable_file_logger("eslog.txt");
    SPDLOG_DEBUG("hello %s %d times!", "world", 3000);
    
    SPDLOG_DEBUG("complete");
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

- (void)testRlp {
    std::string dog = "dog";
    auto ret1 = eth::rlp_encoding(dog);
    std::cout << "ret1 = " << ret1.hex_str() << std::endl;
    
    auto ret2 = eth::rlp_encoding(bytestream("0F"));
    std::cout << "ret2 = " << ret2.hex_str() << std::endl;
    
    dog = "";
    auto ret3 = eth::rlp_encoding(dog);
    std::cout << "ret3 = " << ret3.hex_str() << std::endl;
    
    auto ret4 = eth::rlp_encoding(bytestream("0400"));
    std::cout << "ret4 = " << ret4.hex_str() << std::endl;
    
    auto ret5 = eth::to_binary(1024);
    std::cout << "ret5 = " << bytestream(ret5.c_str(), 0x02).hex_str() << std::endl;
    
    dog = "0x0987654321098765432109876543210987654321";
    auto ret6 = eth::rlp_encoding(dog);
     std::cout << "ret6 = " << ret6.hex_str() << std::endl;
    
    auto ret7 = eth::rlp_encoding(itobs(1000000000));
    std::cout << "ret7 = " << ret7.hex_str() << std::endl;
    
    auto ret8 = eth::rlp_encoding(itobs((int)(0.518 * 100000000)));
    std::cout << "ret8 = " << ret8.hex_str() << std::endl;
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}


@end
