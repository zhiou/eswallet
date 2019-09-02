//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012,深圳市文鼎创数据安全有限公司
// All rights reserved.
//
// 文件名称：esBLEKey.h
// 文件标识：
// 摘要：iOS的BLE device类头文件
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
#import "esBLEKeyProtocol.h"


#define  KEYOBJ_RECV_BUFFER_SIZE  4096  // 必须取2^n

#ifdef __cplusplus
extern "C" {
#endif
    unsigned int EsBLEKeySendDataSub(id hParam, unsigned char *pu1Data, int nDataLen);
#ifdef __cplusplus
}
#endif

@interface ESBLEKey : NSObject <EsBLEKeyProtocol, CBPeripheralDelegate>
{
    unsigned char m_u1RecvBuffer[KEYOBJ_RECV_BUFFER_SIZE];
}



@property (nonatomic, assign) id <EsBLEKeyDelegate> delegate;
@property (strong)   CBPeripheral *activePeripheral;
@property (atomic)   BleConnectStatus  m_nConnectStatus;
@property (atomic)   unsigned long  m_u4NotifyFlag;
@property (atomic)   unsigned int   m_u4ConnectID;
@property (atomic)   unsigned int  m_u4RecvBuffRdInx;
@property (atomic)   unsigned int  m_u4RecvBuffWrInx;
@property (atomic)   unsigned int  m_u4ErrorFlag;
@property (atomic)   unsigned char  m_u1HasPairingOperation;
@property (atomic)   unsigned char  m_u1KeyType;
@property (atomic)   unsigned int  m_u1IsISSCService;

@property (nonatomic, copy) NSString*  serviceUUID;
@property (nonatomic, copy) NSString*  txUUID;
@property (nonatomic, copy) NSString*  rxUUID;

@property (nonatomic) BOOL m_bSystemPairSuccess;
@property (nonatomic,assign,getter = isInteractive)   BOOL interactive;
@property (nonatomic,assign,getter = isRefusePairing)   BOOL refusePairing;
@property (atomic)   unsigned int  m_u4NameAck;


-(unsigned int) writeValue:(NSString*)serviceUUID characteristicUUID:(NSString*)characteristicUUID p:(CBPeripheral *)p data:(NSData *)data ;
-(void) NotifyNameAck;
-(void) SendName;

-(void) OnDisconnect: (id <EsBLEManagerProtocol>) keyMgr;
-(void) OnConnecting: (CBPeripheral *)peripheral Delegate: (id <EsBLEKeyDelegate>) EsBleKeyDelegate;
-(void) OnConnected: (id <EsBLEManagerProtocol>) keyMgr;
-(void) OnConnectFailed: (id <EsBLEManagerProtocol>) keyMgr;

- (void) ClearRecvBuffer;
- (int)  GetRecvData: (unsigned char*) pu1Recv  BufferSize: (int) nBufferSize;
- (void) EnqueueRecvData:(unsigned char*) pu1Recv  RecvDataLen: (int) nLen;

@end


@interface CBPeripheral (ConnectionState)
-(BOOL) connected;
-(NSString *)uuid;
-(BOOL) isSystemPaired;
-(void) setSystemPaired:(BOOL)pair;
@end

