//
//  AlgorithmTest.m
//  TransmitTests
//
//  Created by zhiou on 2019/9/2.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#import <XCTest/XCTest.h>

#import <configure/configure.hpp>

#import <wallet/wallet.hpp>
#import <json/json.hpp>
#import <wallet/path/path.hpp>
#import <wallet/fcbuffer/fcbuffer.hpp>
#import <wallet/s300.hpp>

#import <array>
#import <regex>

using namespace excelsecu;

using mock = wallet::s300;

@interface AlgorithmTest : XCTestCase
{
     mock s300_mocker;
}
@end

@implementation AlgorithmTest

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
//    configure::current().set_host_name("周煌的iphone");
//
//    s300_mocker.connect("hehe");
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
//    s300_mocker.disconnect();
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

- (void)testAddressBuffer {
    auto pubkey = bytestream("02B726B6894F7FCD89C52DD965F61DBA5B5C7E220FC9710E5F6EC267E17ADB2413");
    auto buf = excelsecu::wallet::address::from_buffer(wallet::coin::eos, pubkey);
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
//    auto account_name = s300_mocker.get_account_name(wallet::coin::eos, 0, "m/44'/1'/0'/0/0");
//    std::cout << account_name << std::endl;
//
//    s300_mocker.get_default_permission(wallet::coin::eos, 0);

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
    std::cout << "stx:" << std::endl;
    std::cout << signed_tx << std::endl;
}


- (void)testEth {
    nlohmann::json tx = {
        {
            "input", {
                { "address", "06b9d168c569b29e0728b7a1560d85123b622b9d" },
                { "path", "m/44'/60'/0'/0/0" }
            }
        },
        {   "output", {
                { "address", "06b9d168c569b29e0728b7a1560d85123b622b9d" },
                { "value" , 10000000000000000 }
            }
        },
        {   "nonce", 0 },
        {   "gasPrice", 2000000000 },
        {   "startGas", 21000 },
        {   "gasLimit", 0 },
        {   "data", "" }
    };
    
    s300_mocker.init();
    
    s300_mocker.select(wallet::coin::eth);
    
    auto ret = s300_mocker.get_wallet_info();
    std::cout << ret << std::endl;
    
    s300_mocker.get_pubkey(wallet::coin::eth, "m/44'/60'/0'/0/0");
    
    auto address = s300_mocker.get_address(wallet::coin::eth, "m/44'/60'/0'/0/0");
    std::cout << "ETH address: "<< address << std::endl;
//    XCTAssertTrue(address == "06b9d168c569b29e0728b7a1560d85123b622b9d");
    
    auto signed_tx = s300_mocker.sign_transaction(wallet::coin::eth, tx);
    std::string sign_id = signed_tx["id"];
    std::string sign_hex = signed_tx["hex"];
    XCTAssertTrue(bytestream(sign_id) == bytestream("11f46076d8390122b736fbebf49f514d93163b220fb72352acad2a7e4f3a0966"));
    XCTAssertTrue(bytestream(sign_hex).hex_str() == bytestream("f868808477359400809406b9d168c569b29e0728b7a1560d85123b622b9d872386f26fc10000802ca027915d275bb821af7c0ad622145571e964b6521b60f3de0ded56f3dff36afbf0a0412e897a21167e67e14520ca30f6e2b492ac0a37e4b537bb20092aff23cf9ef7").hex_str());
    std::cout << "stx:" << std::endl;
    std::cout << signed_tx << std::endl;
}

- (void)testEOS {
    nlohmann::json tx = {
        {"expiration", 1538569524},
        {"ref_block_num", 56170},
        {"ref_block_prefix", 3374189397},
        {"max_net_usage_words", 0},
        {"max_cpu_usage_ms", 0},
        {"delay_sec", 0},
        {"context_free_actions", {}},
        {"actions", {
            {
                {"account", "eosio.token"},
                {"name", "transfer"},
                {"authorization",{
                    {
                        {"actor", "inita"},
                        {"permission", "active"}
                    }
                }},
                {"data", "000000000093dd74000000008093dd747011010000000000045359530000000000"}
            }
        }},
        {"transaction_extensions" , {}},
        {"keyPaths", {
            "m/48'/4'/1'/0'/0'"
        }}
    };
    
    XCTAssertTrue(excelsecu::wallet::fcbuffer::serialize(tx) == bytestream("34b5b45b6adb550b1ec9000000000100a6823403ea3055000000572d3ccdcd01000000000093dd7400000000a8ed323221000000000093dd74000000008093dd74701101000000000004535953000000000000"));
    
    
     s300_mocker.init();
    
    s300_mocker.select(wallet::coin::eos);
    
    nlohmann::json tx_sign = {
        {"expiration", 1540211560},
        {"ref_block_num", 60832},
        {"ref_block_prefix", 2249681555},
        {"max_net_usage_words", 0},
        {"max_cpu_usage_ms", 0},
        {"delay_sec", 0},
        {"context_free_actions", {}},
        {"actions", {
            {
                {"account", "okkkkkkkkkkk"},
                {"name", "transfer"},
                {"authorization",{
                    {
                        {"actor", "bosbosbosbos"},
                        {"permission", "active"}
                    }
                }},
                {"data", "80e9c1f4607a303de04da299666f12cd640000000000000000424f53000000002a424f532043414d5041c4b0474e3a20596f752077696e2035303020424f532e20596f757265206c75636b"}
            }
        }},
        {"transaction_extensions" , {}},
        {"keyPaths", {
            "m/48'/4'/1'/0'/0'"
        }}
    };
    
    auto txabi = excelsecu::wallet::fcbuffer::serialize(tx_sign);
    std::cout << "txabi=" << txabi.hex_str() << std::endl;
    XCTAssertTrue(bytestream("68c3cd5ba0ed936a1786000000000100218410420821a4000000572d3ccdcd0180e9c1f4607a303d00000000a8ed32324b80e9c1f4607a303de04da299666f12cd640000000000000000424f53000000002a424f532043414d5041c4b0474e3a20596f752077696e2035303020424f532e20596f757265206c75636b00") == txabi);
    
    auto result = s300_mocker.sign_transaction(wallet::coin::eos, tx_sign);
    auto txId = result["txId"].get<std::string>();
    auto signature = result["signedTx"]["signatures"][0];
    
    XCTAssertTrue(txId == bytestream("7d4924cbd382cbb9fb4f260d4b0f272ebaadfeba566d54835c632de424a54884").hex_str());
    XCTAssert(signature == "SIG_K1_Jx7ZSuAgfSbGuAqYB34t57tGphR6Zb3AGTWCCsG8vaUg8PL4EvowtKteHM8Q7gocMjBxuUrVfK9GixBhbHrEHwHKPrR28P");
    
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
