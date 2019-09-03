//
//  AlgorithmTest.m
//  TransmitTests
//
//  Created by zhiou on 2019/9/2.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#import <XCTest/XCTest.h>

#import "driver_mocker.hpp"
#import "handshake.hpp"
#import "configure.hpp"
#import "transmit.hpp"

#import "wallet.hpp"
#import "json.hpp"
#import "path.hpp"
#import "fcbuffer.hpp"
#import "s300.hpp"

#import <array>
#import <regex>

using namespace excelsecu;

using mock_handshake = handshake<driver_mocker, authenticator<driver_mocker, configure>, configure>;

using mock = wallet::s300<transmit<driver_mocker, mock_handshake>>;

@interface AlgorithmTest : XCTestCase
{
     mock s300_mocker;
}
@end

@implementation AlgorithmTest

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    configure::current().set_host_name("周煌的iphone");

    s300_mocker.connect("hehe");
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    s300_mocker.disconnect();
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
    
    uint64_t update_auth = 0xD5526CA8DACB4000;
    auto encoded_name = wallet::fcbuffer::decode_name(update_auth);
    
    std::cout << encoded_name << std::endl;
}


- (void)testS300 {

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

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
