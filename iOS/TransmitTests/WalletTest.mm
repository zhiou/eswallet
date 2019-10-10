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
    
    auto path = excelsecu::wallet::path::from_buffer(buf);
    XCTAssert(path == "m/44'/60'/1'/0/0");
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

    nlohmann::json tx = {
        {
            "inputs", {
                {
                    {"txId", "476015ca2c1bde00bee951c132526c737f4754eee9defd3f0278d4f0d394bede"},
                    {"index", 0},
                    {"script", "76A91492a0aab7ca4d0125ab12fb580bd6ab4f97d9ddfd88AC"},
                    {"path", "m/44'/0'/0'/0/0"},
                    {"address", "1ENJ8LYCcTA69PjkSbGoKjZV5d4r9ieH27" }
                },
                {
                    {"txId", "87aa3466539790d6aa30c225c3f408acb16a00135ec93d21ba70bca7d344ecf4"},
                    {"index", 2},
                    {"script", "76A91492a0aab7ca4d0125ab12fb580bd6ab4f97d9ddfd88AC"},
                    {"path", "m/44'/0'/0'/0/0"},
                    {"address", "1NrERKT8iV1GaCwEJtr1GjWfaoCpirWdnk" }
                }
            }
        },
        {
            "outputs", {
                {
                    {"value", 561000},
                    {"address", "1NrERKT8iV1GaCwEJtr1GjWfaoCpirWdnk" },
                    {"scriptPubKey", "" }
                },
                {
                    {"value", 200000000},
                    {"address", "3Hmnj95FwVWUNRx9i4ESbzUvPDAkobgUgw" },
                    {"scriptPubKey", "" }
                },
                {
                    {"value", 100000000},
                    {"address", "3Hmnj95FwVWUNRx9i4ESbzUvPDAkobgUgw" },
                    {"scriptPubKey", "" }
                }
            }
        },
        {"changePath", "m/44'/0'/0'/1/0"}
    };
    auto signed_tx = s300_mocker.sign_transaction(wallet::coin::btc, tx);
    std::string sign_id = signed_tx["id"];
    std::string sign_hex = signed_tx["hex"];
    XCTAssertTrue(sign_id == "710CF2EFEE5C8ACD1D9CBBCFB7A7EE8FBA717D6BB19D3DFCFDE1D60A9BCB1889");
    XCTAssertTrue(sign_hex == "0100000002DEBE94D3F0D478023FFDDEE9EE54477F736C5232C151E9BE00DE1B2CCA156047000000006A4730440220690FC37056A358C3A5B03F0375FF846CC9D48DB4BFF1AA782250D55812D213CE02201B52FC5EBF811F2DEF0717AED8F851EE63B4491D7E9B084F3D92F0FA061137400121029B7EF9A41F5DCF7C1EA17CFC2493C24F5204B2E11BA21662192749226C0210B2FDFFFFFFF4EC44D3A7BC70BA213DC95E13006AB1AC08F4C325C230AAD69097536634AA87020000006A4730440220690FC37056A358C3A5B03F0375FF846CC9D48DB4BFF1AA782250D55812D213CE02201B52FC5EBF811F2DEF0717AED8F851EE63B4491D7E9B084F3D92F0FA061137400121029B7EF9A41F5DCF7C1EA17CFC2493C24F5204B2E11BA21662192749226C0210B2FDFFFFFF03688F0800000000001976A914EFAA39B62C83DC14EFCC4B6161E47C90611C61BD88AC00C2EB0B0000000017A914B069138CEA150B811B75236D5450C5F184EF9C5C8700E1F5050000000017A914B069138CEA150B811B75236D5450C5F184EF9C5C8700000000");
}


//- (void)testEth {
//    nlohmann::json tx = {
//        {
//            "input", {
//                { "address", "0x1234567890123456789012345678901234567890" },
//                { "path", "m/44'/60'/0'/0/0" }
//            }
//        },
//        {   "output", {
//                { "address", "0x0987654321098765432109876543210987654321" },
//                { "value" , 0.518 * 100000000 }
//            }
//        },
//            
//        {   "nonce", 0 },
//        {   "gasPrice", 1000000000 },
//        {   "startGas", 21000 },
//        {   "gasLimit", 0 },
//        {   "data", "" }
//    };
//    
//    s300_mocker.init();
//    auto signed_tx = s300_mocker.sign_transaction(wallet::coin::eth, tx);
//}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
