//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012,深圳市文鼎创数据安全有限公司
// All rights reserved.
//
// 文件名称：esBLEKeyProtocol.h
// 文件标识：
// 摘要：iOS的BLE Centeral/Device接口协议头文件
//
// 当前版本：0.9
// 作者：   lzm
// 完成日期：2014年11月03日
//
// 取代版本：
// 原作者：
// 完成日期：
//////////////////////////////////////////////////////////////////////////
#ifndef _ESBLEKEYPROTOCOL_H_
#define _ESBLEKEYPROTOCOL_H_

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>



typedef NS_ENUM(unsigned int, BleConnectStatus){
    BleConnectStatusUnknown =  0,  // 未连接的初始状态或连接断开／连接失败后的重置状态
    BleDisconnected         =  1,  // 连接断开
    BleConnectFailed        =  2,  // 连接失败
    BleConnecting           =  3,  // 正在连接
    BleConnected            =  4,  // 已连接
};

typedef NS_ENUM(unsigned int, BleScanStatus) {
    BleScanning = 0,
    BleScanned = 1,
    BleScanTimeout = 2
};

@protocol EsBLEManagerProtocol;


// BLE Manager的委托协议
@protocol EsBLEManagerDelegate <NSObject>

@optional

/*
 * 函数：centralManagerDidUpdateState
 * 功能：通知BLE Manager状态改变。可以根据BLE Manager状态判断蓝牙是否打开使能
 * 参数：
 *     central             [IN ]  Coreblue的CBCentralManager对象
 * 返回：无
 */
- (void)centralManagerDidUpdateState:(CBCentralManager *)central;

- (void)managerDidStartDiscover:(id<EsBLEManagerProtocol>)mgr;
- (void)managerDidEndDiscover:(id<EsBLEManagerProtocol>)mgr;

@required


/*
 * 函数：didDiscoverPeripheral
 * 功能：通知扫描到的BLE设备信息
 * 参数：
 *     peripheral             [IN ]  BLE设备对象
 *     advertisementData      [IN ]  设备广播数据
 *     RSSI                   [IN ]  设备信号强度
 * 返回：无
 */
- (void) didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI;

@end

// BLE Key的协议
@protocol EsBLEKeyProtocol <NSObject>

@optional

-(unsigned char) GetPairingOperation;   // BLE设备连接后是否进行了配对操作，00--未知，01--已经配对过，02--key进行了配对流程
@required

-(CBPeripheral *)   peripheral;         // BLE设备对象
-(BleConnectStatus) connectStatus;      // BLE设备连接状态
-(unsigned int) connectID;              // BLE设备连接ID
-(BOOL) isInteractive;                  // 判断BLE是否可通讯，收发数据

@end

@protocol EsBLEKeyDelegate <NSObject>


@optional
-(void)keyConnecting:(id<EsBLEKeyProtocol>) key;

@required
/*
 * 函数：keyConnected
 * 功能：通知BLE设备已连接
 * 参数：
 *     esBleKey             [IN ]  EsBLEKeyProtocol 对象
 * 返回：无
 */
-(void) keyConnected:(id<EsBLEKeyProtocol>) esBleKey;
/*
 * 函数：keyConnectFailed
 * 功能：通知BLE设备连接失败
 * 参数：
 *     esBleKey             [IN ]  EsBLEKeyProtocol 对象
 * 返回：无
 */
-(void) keyConnectFailed:(id<EsBLEKeyProtocol>) esBleKey;
/*
 * 函数：keyDisconnected
 * 功能：通知BLE设备断开连接
 * 参数：
 *     esBleKey             [IN ]  EsBLEKeyProtocol 对象
 * 返回：无
 */
-(void) keyDisconnected:(id<EsBLEKeyProtocol>) esBleKey;
@end


@protocol EsScanDelegate <NSObject>

- (void)keyScanned:(CBPeripheral *)p advertisement:(NSDictionary *)advertise;

@end

// BLE Manager 接口协议
@protocol EsBLEManagerProtocol <NSObject>

@required

@property (strong, atomic)  CBCentralManager *CM;  // Coreblue的CBCentralManager对象


//-(NSArray *)retrieveConnectedPeripheralsWithServices:(NSArray *)cbuuidObjects;
//
//-(NSArray *)retrievePeripheralsWithIdentifiers:(NSArray *)nsuuidObjects;

/*
 * 函数：controlSetup
 * 功能：初始化CoreBluetooths Central Manager，如果系统蓝牙未打开，会弹出是否设置蓝牙的对话框.
 * 参数：
 * 返回：0
 */
-(int)  controlSetup:(id <EsBLEManagerDelegate>) Delegate;

/*
 * 函数：scanBLEKey
 * 功能：扫描BLE key，只扫描特定的service的BLE设备
 * 参数：
 *    timeout             [IN ]  扫描时间
 *    bAllowDuplicatesKey [IN ]  设置为YES则不管是否是已扫描到的设备，都会触发didDiscoverPeripheral，这样可以跟踪到RSSI的变化
 *                              设置为NO则每个扫描到的设备只触发1次didDiscoverPeripheral
 *    Delegate            [IN ]  EsBLEManagerDelegate委托对象
 * 返回：无
 */
-(int)  scanBLEKey:(NSTimeInterval) timeout
                   AllowDuplicatesKey:(BOOL) bAllowDuplicatesKey;

/*
 * 函数：stopscan
 * 功能：停止扫描
 * 参数：
 * 返回：无
 */
-(void) stopScan;

/*
 * 函数：connectPeripheral
 * 功能：连接BLE key
 * 参数：
 *    peripheral          [IN ]  BLE设备对象
 *    Delegate            [IN ]  EsBLEKeyDelegate委托对象
 *    bNotifyOnDisconnect [IN ]  App在后台时，设备断开是否弹出提示对话框
 * 返回：无
 */
-(void) connectPeripheral:(CBPeripheral *)peripheral
                            Delegate:(id <EsBLEKeyDelegate>) Delegate
                            NotifyOnDisconnect:(BOOL) bNotifyOnDisconnect;

/*
 * 函数：cancelPeripheralConnection
 * 功能：断开BLE key
 * 参数：
 *    peripheral          [IN ]  BLE设备对象
 * 返回：无
 */
-(void) cancelPeripheralConnection:(CBPeripheral *)peripheral;
                                                               
@end


#ifdef __cplusplus
extern "C" {
#endif
// 获取BLE Manager协议实现类
id<EsBLEManagerProtocol>  EsBLEGetManagerProtocol(void);
#ifdef __cplusplus
}
#endif


#endif
