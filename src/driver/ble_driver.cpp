//
//  ble_driver.cpp
//  transmit
//
//  Created by zhiou on 2019/8/6.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#include <iostream>
#include "ble_driver.hpp"
#include "EsDriverBle.h"

namespace excelsecu {

    bool ble_driver::isconnected()
    {
        u1 pu1Recv[0x10] = {0};
        u4 u4RecvLen = sizeof(pu1Recv);
        BlkPtlFunction ptlFunc;
        PtlGetFunctionBle(&ptlFunc);
        ptlFunc.m_funPtlInit();
        ptlFunc.m_funPtlIoControl((ESHANDLE)1, IOCTL_GETDEVICE_PLUG_STATE, nullptr, 0, pu1Recv, &u4RecvLen);
        return pu1Recv[0] == 1;
    }

    bool ble_driver::disconnect()
    {
        BlkPtlFunction ptlFunc;
        PtlGetFunctionBle(&ptlFunc);
        ptlFunc.m_funPtlInit();
        u4 ret = ptlFunc.m_funPtlDisconnectDev(nullptr);
        return ret == 0;
    }

    bool ble_driver::connect(const std::string &sn)
    {
        BlkPtlFunction ptlFunc;
        PtlGetFunctionBle(&ptlFunc);
        ptlFunc.m_funPtlInit();
        u4 ret = ptlFunc.m_funPtlConnectDev(sn.c_str(), nullptr);
        
        return ret == 0;
    }
    
    bool ble_driver::send(u1* pu1Send, u4 u4SendLen, u1* pu1Recv, u4* pu4RecvLen)
    {
        BlkPtlFunction ptlFunc;
        PtlGetFunctionBle(&ptlFunc);
        ptlFunc.m_funPtlInit();
        u4 ret = ptlFunc.m_funPtlSendApdu((ESHANDLE)1, pu1Send, u4SendLen, pu1Recv, pu4RecvLen);
        return ret == 0;
    }
    
    void ble_driver::set_callback(FunPtlDevChange callback)
    {
        BlkPtlFunction ptlFunc;
        PtlGetFunctionBle(&ptlFunc);
        ptlFunc.m_funPtlInit();
        
        ptlFunc.m_funPtlSetDeviceStateCallback(callback);
    }
}
