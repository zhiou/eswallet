#ifndef _ES_ENCODE_DECODE_H_
#define _ES_ENCODE_DECODE_H_

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 *  @brief Base64编码
	 *  
	 *  @param[in] pu1Src 待编码数据
	 *  @param[in] u4SrcLen 待编码数据长度>0
	 *  @param[out] pszBase64code 存储编码结果
	 *  @param[in] u4Base64BufLen pszBase64code缓冲区长度，必须>=u4SrcLen*4/3
	 *  
     *  @return 编码后的长度，失败时返回0
     *
     *  @details none
	 */
	u4 CALL_TYPE EsBase64Encode(IN const u1* pu1Src, IN u4 u4SrcLen, OUT char* pszBase64code, IN u4 u4Base64BufLen);

	/**
	 *  @brief Base64解码
	 *  
	 *  @param[in] pszBase64code Base64编码格式的数据
	 *  @param[in] u4Base64CodeLen Base64编码格式的数据长度
	 *  @param[out] pu1Buf 存储解码结果，缓冲区大小必须保证>=u4Base64CodeLen*3/4
	 *  
     *  @return 解码后的长度，失败时返回0
     *
     *  @details none
	 */
	u4 CALL_TYPE EsBase64Decode(IN const char* pszBase64code, IN u4 u4Base64CodeLen, OUT u1* pu1Buf);

	/**
	 *  @brief Base58编码
	 *  
	 *  @param[in] pu1Data 待编码数据
	 *  @param[in] u4DataLen 待编码数据长度,>0
	 *  @param[out] pszEncode 存储编码结果
	 *  @param[in,out] pu4EncodedLen 入参为pszBase64code缓冲区长度，出参为编码结果长度
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details TODO!!暂时不支持pszEncode传入NULL求编码长度
	 */
	u4 CALL_TYPE EsBase58Encode(IN const u1* pu1Data, IN u4 u4DataLen, OUT char* pszEncode, INOUT u4* pu4EncodedLen);

	/**
	 *  @brief 对指定数据进行DER编码
	 *  
	 *  @param[in] u4Tag Tag，即T(1字节)
	 *  @param[in] pvValue Value，即V，如果u4Tag=0x06(OID)，则为字符串，忽略u4ValueLen
	 *  @param[in] u4ValueLen Len，即L，编码后最大长度可能为6字节(85 aa bb cc dd ee)
	 *  @param[out] pu1Der 输出编码结果，不允许为NULL，缓冲区长度保证>=7+u4ValueLen
	 *  
	 *  @return 编码后的数据长度
     *
     *  @details TODO!!暂时不支持pu1Der传入NULL求编码长度
	 */
	u4 CALL_TYPE EsDerEncode(IN u4 u4Tag, IN const void* pvValue, IN u4 u4ValueLen, OUT u1* pu1Der);

	/**
	 *  @brief 获取DER编码元素的大小，含TLV
	 *  
	 *  @param[in] pu1Der DER编码的数据，=NULL时返回0
	 *  @param[out] pu4LenLen 输出元素大小，=NULL时返回0
	 *  
	 *  @return 元素大小
     *
     *  @details TODO这个函数默认TAG只有一个字节
	 */
	u4 CALL_TYPE EsDerGetElementLen(IN const u1* pu1Der, OUT u4* pu4LenLen);

	/**
	 *  @brief 获取DER编码数据的T、L的值以及T、L的编码长度
	 *  
	 *  @param[in] pu1DerData DER编码的数据，不允许为NULL
	 *  @param[in] u4DerDataLen DER编码的数据长度，必须>=有效的DER编码数据长度
	 *  @param[out] pu2Tag T的值，可以为NULL
	 *  @param[out] pu4ValueLen L的值，可以为NULL
	 *  @param[out] pu4TagLenSize T、L编码后的长度，可以为NULL
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsDerGetTagAndLen(IN const u1* pu1DerData, IN u4 u4DerDataLen, OUT u2* pu2Tag, OUT u4* pu4ValueLen, OUT u4* pu4TagLenSize);
	
	/**
	 *  @brief 忽略指定的TLV移位指针到下一个TLV
	 *  
	 *  @param[in,out] ppu1Data DER编码的数据指针的地址，不允许为NULL
	 *  @param[in,out] pu4DataLen 入参为*ppu1Data数据长度，出参为移位后剩下的数据长度
	 *  @param[in] u2ExpectTag 指定的T
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsDerSkipTlv(INOUT u1** ppu1Data, INOUT u4* pu4DataLen, u2 u2ExpectTag);
    
    /**
     *  @brief 公钥DER编码
     *
     *  @param[in] pblkAsymPubKey 公钥
     *  @param[out] pu1PubKeyEncoded 存储编码后的公钥，支持传入NULL求长度
     *  @param[in,out] pu4PubKeyEncodedLen 入参为pu1PubKeyEncoded缓冲区大小，出参为编码后公钥长度
     *
     *  @details none
     */
	void CALL_TYPE EsDerEncodePubKey(IN const BlkAsymKey *pblkAsymPubKey, OUT u1* pu1PubKeyEncoded, INOUT u4 * pu4PubKeyEncodedLen);
    
    /**
     *  @brief 公钥DER解码
     *
     *  @param[in] pu1PubKeyEncoded DER编码公钥
     *  @param[in] u4PubKeyEncodedLen DER编码公钥长度
     *  @param[out] pblkAsymKey 解码后的公钥
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 仅支持rsa和sm2!!!而且会把模长为32的ecc算法当作sm2!!!
     */
	u4 CALL_TYPE EsDerDecodePubKey(IN const u1* pu1PubKeyEncoded, IN u4 u4PubKeyEncodedLen, OUT BlkAsymKey * pblkAsymKey);
    
    /**
     *  @brief 公钥DER解码
     *
     *  @param[in] pu1PubKeyEncoded DER编码公钥
	 *  @param[in] u4PubKeyEncodedLen DER编码公钥长度
	 *  @param[in] u4AsymAlgId 已知的非对称算法Id
     *  @param[out] pblkAsymKey 解码后的公钥
     *
     *  @return 0-成功 other-失败，详见EsError.h
     */
	u4 CALL_TYPE EsDerDecodePubKeyEx(IN const u1* pu1PubKeyEncoded, IN u4 u4PubKeyEncodedLen, u4 u4AsymAlgId, OUT BlkAsymKey * pblkAsymKey);

    /**
     *  @brief 私钥DER解码
     *
     *  @param[in] pblkAsymKey 私钥
     *  @param[out] pu1DerPriKey 存储DER编码后的私钥，支持传入NULL求长度
     *  @param[out] pu4DerPriKeyLen 入参为pu1DerPriKey缓冲区大小，出参为编码后私钥长度
     *
     *  @details none
     */
	void CALL_TYPE EsDerEncodePriKey(IN const BlkAsymKey* pblkAsymKey, OUT u1* pu1DerPriKey, INOUT u4* pu4DerPriKeyLen);
    
    /**
     *  @brief 私钥DER解码
     *
     *  @param[in] pu1DerPriKey DER编码私钥
     *  @param[in] u4DerPriKeyLen DER编码私钥长度
     *  @param[out] pblkAsymKey 解码后的私钥
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 仅支持rsa和sm2!!!而且会把模长为32的ecc算法当作sm2!!!
     */
	u4 CALL_TYPE EsDerDecodePriKey(IN const u1* pu1DerPriKey, IN u4 u4DerPriKeyLen, OUT BlkAsymKey* pblkAsymKey);
    
	/**
     *  @brief SM2签名值DER编码
     *
     *  @param[in] pu1Sm2Signature SM2签名值 R||S，大头序
     *  @param[in] u4SignatureLen SM2签名值长度，64字节
     *  @param[out] pu1DerSignature 存储编码后的签名值，支持传入NULL求长度
     *  @param[in,out] pu4DerSignatureLen 入参为pu1DerSignature缓冲区大小，出参为编码结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsDerEncodeSm2Signature(IN const u1* pu1Sm2Signature, IN u4 u4SignatureLen, OUT u1* pu1DerSignature, INOUT u4* pu4DerSignatureLen);
    
    /**
     *  @brief SM2签名值DER编码
     *
     *  @param[in] pu1DerSignature 编码后的签名值
     *  @param[in] u4DerSignatureLen 签名值长度
     *  @param[out] pu1Signature SM2签名值 R||S，大头序，支持传入NULL求长度
     *  @param[in,out] pu4SignatureLen 入参为pu1Signature缓冲区大小，出参为解码后的长度64字节
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsDerDecodeSm2Signature(IN const u1* pu1DerSignature, IN u4 u4DerSignatureLen, OUT u1* pu1Signature, INOUT u4* pu4SignatureLen);

    /**
     *  @brief ECC签名值DER编码（也支持SM2）
     *
     *  @param[in] pu1Signature ECC签名值 R||S，大头序
     *  @param[in] u4SignatureLen ECC签名值长度，模长2倍
     *  @param[out] pu1DerSignature 存储编码后的签名值，支持传入NULL求长度
     *  @param[in,out] pu4DerSignatureLen 入参为pu1DerSignature缓冲区大小，出参为编码结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsDerEncodeEccSignature(IN const u1* pu1Signature, IN u4 u4SignatureLen, OUT u1* pu1DerSignature, INOUT u4* pu4DerSignatureLen);
    
    /**
     *  @brief ECC签名值DER编码
     *
     *  @param[in] pu1DerSignature 编码后的签名值
	 *  @param[in] u4DerSignatureLen 编码后的签名值长度
     *  @param[out] pu1Signature ECC签名值 R||S，大头序，支持传入NULL求长度
     *  @param[in,out] pu4SignatureLen 入参为pu1Signature缓冲区大小，出参为R|S的长度，R、S长度一样
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 注意！签名值长度不保证是模长的2倍，在使用签名值前，调用者需要自行根据模长在R和S前面分别补00补足模长
     */
	u4 CALL_TYPE EsDerDecodeEccSignature(IN const u1* pu1DerSignature, IN u4 u4DerSignatureLen, OUT u1* pu1Signature, INOUT u4* pu4SignatureLen);

    /**
     *  @brief ECC签名值DER编码
     *
     *  @param[in] pu1DerSignature 编码后的签名值
	 *  @param[in] u4DerSignatureLen 编码后的签名值长度
	 *  @param[in] u4ModLen 签名值长度
     *  @param[out] pu1Signature ECC签名值 R||S，大头序，支持传入NULL求长度
     *  @param[in,out] pu4SignatureLen 入参为pu1Signature缓冲区大小，出参为解码后的2*u4ModLen
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 调用者保证u4ModLen的合法性，接口内部也会检查
     */
	u4 CALL_TYPE EsDerDecodeEccSignatureEx(IN const u1* pu1DerSignature, IN u4 u4DerSignatureLen, IN u4 u4ModLen, OUT u1* pu1Signature, INOUT u4* pu4SignatureLen);

#ifdef __cplusplus
};
#endif

#endif
