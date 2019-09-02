//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2012,深圳市文鼎创数据安全有限公司
// All rights reserved.
// 
// 文件名称： EsDriver.h
// 摘   要： 文鼎创移动平台驱动
//
// 当前版本： 1.0
// 作   者： lzm
// 更新日期： 2013-01-26
//
// 取代版本： 1.0
// 原作者 ：  lzm
// 完成日期： 2013-01-26
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _ESDRIVER_H_
#define _ESDRIVER_H_


#define ENABLE_AUDIODIRVER  0
#define ENABLE_BTMDIRVER    0
#define ENABLE_IDOCKDIRVER  0

#define ENABLE_BLEDIRVER    1


#ifndef _U1U2U4_
#define _U1U2U4_
typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int  u4;
typedef unsigned long long  u64t;

typedef signed char s1;
typedef short s2;
typedef int  s4;
typedef signed long long    s64t;

typedef void*				ESHANDLE;
#endif


#ifndef ESDRIVER_TYPEDEF
#define ESDRIVER_TYPEDEF

//#define ESHANDLE  void*
#define CALL_TYPE


#ifndef __PROTOCAL__
#define __PROTOCAL__

#define AUDIO_PROTOCAL      5
#define IDOCK_PROTOCAL      6
#define BT_PROTOCAL         7

#define BLE_PROTOCAL        8
#endif



#define IOCTL_GETDEVICE_PLUG_STATE      1 //输出 01 00 00 00表示存在，  输出 00 00 00 00表示不存在

#define	DRV_DEV_CHANGE_INSERT		0x01
#define	DRV_DEV_CHANGE_REMOVE		0x02



#if defined(__cplusplus)
extern "C"
{
#endif

    
typedef u4 (CALL_TYPE *FunPtlInit)(void);
typedef u4 (CALL_TYPE *FunPtlFinal)(void);
typedef u4 (CALL_TYPE *FunPtlCheckDevName)(const char *szDevName, u2 *pu2Vid, u2 *pu2Pid);
typedef u4 (CALL_TYPE *FunPtlListCallback)(const char *szDevName, u4 u4DevType);
typedef u4 (CALL_TYPE *FunPtlListDev)(FunPtlListCallback funCallback);

typedef u4 (CALL_TYPE *FunPtlConnectDev)(const char *szDevName, ESHANDLE *phDev);
typedef u4 (CALL_TYPE *FunPtlDisconnectDev)(ESHANDLE hDev);
typedef u4 (CALL_TYPE *FunPtlSendApdu)(ESHANDLE hDev, const u1 *pu1Send, u4 u4SendLen, u1 *pu1Recv, u4 *pu4RecvLen);
typedef u4 (CALL_TYPE *FunPtlResetDev)(ESHANDLE hDev, u1 *pu1Atr, u4 *pu4AtrLen);
typedef u4 (CALL_TYPE *FunPtlIoControl)(ESHANDLE hDev, u4 u4IOCode, const u1 *pu1Send, u4 u4SendLen, u1 *pu1Recv, u4 *pu4RecvLen);
    
    
typedef u4 (*FunPtlDevChange)(u4 u4SlotId, u4 u4DevChangeType);

// 新加设置回调接口，不推荐使用IoControl来设置,lzw
typedef u4 (CALL_TYPE *FunPtlSetDeviceStateCallback)(FunPtlDevChange callback);
    
    
typedef struct BlkPtlFunctionTag
{
    FunPtlInit                  m_funPtlInit;
    FunPtlFinal                 m_funPtlFinal;
    FunPtlCheckDevName          m_funPtlCheckDevName;
    FunPtlListDev               m_funPtlListDev;
    FunPtlConnectDev            m_funPtlConnectDev;
    FunPtlDisconnectDev         m_funPtlDisconnectDev;
    FunPtlResetDev              m_funPtlResetDev;
    FunPtlSendApdu              m_funPtlSendApdu;
    FunPtlIoControl             m_funPtlIoControl;
    
    // new add
    FunPtlSetDeviceStateCallback m_funPtlSetDeviceStateCallback;
}BlkPtlFunction;
    
#endif
    
#if defined(__cplusplus)
}
#endif


#endif

