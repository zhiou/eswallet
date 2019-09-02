//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012,深圳市文鼎创数据安全有限公司
// All rights reserved.
//
// 文件名称：esBLEManager.h
// 文件标识：
// 摘要：iOS的BLE Centeral类头文件
//
// 当前版本：0.9
// 作者：   lzm
// 完成日期：2014年11月03日
//
// 取代版本：
// 原作者：
// 完成日期：
//////////////////////////////////////////////////////////////////////////
#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import <CoreBluetooth/CBService.h>
#import "EsBLEKeyDefines.h"
#import "esBLEKey.h"
#import "esBLEKeyProtocol.h"



@interface EsBLEManager : NSObject <EsBLEManagerProtocol, CBCentralManagerDelegate>

@property (nonatomic,assign) id<EsBLEManagerDelegate> delegate;
@property (nonatomic,assign) id<EsScanDelegate> scanDelegate;
@property (strong, atomic)  NSMutableArray *esBleKeys;



@end
