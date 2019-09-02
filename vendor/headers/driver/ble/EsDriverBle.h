//
//  EsDriverBle.h
//  iOSBLEDriver
//
//  Created by 陆振文 on 14-12-8.
//  Copyright (c) 2014年 文鼎创. All rights reserved.
//

#include <stdio.h>

#define IMPL

#import "EsDriver.h"


// 设置BLE连接超时时间，单位是秒
#define IOCTL_SET_CONNECT_TIMEOUT      0x0000e024

// 判断BLE设备连接后是否触发了系统配对操作
#define IOCTL_GET_PAIRING_OPERATION    0x0000e025

// 发送裸数据，不是指令
#define IOCTL_SEND_RAW_DATA            0x0000e026

typedef enum EsPairingOperation {
    SYSTEM_PAIRING_UNKNOWN = 0,   // 未知
    SYSTEM_PAIRING_NOT_TRIGGERED, // 未触发系统配对
    SYSTEM_PAIRING_TRIGGERED     // 触发了系统配对
} EsPairingOperation;

#if defined(__cplusplus)
extern "C"
{
#endif
    
u4 CALL_TYPE PtlGetFunctionBle(BlkPtlFunction *pblkPtlFunction);
    
#if defined(__cplusplus)
}
#endif
