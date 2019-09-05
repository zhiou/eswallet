//
//  MsgQueueTest.m
//  TransmitTests
//
//  Created by zhiou on 2019/9/3.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "message_queue.hpp"

using namespace excelsecu;

static void onForeground(bool _isforeground)
{
    NSLog(@"onForeground");
}

@interface MsgQueueTest : XCTestCase

@end

@implementation MsgQueueTest

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
    bool isforeground = true;
    if (get_def_message_queue() == current_thread_message_queue())
    {
        async_invoke(std::bind(&onForeground, isforeground));
    }
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    cancel_message(def_async_invoke_handler());
    wait_for_running_lock_end(def_async_invoke_handler());
}

- (void)testExample {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
    
    
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
