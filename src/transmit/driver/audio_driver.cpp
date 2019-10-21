//
//  audio_driver.cpp
//  transmit
//
//  Created by zhiou on 2019/8/6.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#include "audio_driver.hpp"

#include "EsDriver.h"

namespace excelsecu {

bool audio_driver::isconnected() {
  return true;
  //        u1 pu1Recv[0x10] = {0};
  //        u4 u4RecvLen = sizeof(pu1Recv);
  //        BlkPtlFunction ptlFunc;
  //        PtlGetFunction(&ptlFunc, AUDIO_PROTOCAL);
  //        ptlFunc.m_funPtlInit();
  //        ptlFunc.m_funPtlIoControl((ESHANDLE)1, IOCTL_GETDEVICE_PLUG_STATE,
  //        nullptr, 0, pu1Recv, &u4RecvLen); return pu1Recv[0] == 1;
}

bool audio_driver::disconnect() {
  return true;
  //        BlkPtlFunction ptlFunc;
  //        PtlGetFunction(&ptlFunc, AUDIO_PROTOCAL);
  //        ptlFunc.m_funPtlInit();
  //        u4 ret = ptlFunc.m_funPtlDisconnectDev(nullptr);
  //        return ret == 0;
}

bool audio_driver::connect(const std::string &sn) {
  return true;
  //        BlkPtlFunction ptlFunc;
  //        PtlGetFunction(&ptlFunc, AUDIO_PROTOCAL);
  //        ptlFunc.m_funPtlInit();
  //        u4 ret = ptlFunc.m_funPtlConnectDev(sn.c_str(), nullptr);
  //        return ret == 0;
}

bool audio_driver::send(u1 *pu1Send, u4 u4SendLen, u1 *pu1Recv,
                        u4 *pu4RecvLen) {
  return true;
  //        BlkPtlFunction ptlFunc;
  //        PtlGetFunction(&ptlFunc, AUDIO_PROTOCAL);
  //        ptlFunc.m_funPtlInit();
  //        u4 ret = ptlFunc.m_funPtlSendApdu((ESHANDLE)1, pu1Send, u4SendLen,
  //        pu1Recv, pu4RecvLen); return ret == 0;
}

void audio_driver::set_callback(FunPtlDevChange callback) {
  //        BlkPtlFunction ptlFunc;
  //        PtlGetFunction(&ptlFunc, AUDIO_PROTOCAL);
  //        ptlFunc.m_funPtlInit();
  //
  //        u4 ret = ptlFunc.m_funPtlSetDeviceStateCallback(callback);
}
} // namespace excelsecu
