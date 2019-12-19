//
//  ITransmit.h
//  SDK
//
//  Created by Huang Zhou on 2018/3/28.
//  Copyright © 2018年 excelsecu. All rights reserved.
//

#ifndef ITransmit_h
#define ITransmit_h

/**
 C层对外提供的抽象类，用于发送明文或密文Apdu指令，隐藏内部实现
 */
class ITransmit {
public:
    
    static ITransmit* getInstance();
    
public:
    virtual ~ITransmit(){}
    
    // 内部已处理90XX指令
    virtual unsigned int operator()(const unsigned char* pu1Send, unsigned int u4SendLen, unsigned char* pu1Recv, unsigned int* pu4RecvLen, bool bEncrypted = false) = 0;
protected:
    static ITransmit *m_transmit;
  
};

// 提供简写，便于调用，内部处理90xx指令
#define Transmit(...) ((*ITransmit::getInstance())(__VA_ARGS__))


#endif /* ITransmit_h */
