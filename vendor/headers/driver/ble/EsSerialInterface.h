//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012,深圳市文鼎创数据安全有限公司
// All rights reserved.
//
// 文件名称：EsSerialInterface.h
// 文件标识：
// 摘要：Es串口协议接口头文件
//
// 当前版本：0.9
// 作者：   lzm
// 完成日期：2014年11月03日
//
// 取代版本：
// 原作者：
// 完成日期：
//////////////////////////////////////////////////////////////////////////

#ifndef _ESSERIALINTERFACE_H_
#define _ESSERIALINTERFACE_H_


#ifdef __cplusplus
extern "C" {
#endif
    
/*
 * 函数：EsSerialDriverCreate
 * 功能：创建串口协议驱动类对象
 * 参数：
 * 返回：串口协议驱动类指针
 */
void *EsSerialDriverCreate();
    
/*
 * 函数：EsSerialDriverRelease
 * 功能：释放串口协议驱动类对象
 * 参数：
 *    pEsSerial          [IN ]  串口协议驱动类对象
 * 返回：
 */
void  EsSerialDriverRelease(void *pEsSerial);

/*
 * 函数：EsSerialSetDeviceHandle
 * 功能：关联设备对象的指针
 * 参数：
 *    pEsSerial          [IN ]  串口协议驱动类对象
 *    pvParam            [IN ]  关联设备对象的指针
 * 返回：
 */
void  EsSerialSetDeviceHandle(void *pEsSerial, void* pvParam);
    
/*
 * 函数：EsSerialSendApdu
 * 功能：执行APDU
 * 参数：
 *    pEsSerial          [IN ]  串口协议驱动类对象
 *    pu1Data            [IN ]  APDU命令
 *    u4DataLen          [IN ]  APDU命令长度
 *    pu1Recv            [IN ]  响应数据缓冲区
 *    pu4RecvLen         [OUT]  响应数据长度
 * 返回：0--成功，其它--错误码
 *    1) 响应数据缓冲区大小应该要足够大，要容纳响应数据长度（包括SW1SW2)，目前支持到2KB的长指令
 *    2) 命令为1字节的0xA4表示resetcos
 */
unsigned int EsSerialSendApdu(void *pEsSerial, unsigned char *pu1Data, unsigned int u4DataLen, unsigned char *pu1Recv, unsigned int *pu4RecvLen);
  
    
/*
 * 函数：EsSerialSendData
 * 功能：发送原生数据
 * 参数：
 *    pEsSerial          [IN ]  串口协议驱动类对象
 *    pu1Data            [IN ]  APDU命令
 *    u4DataLen          [IN ]  APDU命令长度
 *    pu1Recv            [IN ]  响应数据缓冲区
 *    pu4RecvLen         [OUT]  响应数据长度
 * 返回：0--成功，其它--错误码
 */
unsigned int EsSerialSendData(void *pEsSerial, unsigned char *pu1Data, unsigned int u4DataLen, unsigned char *pu1Recv, unsigned int *pu4RecvLen);
    
#ifdef __cplusplus
}
#endif

#endif
