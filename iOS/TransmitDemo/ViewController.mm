//
//  ViewController.m
//  TransmitDemo
//
//  Created by zhiou on 2019/8/9.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#import "ViewController.h"
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

#import "basic_file_sink.hpp"
#import "eslog.hpp"

using namespace excelsecu;

using mock_device = device<driver_mocker, configure, authenticator, handshake, transmit>;

@interface ViewController ()
{
    mock_device mocker;
}
@property (weak, nonatomic) IBOutlet UITextField *snTextField;
@property (weak, nonatomic) IBOutlet UILabel *randomLabel;
@property (nonatomic, strong) NSString* sn;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    NSString* hostName = [UIDevice currentDevice].name;
    configure::current().set_host_name(hostName.UTF8String);
    
    //    file_logger->info("%s %d", __FUNCTION__, __LINE__);
    LOG_DEBUG("hello %s %d times!", "world", 3000);
    enable_file_logger("eslog.txt");
    LOG_DEBUG("hello %s %d times!", "world", 3000);
    
    LOG_INFO("complete");
    
    [self testError];
    
}

- (IBAction)connect:(id)sender {
    NSString *sn = self.snTextField.text.length ? self.snTextField.text : self.snTextField.placeholder;
    mocker.connect(sn.UTF8String);
}

- (IBAction)getRandom:(id)sender {
    auto repo = mocker.send(bytestream("0084000008"));
    std::cout << "repo = " << repo << std::endl;
    self.randomLabel.text = [NSString stringWithUTF8String:repo.hex_str().c_str()];
}


- (void)testError {
    auto error = es_error("6FFB");
    try {
        throw error;
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
        throw es_error("user cancel", 0x6FFB, 0xE061);
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
@end
