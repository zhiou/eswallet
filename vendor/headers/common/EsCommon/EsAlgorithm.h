#ifndef _ES_ALGORITHM_H_
#define _ES_ALGORITHM_H_

#ifdef __cplusplus
extern "C" 
{
#endif

	////////////////////////////////////////////////////////////
	// Hash algorithm
	////////////////////////////////////////////////////////////

	#define	ES_HASH_ALG_MD5 			0x80000001
	#define	ES_HASH_ALG_SHA1			0x80000002
	#define	ES_HASH_ALG_SHA256			0x80000003
	#define	ES_HASH_ALG_SHA384			0x80000004
	#define	ES_HASH_ALG_SHA512			0x80000005
	#define	ES_HASH_ALG_SM3 			0x80000006
	#define	ES_HASH_ALG_MD5SHA1 		0x80000007
	#define	ES_HASH_ALG_RIPEMD160		0x80000008
    // SHA3基于keccak算法，但2015年8月NIST完成SHA3标准化时调整了填充算法
    #define ES_HASH_ALG_SHA3224         0x80000009
    #define ES_HASH_ALG_SHA3256         0x8000000A
    #define ES_HASH_ALG_SHA3384         0x8000000B
    #define ES_HASH_ALG_SHA3512         0x8000000C
    #define ES_HASH_ALG_KECCAK224       0x8000000D
    #define ES_HASH_ALG_KECCAK256       0x8000000E
    #define ES_HASH_ALG_KECCAK384       0x8000000F
    #define ES_HASH_ALG_KECCAK512       0x80000010
	#define ES_HASH_ALG_SET				0x800000FF	// 只能通过设置接口设置哈希值

    /**
	 *  @brief Hash-初始化
	 *
	 *  @param[out] phHash 存储Hash句柄
	 *  @param[in] pvIv 初始值，可以为NULL，有效数据长度与Hash算法结果长度一致
	 *  @param[in] u4HashAlgId 算法Id，详见ES_HASH_ALG_XXX
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashInit(OUT ESHANDLE* phHash, IN const void* pvIv, IN u4 u4HashAlgId);
    
    /**
	 *  @brief Hash-多分组操作
	 *
	 *  @param[in] hHash Hash句柄
	 *  @param[in] pvInput 待操作数据
	 *  @param[in] u4InputLen 待操作数据长度
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashUpdate(IN ESHANDLE hHash, IN const void *pvInput, IN u4 u4InputLen);
    
    /**
	 *  @brief Hash-结束运算
	 *
	 *  @param[in] hHash Hash句柄
	 *  @param[out] pvHashValue 存储操作结果，缓冲区长度可以调用EsHashGetInfo获取
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashFinal(IN ESHANDLE hHash, OUT void* pvHashValue);
    
    /**
	 *  @brief Hash-释放句柄
	 *
	 *  @param[in] hHash Hash句柄
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashRelease(IN ESHANDLE hHash);
    
    /**
	 *  @brief Hash-获取信息
	 *
	 *  @param[in] hHash Hash句柄
	 *  @param[out] pu4BlockSize 分组大小，可以为NULL
	 *  @param[out] pu4IvSize IV大小，可以为NULL
	 *  @param[out] pu4ValueSize 结果大小，可以为NULL
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
    u4 CALL_TYPE EsHashGetInfo(IN ESHANDLE hHash, OUT u4* pu4BlockSize, OUT u4* pu4IvSize, OUT u4* pu4ValueSize);
    u4 CALL_TYPE EsHashGetParam(IN u4 u4AlgId, OUT u4* pu4BlockSize, OUT u4* pu4IvSize, OUT u4* pu4ValueSize);
    
    /**
	 *  @brief Hash-获取句柄对应的Hash算法Id
	 *
	 *  @param[in] hHash Hash句柄
	 *  @param[out] pu4AlgId 算法Id，不允许为NULL
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashGetAlgId(IN ESHANDLE hHash, u4* pu4AlgId);

    /**
	 *  @brief Hash-获取中间值
	 *
	 *  @param[in] hHash Hash句柄
	 *  @param[out] pvMidValue Hash中间值，长度与IV长度一致，调用者保证足够的缓冲区，可通过EsHashGetInfo获取
	 *  @param[out] pu4DataLen 已进行Hash的数据长度(可以理解为EsHashUpdate累计的数据长度)，可以为NULL
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashGetMidValue(IN ESHANDLE hHash, OUT void* pvMidValue, OUT u4* pu4DataLen);
    
    /**
	 *  @brief Hash-设置中间值
	 *
	 *  @param[in] hHash Hash句柄
	 *  @param[in] pvMidValue 中间值，长度与IV长度一致
	 *  @param[in] u4DataLen 已进行Hash的数据长度，如果为0，相当于重设为初始值
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashSetMidValue(IN ESHANDLE hHash, IN const void* pvMidValue, IN u4 u4DataLen);
    
    /**
	 *  @brief Hash-获取HashOid
	 *
	 *  @param[in] u4AlgId 算法Id，详见ES_HASH_ALG_XXX
	 *  @param[out] pu1Oid Oid
	 *
	 *  @return Oid长度，失败时返回0
	 *
	 *  @details none
	 */
	u1 CALL_TYPE EsHashGetOid(IN u4 u4AlgId, OUT u1* pu1Oid);
    
    /**
	 *  @brief Hash-消息运算
	 *
	 *  @param[in] u4HashAlgId 算法Id，详见ES_HASH_ALG_XXX
	 *  @param[in] pvMsg 待运算的消息数据
	 *  @param[in] u4MsgLen 待运算的消息数据长度
	 *  @param[out] pvHashValue 存储Hash结果
	 *
	 *  @return Hash长度，失败时返回0
	 *
	 *  @details none
	 */
	u1 CALL_TYPE EsHashData(IN u4 u4HashAlgId, IN const void* pvMsg, IN u4 u4MsgLen, OUT void* pvHashValue);
    
	////////////////////////////////////////////////////////////
	// Symmetric algorithm
	////////////////////////////////////////////////////////////

	// Symm AlgId
	#define	ES_SYMM_ALG_DES		    (0x00000001)
	#define ES_SYMM_ALG_3DES112	    (0x00000002)
	#define ES_SYMM_ALG_3DES168		(0x00000004)
	#define ES_SYMM_ALG_AES128		(0x00000008)
	#define ES_SYMM_ALG_AES192		(0x00000010)
	#define ES_SYMM_ALG_AES256		(0x00000020)
	#define ES_SYMM_ALG_SSF33 		(0x00000100)
	#define ES_SYMM_ALG_SM1			(0x00000200)
	#define ES_SYMM_ALG_SM4		    (0x00000400)
	#define ES_SYMM_ALG_RC4			(0x00000800) // 128bit
	#define ES_SYMM_ALG_RC4_40		(0x00001000)
	#define ES_SYMM_ALG_RC2_128		(0x00002000)
	#define ES_SYMM_ALG_RC2_64		(0x00004000)
	#define ES_SYMM_ALG_RC2_40		(0x00008000)

    #define ES_SYMM_ALG_MODE_ECB (0x00)
    #define ES_SYMM_ALG_MODE_CBC (0x01)
    #define ES_SYMM_ALG_MODE_CFB (0x02)
    #define ES_SYMM_ALG_MODE_OFB (0x04)
    #define ES_SYMM_ALG_MODE_MAC (0x08)

	#define ES_OP_TYPE_ENC		(1) // 加密
	#define ES_OP_TYPE_DEC		(0) // 解密

    /**
     *  @brief 对称算法: 加密/解密操作回调接口，整数倍分组运算
	 *
	 *  @param [in] u4AlgId 对称算法Id，详见ES_SYMM_ALG_XXX
	 *  @param [in] u4Mode 对称算法模式，详见ES_SYMM_ALG_MODE_XXX，除了ES_SYMM_ALG_MODE_MAC外的
	 *  @param [in] u4OperationType 运算模式，ES_OP_TYPE_ENC/ES_OP_TYPE_DEC
	 *  @param [in] pvKey 对称密钥，内部自动读取与对应的算法相匹配的长度的内存
	 *  @param [in] pvInput 输入数据，明文，调用者自行补足
	 *  @param [in] u4InputLen 输入数据长度，必须是整分组的倍数
     *  @param [out] pvOutput 密文，支持传入NULL求长度
     *  @param [in,out] pu4OutputLen 密文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	typedef u4 (CALL_TYPE* FunSymmOperation)(IN u4 u4AlgId, 
											 IN u4 u4Mode, 
											 IN u4 u4OperationType,
											 IN const void* pvKey, 
											 IN const void* pvIv,
											 IN const void* pvInput, IN u4 u4InputLen,
											 OUT void* pvOutput, INOUT u4* pu4OutputLen);

    /**
     *  @brief 对称算法: 设置算法加密/解密外部实现(目前仅支持设置SM1/SSF33算法)
	 *
	 *  @param [in] u4AlgId 对称算法Id，详见ES_SYMM_ALG_XXX
	 *  @param [in] u4ModeFlag 对称算法模式标记，详见ES_SYMM_ALG_MODE_XXX，如果支持多个标记，请使用|运算符连接
	 *  @param [in] funSymmOperation 如果为NULL表示取消
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 回调接口必须同时支持加密和解密——非线程安全，调用者自行保证
     */
	u4 CALL_TYPE EsSymmSetExternImpl(IN u4 u4AlgId, IN u4 u4ModeFlag, IN FunSymmOperation funSymmOperation);

    /**
     *  @brief 对称算法: 获取算法信息，比如分组长度，密钥长度
     *
     *  @param [in] u4AlgId 对称算法Id，详见ES_SYMM_ALG_XXX（除了RC4算法）
     *  @param [out] pu4BlockLen 分组长度，允许为NULL
     *  @param [out] pu4KeyLen 密钥长度，允许为NULL
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmGetInfo(IN u4 u4AlgId, OUT u4* pu4BlockLen, OUT u4* pu4KeyLen);

    /**
     *  @brief 对称算法: ECB加密
	 *
	 *  @param [in] u4AlgId 对称算法Id，详见ES_SYMM_ALG_XXX
	 *  @param [in] pvKey 对称密钥，内部自动读取与对应的算法相匹配的长度的内存
	 *  @param [in] pvInput 输入数据，明文，调用者自行补足
	 *  @param [in] u4InputLen 输入数据长度，必须是整分组的倍数
     *  @param [out] pvOutput 密文，支持传入NULL求长度
     *  @param [in,out] pu4OutputLen 密文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmEncEcb(IN u4 u4AlgId,
                              IN const void* pvKey,
                              IN const void* pvInput, IN u4 u4InputLen,
                              OUT void* pvOutput, INOUT u4* pu4OutputLen);
    
    /**
     *  @brief 对称算法: ECB解密
     *
     *  @param [in] u4AlgId 对称算法Id，详见ES_SYMM_ALG_XXX
     *  @param [in] pvKey 对称密钥，内部自动读取与对应的算法相匹配的长度的内存
     *  @param [in] pvInput 输入数据，密文
     *  @param [in] u4InputLen 输入数据长度，必须是整分组的倍数
     *  @param [out] pvOutput 明文，内部不去补足，支持传入NULL求长度
     *  @param [in,out] pu4OutputLen 明文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmDecEcb(IN u4 u4AlgId,
                              IN const void* pvKey,
                              IN const void* pvInput, IN u4 u4InputLen,
                              OUT void* pvOutput, INOUT u4* pu4OutputLen);
    
    /**
     *  @brief 对称算法: CBC加密
     *
     *  @param [in] u4AlgId 对称算法Id，详见ES_SYMM_ALG_XXX
     *  @param [in] pvKey 对称密钥，内部自动读取与对应的算法相匹配的长度的内存
     *  @param [in] pvIv 初始值，与pvKey长度相同
     *  @param [in] pvInput 输入数据，明文，调用者自行补足
     *  @param [in] u4InputLen 输入数据长度，必须是整分组的倍数
     *  @param [out] pvOutput 密文，支持传入NULL求长度
     *  @param [in,out] pu4OutputLen 密文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmEncCbc(IN u4 u4AlgId,
                              IN const void* pvKey,
                              IN const void* pvIv,
                              IN const void* pvInput, IN u4 u4InputLen,
                              OUT void* pvOutput, INOUT u4* pu4OutputLen);
    
    /**
     *  @brief 对称算法: CBC解密
     *
     *  @param [in] u4AlgId 对称算法Id，详见ES_SYMM_ALG_XXX
     *  @param [in] pvKey 对称密钥，内部自动读取与对应的算法相匹配的长度的内存
     *  @param [in] pvIv 初始值，与pvKey长度相同
     *  @param [in] pvInput 输入数据，密文
     *  @param [in] u4InputLen 输入数据长度，必须是整分组的倍数
     *  @param [out] pvOutput 明文，内部不去补足，支持传入NULL求长度
     *  @param [in,out] pu4OutputLen 明文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmDecCbc(IN u4 u4AlgId,
                              IN const void* pvKey,
                              IN const void* pvIv,
                              IN const void* pvInput, IN u4 u4InputLen,
                              OUT void* pvOutput, INOUT u4* pu4OutputLen);
    /**
     *  @brief 对称算法: 计算MAC码
     *
     *  @param [in] u4AlgId 对称算法Id，详见ES_SYMM_ALG_XXX
     *  @param [in] pvKey 对称密钥，内部自动读取与对应的算法相匹配的长度的内存
     *  @param [in] pvIv 初始值，与pvKey长度相同
     *  @param [in] pvInput 输入数据，明文，
     *  @param [in] u4InputLen 输入数据长度，数据长度可以不是整分组，内部会自动补足
     *  @param [out] pvOutput 密文，4字节，不支持传入NULL求长度
     *  @param [in,out] pu4OutputLen 密文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmMac(IN u4 u4AlgId,
                           IN const void* pvKey,
                           IN const void* pvIv,
                           IN const void* pvInput, IN u4 u4InputLen,
                           OUT void* pvOutput, INOUT u4* pu4OutputLen);

	////////////////////////////////////////////////////////////
	// EsSymmCipherXXX接口主要按照PKCS11的接口形式提供，CSP可以在此接口上封装实现。
    ////////////////////////////////////////////////////////////

	/**
     *  @brief 对称算法: 运算初始化
	 *
	 *  @param [in] u4SlotId 未使用，请设置为0
	 *  @param [in] u4AlgId 对称算法Id，详见ES_SYMM_ALG_XXX，不支持RC4
	 *  @param [in] u4BlockSize 对称算法分组长度
	 *  @param [in] pu1Key 对称密钥
	 *  @param [in] u4KeyLen 对称密钥pvKey长度，必须>=对应算法的密钥长度
	 *  @param [in] pu1IV 初始值，与pvKey长度相同
	 *  @param [in] u4IVLen 初始值长度，必须>=对应算法的密钥长度
     *  @param [in] u4Mode 对称算法模式，详见ES_SYMM_ALG_MODE_XXX，目前仅支持ECB/CBC
     *  @param [in] u4Padding 是否补足，0-不补足，其他-补足
     *  @param [out] phCipher 句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmCipherInit(IN u4 u4SlotId, IN u4 u4AlgId, IN u4 u4BlockSize, 
								  IN const u1* pu1Key, IN u4 u4KeyLen, 
								  IN const u1* pu1IV, IN u4 u4IVLen,
								  IN u4	u4Mode, IN u4 u4Padding, 
								  OUT ESHANDLE* phCipher);

	/**
     *  @brief 对称算法: 释放
	 *
	 *  @param [in] hCipher 句柄，由EsSymmCipherInit生成
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmCipherRelease(IN ESHANDLE hCipher);

	/**
     *  @brief 对称算法: 运算
	 *
	 *  @param [in] hCipher 句柄，由EsSymmCipherInit生成
	 *  @param [in] u4OperationType 操作类型,加密/解密,详见ES_OP_TYPE_XXX
	 *  @param [out] pu1Out 输出数据
	 *  @param [in,out] pu4OutLen 输出数据缓冲区/结果长度
	 *  @param [in] pu1In 输入数据
	 *  @param [in] u4InLen 输入数据长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 解密操作或者不补足的加密操作时，输入数据长度必须是分组长度倍数
     */
	u4 CALL_TYPE EsSymmCipherOperation(IN ESHANDLE hCipher, IN u4 u4OperationType, 
									   OUT u1* pu1Out, INOUT u4* pu4OutLen, 
									   IN const u1* pu1In, IN u4 u4InLen);

	/**
     *  @brief 对称算法: 多分组运算-update
	 *
	 *  @param [in] hCipher 句柄，由EsSymmCipherInit生成
	 *  @param [in] u4OperationType 操作类型,加密/解密,详见ES_OP_TYPE_XXX
	 *  @param [out] pu1Out 输出数据
	 *  @param [in,out] pu4OutLen 输出数据缓冲区/结果长度
	 *  @param [in] pu1In 输入数据
	 *  @param [in] u4InLen 输入数据长度，可以不是整分组长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmCipherUpdate(IN ESHANDLE hCipher, IN u4 u4OperationType, 
									OUT u1* pu1Out, INOUT u4* pu4OutLen, 
									IN const u1* pu1In, IN u4 u4InLen);

	/**
     *  @brief 对称算法: 多分组运算-final
	 *
	 *  @param [in] hCipher 句柄，由EsSymmCipherInit生成
	 *  @param [in] u4OperationType 操作类型,加密/解密,详见ES_OP_TYPE_XXX
	 *  @param [out] pu1Out 输出数据
	 *  @param [in,out] pu4OutLen 输出数据缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details 1. 不补足情况下，所有数据应该完成了加密/解密，RemainLen=0
	 *			 2. 补足且解密操作，应满足RemainLen=BlockLen
     */
	u4 CALL_TYPE EsSymmCipherFinal(IN ESHANDLE hCipher, IN u4 u4OperationType, 
								   OUT u1* pu1Out, INOUT u4* pu4OutLen);

	// 这两个接口主要是CSP调用，为了在上层调用为取长度和上层缓冲区长度不足时不进行实质性的运算，特提供以下两个接口
	// EsSymCipherEncryptGetOutLen,EsSymCipherDecryptGetOutLen are for CSP convenience
	/**
     *  @brief 对称算法: 获取加密运算结果长度
	 *
	 *  @param [in] hCipher 句柄，由EsSymmCipherInit生成
	 *  @param [in,out] pu4OutLen 输出结果长度
	 *  @param [in] u4InLen 输入数据长度
	 *  @param [in] u4FinalFlag 0:非final操作，其他:final操作
     *
     *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details 此接口主要是CSP调用，为了在上层调用为取长度和上层缓冲区长度不足时[不进行实质性]的运算
     */
	u4 CALL_TYPE EsSymCipherEncryptGetOutLen(IN ESHANDLE hCipher, OUT u4* pu4OutLen, IN u4 u4InLen, IN u4 u4FinalFlag);

	/**
     *  @brief 对称算法: 获取解密运算结果长度
	 *
	 *  @param [in] hCipher 句柄，由EsSymmCipherInit生成
	 *  @param [in,out] pu4OutLen 输出结果长度
	 *  @param [in] u4InLen 输入数据长度
	 *  @param [in] u4FinalFlag 0:非final操作，其他:final操作
     *
     *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details 此接口主要是CSP调用，为了在上层调用为取长度和上层缓冲区长度不足时[不进行实质性]的运算
     */
	u4 CALL_TYPE EsSymCipherDecryptGetOutLen(IN ESHANDLE hCipher, OUT u4* pu4OutLen, IN u4 u4InLen, IN u4 u4FinalFlag);
	//---------------------------------------------------------------------------------
    
	//////////////////////////////////////////////////////////////////////////
	// 非对称算法
	//////////////////////////////////////////////////////////////////////////

#	define ES_ASYM_ALG_RSA				(0x0001)
#	define ES_ASYM_ALG_SM2				(0x0002)
#	define ES_ASYM_ALG_ECC				(0x0003) // 代表ecc和sm2的算法，仅作为内部使用，API不支持
	// ecc 重要!!! 0x01xx为ECC算法
	// ecc 曲线参数为secp r1系列，0x010x
#	define ES_ASYM_ALG_ECC_SECP192R1	(0x0101) // nistP192,模长24byte
#	define ES_ASYM_ALG_ECC_SECP224R1	(0x0102) // nistP224,模长28byte
#	define ES_ASYM_ALG_ECC_SECP256R1	(0x0103) // ECDSA_P256,模长32byte 
#	define ES_ASYM_ALG_ECC_SECP384R1	(0x0104) // ECDSA_P384,模长48byte
#	define ES_ASYM_ALG_ECC_SECP521R1	(0x0105) // ECDSA_P521,模长521bit,采用68byte
	// ecc 曲线参数为secp k1系列，0x011x
#	define ES_ASYM_ALG_ECC_SECP192K1	(0x0111) // secP192k1,模长24byte
#	define ES_ASYM_ALG_ECC_SECP224K1	(0x0112) // secP224k1,模长28byte
#	define ES_ASYM_ALG_ECC_SECP256K1	(0x0113) // secP256k1,模长32byte 
	// ecc 曲线由调用者确定 0x01FF
#	define ES_ASYM_ALG_ECC_SET			(0x01FF) // ecc算法，用户自定义曲线和模长

#	define IS_ECC_ALG(u4AlgId)			( 0x0100 == ((u4AlgId)&0x0100) )

#	define ES_RSA_MAX_MOD_LEN			(0x200) // 可支持到rsa4096
#	define ES_ECC_MAX_MOD_LEN			(0x100)	// 可支持到ecc2048

	#ifndef _BLKASYMKEY_
	#define _BLKASYMKEY_
	typedef struct BlkSmallRsaKeyTag
	{
		u4				m_u4ELen;
		u1				m_pu1E[256];
		u1				m_pu1N[256];
		u1				m_pu1D[256];
		u1				m_pu1P[128];
		u1				m_pu1Q[128];
		u1				m_pu1Dp[128];
		u1				m_pu1Dq[128];
		u1				m_pu1Qinv[128];
	}BlkSmallRsaKey; // 原来的BlkRsaKey，暂时保留方便兼容旧密钥结构的代码使用
	typedef struct BlkRsaKeyTag
	{
		u4				m_u4ELen;
		u1				m_pu1E[512];
		u1				m_pu1N[512];
		u1				m_pu1D[512];
		u1				m_pu1P[256];
		u1				m_pu1Q[256];
		u1				m_pu1Dp[256];
		u1				m_pu1Dq[256];
		u1				m_pu1Qinv[256];
	}BlkRsaKey;
	typedef struct BlkSm2KeyTag
	{
		u1				m_pu1PubKeyX[32];
		u1				m_pu1PubKeyY[32];
		u1				m_pu1PriKeyD[32];
	}BlkSm2Key;
	typedef struct BlkEccKeyTag
	{
		u1				m_pu1PubKeyX[ES_ECC_MAX_MOD_LEN];
		u1				m_pu1PubKeyY[ES_ECC_MAX_MOD_LEN];
		u1				m_pu1PriKeyD[ES_ECC_MAX_MOD_LEN];
	}BlkEccKey;
	typedef union UnionAsymKeyTag
	{
		BlkRsaKey		m_blkRsaKey;
		BlkSm2Key		m_blkSm2Key;
		BlkEccKey		m_blkEccKey;
	}UnionAsymKey;
	typedef struct BlkAsymKeyTag
	{
		u4				m_u4AlgId;
		u4				m_u4ModLen;
		UnionAsymKey	m_unionAsymKey;
	}BlkAsymKey;
	#endif

    /**
     *  @brief 非对称算法: 生成RSA密钥对
     *
     *  @param [in] iModBitLen 密钥模长(位)，1024/2048/4096
     *  @param [out] pvPubKeyN 公钥N，iModBitLen/8
     *  @param [out] pvPubKeyE 公钥指数E，4字节
     *  @param [out] pvPriKeyD 私钥D，iModBitLen/8
     *  @param [out] pvPriKeyP 私钥P，iModBitLen/16
	 *  @param [out] pvPriKeyQ 私钥Q，iModBitLen/16
     *  @param [out] pvPriKeyDP 私钥DP，iModBitLen/16
     *  @param [out] pvPriKeyDQ 私钥DQ，iModBitLen/16
     *  @param [out] pvPriKeyQInv 私钥QInv，iModBitLen/16
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaGen(IN int iModBitLen,
                          OUT void* pvPubKeyN, OUT void* pvPubKeyE,
                          OUT void* pvPriKeyD,
                          OUT void* pvPriKeyP, OUT void* pvPriKeyQ,
                          OUT void* pvPriKeyDP, OUT void* pvPriKeyDQ,
                          OUT void* pvPriKeyQInv);
    
    /**
     *  @brief 非对称算法RSA: 公钥加密
     *
     *  @param [in] u2ModLen 密钥模长(字节)，0x80/0x100/0x200
     *  @param [in] pvPubKeyN 公钥N，iModBitLen/8
     *  @param [in] pvPubKeyE 公钥N
     *  @param [in] u4ELen E的长度
     *  @param [in] pvInput 输入数据，长度要求为模长，建议使用PKCS1 type2补足（实际测试结果这个参数的传值可以超过或者少于模长个字节,不使用PKCS1 type2补足也可以进行加密）
     *  @param [out] pvOutput 输出，长度要求为模长
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaEnc(IN u2 u2ModLen,
                          IN const void* pvPubKeyN,
                          IN const void* pvPubKeyE, IN u4 u4ELen,
                          IN const void* pvInput,
                          OUT void* pvOutput);
    
    /**
     *  @brief 非对称算法RSA: 私钥签名(先补OID，接着按照PKCS1 type1补足后签名)
     *
     *  @param [in] pblkAsymKey 私钥
     *  @param [in] u4HashAlgId Hash算法Id，详见ES_HASH_ALG_XXXX
     *  @param [in] pvHashValue Hash值，长度与hash算法id匹配
     *  @param [out] pvSignature 签名值，缓冲区长度保证有模长
     *  @param [out] pu4SignatureLen 签名值长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaSign(IN const BlkAsymKey* pblkAsymKey,
                           IN u4 u4HashAlgId, IN const void* pvHashValue,
                           OUT void* pvSignature, OUT u4* pu4SignatureLen);
    
    /**
     *  @brief 非对称算法RSA: 公钥验签
     *
     *  @param [in] u2ModLen 模长(字节)
     *  @param [in] pvPubKeyN 公钥N，长度为模长
     *  @param [in] pvPubKeyE 公钥E
     *  @param [in] u4ELen 公钥E长度
     *  @param [in] pvHashValue Hash值
     *  @param [in] u4HashValueLen Hash值长度
     *  @param [in] pvSignature 签名值
     *  @param [in] u4SignatureLen 签名值长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaVerify(IN u2 u2ModLen,
                             IN const void* pvPubKeyN,
                             IN const void* pvPubKeyE, IN u4 u4ELen,
                             IN const void* pvHashValue, IN u4 u4HashValueLen,
                             IN const void* pvSignature, IN u4 u4SignatureLen);
    
    /**
     *  @brief 非对称算法RSA: 公钥解密
     *
     *  @param [in] u2ModLen 模长(字节)
     *  @param [in] pvPubKeyN 公钥N，长度为模长
     *  @param [in] pvPubKeyE 公钥E
	 *  @param [in] u4ELen 公钥E长度
	 *  @param [in] pvInput 输入数据，长度为模长
	 *  @param [out] pvOutput 解密结果，长度为模长
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaPubKeyDec(IN u2 u2ModLen, 
								IN const void* pvPubKeyN, 
								IN const void* pvPubKeyE, IN u4 u4ELen, 
								IN const void* pvInput, 
								OUT void* pvOutput);
    
    /**
     *  @brief 非对称算法RSA: 私钥D解密
     *
     *  @param [in] u2ModLen 模长(字节)
     *  @param [in] pvPubKeyN 公钥N，长度为模长
     *  @param [in] pvPriKeyD 私钥D
     *  @param [in] pvInput 输入数据，长度为模长
     *  @param [out] pvOutput 解密结果，长度为模长
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaDecD(IN u2 u2ModLen,
                           IN const void* pvPubKeyN,
                           IN const void* pvPriKeyD,
                           IN const void* pvInput,
                           OUT void* pvOutput);
    
    /**
     *  @brief 非对称算法RSA: 私钥P/Q/DP/DQ/QINV解密
     *
     *  @param [in] u2ModLen 模长(字节)
     *  @param [in] pvPriKeyP 私钥P，长度为模长/2
     *  @param [in] pvPriKeyQ 私钥Q，长度为模长/2
     *  @param [in] pvPriKeyDP 私钥DP，长度为模长/2
     *  @param [in] pvPriKeyDQ 私钥DQ，长度为模长/2
     *  @param [in] pvPriKeyQinv 私钥QInv，长度为模长/2
     *  @param [in] pvInput 输入数据，长度为模长
     *  @param [out] pvOutput 解密结果，长度为模长
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaDec(IN u2 u2ModLen,
                          IN const void* pvPriKeyP, IN const void* pvPriKeyQ,
                          IN const void* pvPriKeyDP, IN const void* pvPriKeyDQ,
                          IN const void* pvPriKeyQinv,
                          IN const void* pvInput,
                          OUT void* pvOutput);
    /**
     *  @brief 非对称算法RSA: 根据P、Q计算出私钥全部分量
     *
     *  @param [in] u4E E，默认为0x10001
     *  @param [in] pu1P 私钥P，长度为模长/2
     *  @param [in] pu1Q 私钥Q，长度为模长/2
     *  @param [in] u4ModLen 模长
     *  @param [out] pblkAsymKey 私钥
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
    u4 CALL_TYPE EsRsaPQGetKey(IN u4 u4E,
                               IN const u1* pu1P, IN const u1* pu1Q,
                               IN u4 u4ModLen,
                               OUT BlkAsymKey* pblkAsymKey);

    /**
     *  @brief 非对称算法SM2: 生成密钥对
     *
     *  @param [out] pu1PriKeyD 私钥D，32字节
     *  @param [out] pu1PubKeyQx 公钥X，32字节
     *  @param [out] pu1PubKeyQy 公钥Y，32字节
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2GenKey(OUT u1* pu1PriKeyD, OUT u1* pu1PubKeyQx, OUT u1* pu1PubKeyQy);
    
    /**
     *  @brief 非对称算法SM2: 私钥签名
     *
     *  @param [in] pu1PriKeyD 私钥D，32字节
     *  @param [in] pu1HashValue hash值，默认为SM3结果，32字节
     *  @param [out] pu1SignatureR 签名值R，32字节
     *  @param [out] pu1SignatureS 签名值S，32字节
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2Sign(IN const u1* pu1PriKeyD,
                           IN const u1* pu1HashValue,
                           OUT u1* pu1SignatureR,
                           OUT u1* pu1SignatureS);
    
    /**
     *  @brief 非对称算法SM2: 外部消息私钥签名
     *
     *  @param [in] pu1PriKeyD 私钥D，32字节
     *  @param [in] pvMsg 消息
     *  @param [in] u4MsgLen 消息长度
     *  @param [out] pvSignature 签名值，R|S，64字节
     *  @param [out] pu4SignatureLen 签名值长度，64
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2SignMsg(IN const u1* pu1PriKeyD,
                              IN const void* pvMsg, IN u4 u4MsgLen,
                              OUT void* pvSignature, OUT u4* pu4SignatureLen);
    
    /**
     *  @brief 非对称算法SM2: 公钥验签
     *
     *  @param [in] pu1PubKeyQx 公钥X，32字节
     *  @param [in] pu1PubKeyQy 公钥Y，32字节
     *  @param [in] pu1HashValue Hash值，32字节
     *  @param [in] pu1SignatureR 签名值R，32字节
     *  @param [in] pu1SignatureS 签名值S，32字节
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2Verify(IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy,
                             IN const u1* pu1HashValue,
                             IN const u1* pu1SignatureR, IN const u1* pu1SignatureS);
    
    /**
     *  @brief 非对称算法SM2: 计算Z值
     *
     *  @param [in] pszID ID值，如果为空，则使用默认的"1234567812345678"
     *  @param [in] pu1PubKeyQx 公钥X，32字节
     *  @param [in] pu1PubKeyQy 公钥Y，32字节
     *  @param [out] pu1Za Z值，32字节
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2GetZa(IN const char* pszID, IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy, OUT u1* pu1Za);
    
    /**
     *  @brief 非对称算法SM2: 公钥加密
     *
     *  @param [in] pu1PubKeyQx 公钥X，32字节
     *  @param [in] pu1PubKeyQy 公钥Y，32字节
     *  @param [in] pu1Plain 明文
     *  @param [in] u4PlainLen 明文长度
     *  @param [out] pu1Cipher 密文 国密新规范C1|C3|C2，支持传入NULL求长度
     *  @param [in,out] pu4CipherLen 密文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2Enc(IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy,
                          IN const u1* pu1Plain, IN u4 u4PlainLen,
                          OUT u1* pu1Cipher, INOUT u4* pu4CipherLen);
    
    /**
     *  @brief 非对称算法SM2: 私钥解密
     *
     *  @param [in] pu1PriKeyD 私钥D，32字节
     *  @param [in] pu1Cipher 密文 国密新规范C1|C3|C2或旧规范C1|C2|C3都可以
     *  @param [in] u4CipherLen 密文长度
     *  @param [out] pu1Plain 明文，支持传入NULL求长度
     *  @param [in,out] pu4PlainLen 明文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2Dec(IN const u1* pu1PriKeyD,
                          IN const u1* pu1Cipher, IN u4 u4CipherLen,
                          OUT u1* pu1Plain, INOUT u4* pu4PlainLen);
    
    /**
     *  @brief 非对称算法SM2: 由私钥计算出公钥
     *
     *  @param [in] pu1PriKeyD 私钥D，32字节
     *  @param [out] pu1PubKeyQx 公钥X，32字节
     *  @param [out] pu1PubKeyQy 公钥Y，32字节
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2ExportPubKey(IN const u1* pu1PriKeyD, OUT u1* pu1PubKeyQx, OUT u1* pu1PubKeyQy);

    /**
     *  @brief 非对称算法SM2: SM3带Z值Hash运算
     *
     *  @param [in] bCalcZa ESFALSE-不计算Z值,否则计算
     *  @param [in] pblkAsymKey 公钥，bCalcZa=ESTRUE时生效
     *  @param [in] pvMsg 消息
     *  @param [in] u4MsgLen 消息长度
     *  @param [out] pu1HashValue Hash值，32字节
     *
     *  @return SM3 Hash结果长度32字节，失败时返回0
     *
     *  @details MSB first
     */
	u1 CALL_TYPE EsSm3Hash(IN ESBOOL bCalcZa, IN const BlkAsymKey* pblkAsymKey,
                           IN const void* pvMsg, IN u4 u4MsgLen,
                           OUT u1* pu1HashValue);
    
    /**
     *  @brief 非对称算法: 生成密钥对
     *
     *  @param [in,out] pblkAsymKey 密钥（含私钥），入参时请设置m_u4AlgId和m_u4ModLen
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymGenKeyPair(INOUT BlkAsymKey* pblkAsymKey);
    
    /**
     *  @brief 非对称算法: 私钥签名
     *
     *  @param [in] pblkAsymKey 密钥（含私钥）
     *  @param [in] pvSignData 签名数据，长度为模长，RSA默认为补足后的数据，SM2默认为SM3的Hash值,ECC详见EsEccSign
     *  @param [out] pvSignature 签名值,支持传入NULL求长度
     *  @param [in,out] pu4SignatureLen 签名值缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymSign(IN const BlkAsymKey* pblkAsymKey,
                            IN const void* pvSignData,
                            OUT void* pvSignature, INOUT u4* pu4SignatureLen);
    
    /**
     *  @brief 非对称算法: 公钥加密
     *
     *  @param [in] pblkAsymKey 密钥
     *  @param [in] pvPlainText 明文数据，RSA长度要求为模长，建议使用PKCS1 type2补足。SM2没有特殊要求
     *  @param [in] u4PlainTextLen 明文数据长度
     *  @param [out] pvCipherText 密文,支持传入NULL求长度，SM2按照C1|C3|C2返回
     *  @param [in,out] pu4CipherTextLen 密文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymEnc(IN const BlkAsymKey* pblkAsymKey,
                           IN const void* pvPlainText, IN u4 u4PlainTextLen,
                           OUT void* pvCipherText, INOUT u4* pu4CipherTextLen);
    
    /**
     *  @brief 非对称算法: 私钥解密
     *
     *  @param [in] pblkAsymKey 密钥
     *  @param [in] pvCipherText 密文,SM2支持C1|C2|C3或C1|C3|C2
     *  @param [in] u4CipherTextLen 签名值缓冲区/结果长度
     *  @param [out] pvPlainText 明文,支持传入NULL求长度
     *  @param [in,out] pu4PlainTextLen 明文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymDec(IN const BlkAsymKey* pblkAsymKey,
                           IN const void* pvCipherText, IN u4 u4CipherTextLen,
                           OUT void* pvPlainText, INOUT u4* pu4PlainTextLen);
    
    /**
     *  @brief 非对称算法: 验签
     *
     *  @param [in] pblkAsymKey 密钥
     *  @param [in] u4HashAlgId hash算法，详见ES_HASH_ALG_XXX，对RSA有效
     *  @param [in] pvHashValue Hash值，长度与Hash算法对应
     *  @param [in] pvSignature 签名值
     *  @param [in] u4SignatureLen 签名值长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymVerifySignature(IN const BlkAsymKey* pblkAsymKey,
                                       IN u4 u4HashAlgId,
                                       IN const void* pvHashValue,
                                       IN const void* pvSignature, IN u4 u4SignatureLen);
    
    /**
     *  @brief 非对称算法: 消息验签
     *
     *  @param [in] pblkAsymKey 密钥
     *  @param [in] bCalcZa ESFALSE-不计算Z值，否则计算，对SM2有效
     *  @param [in] u4HashAlgId hash算法，详见ES_HASH_ALG_XXX
     *  @param [in] pvMsg 消息
     *  @param [in] u4MsgLen 消息长度
     *  @param [in] pvSignature 签名值
     *  @param [in] u4SignatureLen 签名值长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymVerifySignatureMsg(IN const BlkAsymKey* pblkAsymKey,
                                          IN ESBOOL bCalcZa,
                                          IN u4 u4HashAlgId,
                                          IN const void* pvMsg, IN u4 u4MsgLen,
                                          IN const void* pvSignature, IN u4 u4SignatureLen);
    
    /**
     *  @brief 非对称算法: 消息签名
     *
     *  @param [in] pblkAsymKey 密钥
     *  @param [in] u4HashAlgId hash算法，详见ES_HASH_ALG_XXX
     *  @param [in] pvMsg 消息
     *  @param [in] u4MsgLen 消息长度
     *  @param [out] pvSignature 签名值
     *  @param [in,out] pu4SignatureLen 签名值缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymSignMsg(IN const BlkAsymKey* pblkAsymKey,
                               IN u4 u4HashAlgId,
                               IN const void* pvMsg, IN u4 u4MsgLen,
                               OUT void* pvSignature, INOUT u4* pu4SignatureLen);

	typedef struct BlkSm2CtxTag
	{
		u4 m_u4State;					// 0x00 未计算过C1, 0x01 已经计算过C1
		u1 m_pu1RFU[0x40+0x04+0x68];    // 函数内部使用的结构，调用者不用关心
	}BlkSm2Ctx;
    
    /**
     *  @brief 非对称算法SM2: 公钥加密-支持分组
     *
     *  @param [in] pu1PubKeyQx 公钥X，32字节
     *  @param [in] pu1PubKeyQy 公钥Y，32字节
     *  @param [in] u4FinalFlag 0:中间运算,明文长度必须是0x40整数倍;其他:final运算，明文长度任意
     *  @param [in] pblkSm2Ctx 加密上下文信息
     *  @param [in] pu1Plain 明文
     *  @param [in] u4PlainLen 明文长度
     *  @param [out] pu1Cipher 密文 按C1C2C3返回
     *                         第1次update：返回C1|第一个密文；
     *                         接下来的任意次update：仅返回对应的密文；
     *                         final：除了密文，还包括C3
     *  @param [in,out] pu4CipherLen 密文缓冲区/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 1. MSB first
     *           2. 密文结果: C1C2C3
     */
	u4 CALL_TYPE EsSm2EncEx(IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy,
                            IN u4 u4FinalFlag,
                            BlkSm2Ctx* pblkSm2Ctx,
                            IN const u1* pu1Plain, IN u4 u4PlainLen,
                            OUT u1* pu1Cipher, INOUT u4* pu4CipherLen);

	#define ES_SM2_CIPHER_MODE_C1C2C3	(0x01)
	#define ES_SM2_CIPHER_MODE_C1C3C2	(0x02)
    /**
     *  @brief 非对称算法SM2: 公钥加密-可指定输出格式是C1C2C3/C1C3C2
     *
     *  @param [in] pu1PubKeyQx 公钥X，32字节
     *  @param [in] pu1PubKeyQy 公钥Y，32字节
     *  @param [in] pu1Plain 明文
     *  @param [in] u4PlainLen 明文长度
     *  @param [out] pu1Cipher 密文 国密新规范C1|C3|C2，支持传入NULL求长度
     *  @param [in,out] pu4CipherLen 密文缓冲区/结果长度
     *  @param [in] u4CipherMode 加密模式，详见ES_SM2_CIPHER_MODE_XXX
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2EncWithMode(IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy,
                                  IN const u1* pu1Plain, IN u4 u4PlainLen,
                                  OUT u1* pu1Cipher, INOUT u4* pu4CipherLen,
                                  IN u4 u4CipherMode);
    
    /**
     *  @brief 非对称算法SM2: 私钥解密-可指定输出格式是C1C2C3/C1C3C2
     *
     *  @param [in] pu1PriKeyD 私钥D，32字节
     *  @param [in] pu1Cipher 密文 国密新规范C1|C3|C2
     *  @param [in] u4CipherLen 密文长度
     *  @param [out] pu1Plain 明文，支持传入NULL求长度
     *  @param [in,out] pu4PlainLen 明文缓冲区/结果长度
     *  @param [in] u4CipherMode 加密模式，详见ES_SM2_CIPHER_MODE_XXX
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2DecWithMode(IN const u1* pu1PriKeyD,
                                  IN const u1* pu1Cipher, IN u4 u4CipherLen,
                                  OUT u1* pu1Plain, INOUT u4* pu4PlainLen,
                                  IN u4 u4CipherMode);
	
	//////////////////////////////////////////////////////////////////////////
	// 密钥协商
	//////////////////////////////////////////////////////////////////////////
	/**
	 *  @brief 做为密钥协商的发起方首先调用此函数获取到临时密钥对和协商句柄，agt=agreement。
	 *
	 *  @param[in] pszSponsorId 发起者的ID，NULL表示使用缺省的ID值“1234567812345678”。
	 *  @param[in] pblkAsymKeySponsor 发起者的公私钥对，一定要有私钥。 
	 *  @param[out] pblkAsymKeySponsorTemp 发起者的临时公钥。
	 *  @param[out] phAgt 发起者的协商句柄，后续可以用此句柄生成协商密钥。
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsSm2AgtSponsorInit(IN const char* pszSponsorId, 
									 IN const BlkAsymKey* pblkAsymKeySponsor, 
									 OUT BlkAsymKey* pblkAsymKeySponsorTemp, 
									 OUT ESHANDLE* phAgt);
	/**
	 *  @brief 做为密钥协商的响应方调用此函数获取到会话密钥，agt=agreement。
	 *
	 *  @param[in] pszSponsorId 发起者的ID，NULL表示使用缺省的ID值“1234567812345678”。
	 *  @param[in] pblkAsymKeySponsor 发起者的公钥。 
	 *  @param[in] pblkAsymKeySponsorTemp 发起者的临时公钥。
	 *  @param[in] pszResponderId 响应方的ID，NULL表示使用缺省的ID值“1234567812345678”。
	 *  @param[in] pblkAsymKeyResponder 响应方的公私钥对，一定要有私钥。 
	 *  @param[out] pblkAsymKeyResponderTemp 响应方的临时公私钥对。
	 *  @param[out] pu1SKey 协商到的会话密钥。保证0x20的缓冲区长度
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsSm2AgtResponderGenSKey(IN const char* pszSponsorId, 
										  IN const BlkAsymKey* pblkAsymKeySponsor, 
										  IN const BlkAsymKey* pblkAsymKeySponsorTemp, 
										  IN const char* pszResponderId, 
										  IN const BlkAsymKey* pblkAsymKeyResponder, 
										  OUT BlkAsymKey* pblkAsymKeyResponderTemp, 
										  OUT u1* pu1SKey);
	/**
	 *  @brief 密钥协商发起方生成会话密钥，agt=agreement。
	 *
	 *  @param[in] hAgt 发起方协商句柄。
	 *  @param[in] pszResponderId 响应方的ID，NULL表示使用缺省的ID值“1234567812345678”。
	 *  @param[in] pblkAsymKeyResponder 响应方的公钥。 
	 *  @param[in] pblkAsymKeyResponderTemp 响应方的临时公钥。
	 *  @param[out] pu1SKey 协商到的会话密钥。保证0x20的缓冲区长度
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsSm2AgtSponsorGenSKey(IN ESHANDLE hAgt, IN const char* pszResponderId, 
										IN const BlkAsymKey* pblkAsymKeyResponder, 
										IN const BlkAsymKey* pblkAsymKeyResponderTemp, 
										OUT u1* pu1SKey);
	
    /**
	 *  @brief 会话密钥协商-释放发起方建立的密钥协商句柄，agt=agreement。
	 *
	 *  @param[in,out] phAgt 发起方协商句柄
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsSm2AgtSponsorFinal(INOUT ESHANDLE* phAgt);

	//////////////////////////////////////////////////////////////////////////
	// 补足
	//////////////////////////////////////////////////////////////////////////
	#	define	PADDING_TYPE_ZERO		0x00	// 在原始数据前补足00到满分组，满分组不补足
	#	define	PADDING_TYPE_PKCS_1		0x01	// padding fmt: 00 01 FF...FF 00 Data pkcs padding type 1
	#	define	PADDING_TYPE_PKCS_2		0x02	// padding fmt: 00 02 PS 00 Data pkcs padding type 2
	#	define	PADDING_TYPE_PKCS_5		0x05	// padding fmt: XX[XX]
	#	define	PADDING_TYPE_PKCS_9		0x09	// padding fmt: 00 09 FF...FF 00 Data
	#	define	PADDING_TYPE_7816		0x80	// padding fmt: Data 80 00..00，如果Data是满分组，就不补足

	/**
	 *  @brief 数据补足
	 *  
	 *  @param [in] pvOrgData 原始数据
	 *  @param [in] u4OrgDataLen 原始数据长度，>0
	 *  @param [in] u4PaddingType 补足类型，详见PADDING_TYPE_XXX
	 *  @param [in] u4ModOrBlockLen 模长/分组长度
	 *  @param [out] pvOutData 补足后的数据，调用者需要保证缓冲区大小足够，详见details
	 *  @param [out] pu4OutDataLen 补足后的数据长度，允许为NULL
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details 分组方式和长度要求说明
	 *           PADDING_TYPE_ZERO  :在原始数据前补足00到满分组，满分组不补足，输入长度<=分组长度，输出长度为分组长度
	 *           PADDING_TYPE_PKCS_1:在原始数据前补足00 01 FF...FF 00到满分组，(输入长度+3)<=分组长度，输出长度为分组长度
	 *           PADDING_TYPE_PKCS_2:在原始数据前补足00 01 非0随机数 00到满分组，(输入长度+3)<=分组长度，输出长度为分组长度
	 *           PADDING_TYPE_PKCS_5:在原始数据后补足N，N=模长-输入长度%模长)到满分组，原始数据是满分组也需要补足，输出长度为分组长度(或+1分组长度)
	 *           PADDING_TYPE_PKCS_9:在原始数据前补足00 01 FF...FF 00到满分组，(输入长度+3)<=分组长度，输出长度为分组长度
	 *           PADDING_TYPE_7816  :在原始数据后补足80 00...到满分组，满分组不补足，输出长度为分组长度
	 */
	u4 CALL_TYPE EsPadding(IN const void* pvOrgData, IN u4 u4OrgDataLen,
                           IN u4 u4PaddingType, IN u4 u4ModOrBlockLen,
                           OUT void* pvOutData, OUT u4* pu4OutDataLen);

	/**
	 *  @brief 数据去补足
	 *  
	 *  @param [in] pvOrgData 补足数据
	 *  @param [in] u4OrgDataLen 补足数据长度，
	 *  @param [in] u4PaddingType 补足类型，详见PADDING_TYPE_XXX
	 *  @param [in] u4ModOrBlockLen 模长/分组长度
	 *  @param [out] pvOutData 去补足后的数据，调用者需要保证缓冲区大小足够，建议与输入参数长度一致
	 *  @param [out] pu4OutDataLen 补足后的数据长度，允许为NULL
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details 补足模式以及参数的要求详见EsPadding
	 */
	u4 CALL_TYPE EsPaddingRemove(IN const void* pvOrgData, IN u4 u4OrgDataLen,
                                 IN u4 u4PaddingType, IN u4 u4ModOrBlockLen,
                                 OUT void* pvOutData, OUT u4* pu4OutDataLen);

	//////////////////////////////////////////////////////////////////////////
	// HMAC
	//////////////////////////////////////////////////////////////////////////

	/**
	 *  @brief 初始化Hmac句柄
	 *  
	 *  @param [in] u4HashAlgId Hash算法Id
	 *  @param [in] pu1IV Hash初始值
	 *  @param [in] pu1Key 密钥
	 *  @param [in] u4KeyLen 密钥长度
	 *  @param [out] phEsHmac 返回Hmac句柄
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacInit(IN u4 u4HashAlgId, IN const u1*pu1IV, IN const u1* pu1Key, IN u4 u4KeyLen, OUT ESHANDLE* phEsHmac);

	/**
	 *  @brief Hmac运算
	 *  
	 *  @param [in] hEsHmac Hmac句柄
	 *  @param [in] pu1In 原文
	 *  @param [in] u4InLen 原文长度
	 *  @param [out] pu1Out hmac值，支持传入NULL求长度
	 *  @param [out] pu4OutLen hmac值缓冲区/结果长度
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacOperation(IN ESHANDLE hEsHmac, IN const u1* pu1In, IN u4 u4InLen, OUT u1* pu1Out, u4* pu4OutLen);

	/**
	 *  @brief Hmac中间运算
	 *  
	 *  @param [in] hEsHmac Hmac句柄
	 *  @param [in] pu1In 原文
	 *  @param [in] u4InLen 原文长度
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacUpdate(IN ESHANDLE hEsHmac, IN const u1* pu1In, IN u4 u4InLen);

	/**
	 *  @brief Hmac结尾运算
	 *  
	 *  @param [in] hEsHmac Hmac句柄
	 *  @param [out] pu1Out hmac值，支持传入NULL求长度
	 *  @param [in,out] pu4OutLen hmac值缓冲区/结果长度
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacFinal(IN ESHANDLE hEsHmac, OUT u1* pu1Out, INOUT u4* pu4OutLen);

	/**
	 *  @brief 释放Hmac句柄
	 *  
	 *  @param [in] hEsHmac Hmac句柄
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacRelease(IN ESHANDLE hEsHmac);

	/**
	 *  @brief Hmac运算
	 *  
	 *  @param [in] u4HashAlgId Hash算法Id
	 *  @param [in] pu1Key 密钥
	 *  @param [in] u4KeyLen 密钥长度
	 *  @param [in] pu1Msg 消息
	 *  @param [in] u4MsgLen 消息长度
	 *  @param [out] pu1Hmac 操作结果，支持传入NULL求长度
	 *  @param [in,out] pu4HmacLen 操作结果缓冲区/结果长度
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmac(IN u4 u4HashAlgId, IN const u1* pu1Key, IN u4 u4KeyLen, IN const u1* pu1Msg, IN u4 u4MsgLen, OUT u1* pu1Hmac, INOUT u4* pu4HmacLen);

	//////////////////////////////////////////////////////////////////////////
	// ECC算法
	//////////////////////////////////////////////////////////////////////////

#	define ES_ECC_PRIME_FIELD				(0x01)	// Fp
#	define ES_ECC_CHARACTERISTIC_TWO_FIELD	(0x02)	// F2m

	typedef struct BlkEccCurveTag {
		u4		m_u4Version;						// version of the struct, should be 0x00
		u2		m_u2ModBitLen;						// real mod len of the curve in bit, u2ModBitLen is not always 8*u2ModLen, such as secp521r1
		u2		m_u2ModLen;							// mod len of the curve in byte, 
													// 计算方式: m_u2ModLen = (0==m_u2ModBitLen%8) ? m_u2ModBitLen/8 : (m_u2ModBitLen+8)/8
		u2		m_u2FieldType;						// either ES_ECC_PRIME_FIELD or ES_ECC_CHARACTERISTIC_TWO_FIELD
		char	m_szP[ES_ECC_MAX_MOD_LEN+1];		// either a prime number or a polynomial
		char	m_szA[ES_ECC_MAX_MOD_LEN+1];		// coefficient a
		char	m_szB[ES_ECC_MAX_MOD_LEN+1];		// coefficient b
		char	m_szGx[ES_ECC_MAX_MOD_LEN+1];		// the x coordinate of the generator
		char	m_szGy[ES_ECC_MAX_MOD_LEN+1];		// the y coordinate of the generator
		char	m_szOrderN[ES_ECC_MAX_MOD_LEN*2+1];	// the order of the group generated by the generator
		u1		m_u1CofactorH;						// the cofactor h
		char	m_szComment[0x41];					// a short description of the curve
	}BlkEccCurve;

	/**
	 *  @brief ECC: 获取算法模长(字节)
	 *
     *  @param[in] u4AlgId ecc算法，详见ES_ASYM_ALG_SM2 or ES_ASYM_ALG_ECC_XXXX
	 *  
	 *  @return 算法模长(字节)，不支持的算法返回0
	 *  
	 *  @details 不支持ES_ASYM_ALG_ECC_SET
	 */
	u4 CALL_TYPE EsEccGetModLen(IN u4 u4AlgId);
	
	/**
	 *  @brief ECC: 获取算法模长(比特)
	 *
     *  @param[in] u4AlgId ecc算法，详见ES_ASYM_ALG_SM2 or ES_ASYM_ALG_ECC_XXXX
	 *  
	 *  @return 算法模长(比特)，不支持的算法返回0
	 *  
	 *  @details 不支持ES_ASYM_ALG_ECC_SET
	 */
	u4 CALL_TYPE EsEccGetModBitLen(IN u4 u4AlgId);

	/**
	 *  @brief ECC: 初始化
	 *
     *  @param[in] u4AlgId ecc算法，详见ES_ASYM_ALG_SM2 or ES_ASYM_ALG_ECC_XXXX
     *  @param[out] phEcc ecc句柄
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details 不支持ES_ASYM_ALG_ECC_SET
	 */
	u4 CALL_TYPE EsEccInit(IN u4 u4AlgId, OUT ESHANDLE* phEcc);

	/**
	 *  @brief ECC: 初始化（自定义曲线参数），
	 *
	 *  @param[in] pblkCurve 曲线
	 *  @param[out] phEcc ecc句柄
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details 初始化过程会自动匹配算法Id，未知曲线对应的算法为ES_ASYM_ALG_ECC_SET
	 */
	u4 CALL_TYPE EsEccInitWithCurve(IN const BlkEccCurve* pblkCurve, OUT ESHANDLE* phEcc);

	/**
	 *  @brief ECC:获取曲线参数
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[out] pu4AlgId 算法id，可以为NULL，表示不需要获取此参数
     *  @param[out] pblkCurve 曲线参数，可以为NULL，表示不需要获取此参数
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccGetCurveParam(IN ESHANDLE hEcc, OUT u4* pu4AlgId, OUT BlkEccCurve* pblkCurve);

	/**
	 *  @brief ECC: 反初始化
	 *
	 *  @param[in,out] phEcc 句柄
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccRelease(ESHANDLE* phEcc);

	/**
	 *  @brief ECC:检查是否是曲线上的点
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[in] pu1Point 点坐标X||Y，长度为2倍模长，大头序
	 *  @param[in] u4PointLen 点坐标数据长度，长度为2倍模长
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccPointCheck(IN ESHANDLE hEcc, IN const u1* pu1Point, IN u4 u4PointLen);

	/**
	 *  @brief ECC:点乘运算
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[in] pu1K 点乘系数K，长度为模长，大头序
	 *  @param[in] pu1PointP 参与点乘的点坐标Px|Py，长度为两倍模长，大头序
	 *  @param[out] pu1PointQ 点乘结果Q的坐标Qx和Qy，长度为两倍模长，大头序
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccPointMul(IN ESHANDLE hEcc, IN const u1* pu1K, IN const u1* pu1PointP, OUT u1* pu1PointQ);

	/**
	 *  @brief ECC:点加运算
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[in] pu1PointP 加数坐标P x|y，长度为两倍模长，大头序
	 *  @param[in] pu1PointQ 被加数坐标Q x|y，长度为两倍模长，大头序
	 *  @param[out] pu1PointR 操作结果坐标R x|y，长度为两倍模长，大头序
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccPointAdd(IN ESHANDLE hEcc, IN const u1* pu1PointP, IN const u1* pu1PointQ, OUT u1* pu1PointR);

	/**
	 *  @brief ECC:由私钥获取公钥
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[in] pu1PriKeyD 私钥D，长度为模长，大头序
	 *  @param[out] pu1PubKey 公钥 x|y，长度为两倍模长，大头序
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccGetPubKey(IN ESHANDLE hEcc, IN const u1* pu1PriKeyD, OUT u1* pu1PubKey);

	/**
	 *  @brief ECC:生成密钥对
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[out] pu1PriKeyD 私钥D，长度为模长，大头序
	 *  @param[out] pu1PubKey 公钥 x|y，长度为两倍模长，大头序
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccGenKey(IN ESHANDLE hEcc, OUT u1* pu1PriKeyD, OUT u1* pu1PubKey);

	/**
	 *  @brief ECC:数据签名
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[in] pu1PriKeyD 私钥，模长，大头序
	 *  @param[in] pu1SignData hash值或者经过补足/截断后签名数据，如果是SM2，则按照SM2规范处理。长度为模长
	 *  @param[out] pu1Signature 签名值 R|S，长度为2倍模长
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details 补足: 长度小于模长，在高位补0到模长。截断: 长度大于模长，取高位模长字节
	 */
	u4 CALL_TYPE EsEccSign(IN ESHANDLE hEcc, IN const u1* pu1PriKeyD, IN const u1* pu1SignData, OUT u1* pu1Signature);

	/**
	 *  @brief ECC:消息签名
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[in] pu1PriKeyD 私钥，模长，大头序
	 *  @param[in] u4HashAlgId hash算法
	 *  @param[in] pu1Msg 消息原文
	 *  @param[in] u4MsgLen 消息原文长度
	 *  @param[out] pu1Signature 签名值 R|S，长度为2倍模长
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details 补足: 长度小于模长，在高位补0到模长。截断: 长度大于模长，取高位模长字节
	 */
	u4 CALL_TYPE EsEccSignMsg(IN ESHANDLE hEcc, IN const u1* pu1PriKeyD, IN u4 u4HashAlgId, IN const u1* pu1Msg, IN u4 u4MsgLen, OUT u1* pu1Signature);

	/**
	 *  @brief ECC:验签
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[in] pu1PubKey 公钥 x|y,长度为2倍模长
	 *  @param[in] pu1SignData hash值或者经过补足/截断后签名数据，如果是SM2，则按照SM2规范处理。长度为模长
	 *  @param[in] pu1Signature 签名值 R|S,长度为2倍模长
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccVerify(IN ESHANDLE hEcc, IN const u1* pu1PubKey, IN const u1* pu1SignData, IN const u1* pu1Signature);

	/**
	 *  @brief ECC:验签
	 *
	 *  @param[in] hEcc ecc句柄
	 *  @param[in] pu1PubKey 公钥 x|y,长度为2倍模长
	 *  @param[in] u4HashAlgId hash算法
	 *  @param[in] pu1Msg 消息原文
	 *  @param[in] u4MsgLen 消息原文长度
	 *  @param[in] pu1Signature 签名值 R|S,长度为2倍模长
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccVerifySignatureMsg(IN ESHANDLE hEcc, IN const u1* pu1PubKey, IN u4 u4HashAlgId, IN const u1* pu1Msg, IN u4 u4MsgLen, IN const u1* pu1Signature);

	//////////////////////////////////////////////////////////////////////////
	// sm9
	//////////////////////////////////////////////////////////////////////////

	// 调用SM9接口前，需要调用此函数（SM9与SM2、比特币钱包的曲线不同）
	u4 CALL_TYPE EsSm9Init();

	u4 CALL_TYPE EsSm9Release();

	u4 CALL_TYPE EsSM9GenMasterSignKey(IN OUT u1 ks[0x20], OUT u1 Ppubs[0x20*4]);
	u4 CALL_TYPE EsSM9GenUserSignKey(IN u1 hid, IN u1 ks[0x20], IN const char IDA[],  OUT u1 dsA[0x20*2]);

	u4 CALL_TYPE EsSM9GenMasterEncKey(IN OUT u1 ke[0x20], OUT u1 Ppube[0x20*2]);
	u4 CALL_TYPE EsSM9GenUserEncKey(IN u1 hid, IN u1 ke[0x20], IN const char IDB[], OUT u1 deB[0x20*4]);

	u4 CALL_TYPE EsSM9Sign(IN u1 Ppubs[0x20*4], IN u1 dsA[0x20*2], 
		IN u1 msg[], IN int msgLen, 
		OUT u1 h[0x20], OUT u1 S[0x20*2]);

	u4 CALL_TYPE EsSM9Verify(IN u1 hid, IN u1 Ppubs[0x20*4], IN const char IDA[],
		IN u1 msg[], IN int msgLen, 
		IN u1 h[0x20], IN u1 S[0x20*2]);

	u4 CALL_TYPE EsSM9VerifyPre(IN u1 hid, IN u1 Ppubs[0x20*4], IN const char IDA[], 
		OUT u1 P[0x20*4]);

	u4 CALL_TYPE EsSM9Verify2(IN u1 Ppubs[0x20*4], IN u1 P[0x20*4], 
		IN u1 msg[], IN int msgLen, 
		IN u1 h[0x20], IN u1 S[0x20*2]);


	u4 CALL_TYPE EsSM9Enc(IN u1 hid, IN u1 Ppube[0x20*2], 
		IN const char IDB[],
		IN u1 msg[], IN int msgLen,
		IN int EncID, IN int blockMode, IN unsigned char IV[], IN int k1Len, IN int k2Len, 
		OUT u1 C[], OUT int *CLen);

	u4 CALL_TYPE EsSM9Dec(IN u1 deB[0x20*4], IN const char IDB[],
		IN u1 C[], IN int CLen, 
		IN int EncID, IN int blockMode, IN unsigned char IV[], IN int k1Len, IN int k2Len, 
		OUT u1 M[], OUT int *MLen);


	u4 CALL_TYPE EsSM9KeyEncap(IN u1 hid, IN u1 Ppube[0x20*2], 
		IN const char IDB[], IN int KLen, 
		OUT u1 C[0x20*2], OUT u1 K[]);

	u4 CALL_TYPE EsSM9KeyDecap(IN const char IDB[], IN u1 deB[0x20*4], IN u1 C[0x20*2], IN int KLen, OUT u1 K[]);


	u4 CALL_TYPE EsSM9KeyExA1(IN u1 hid, IN u1 Ppube[0x20*2], 
		IN const char *IDB, 
		OUT u1 randA[0x20], OUT u1 RA[0x20*2]);

	u4 CALL_TYPE EsSM9KeyExB1(IN u1 hid, IN u1 Ppube[0x20*2], 
		IN const char *IDA, IN u1 RA[0x20*2], 
		IN const char *IDB, IN u1 deB[0x20*4], 
		IN int kLen, 
		OUT u1 randB[0x20], OUT u1 RB[0x20*2], OUT u1 SB[0x20], 
		OUT u1 g1[0x20*0x0C], OUT u1 g2[0x20*0x0C], OUT u1 g3[0x20*0x0C], 
		OUT unsigned char SKB[]);

	u4 CALL_TYPE EsSM9KeyExA2(IN u1 Ppube[0x20*2], 
		IN const char *IDA, IN u1 deA[0x20*4], IN u1 randA[0x20], IN u1 RA[0x20*2],
		IN const char *IDB, IN u1 RB[0x20*2], IN u1 SB[0x20], 
		IN int kLen, OUT unsigned char SKA[], 
		OUT u1 SA[0x20]);

	u4 CALL_TYPE EsSM9KeyExB2(IN const char *IDA, IN const char *IDB, 
		IN u1 g1[0x20*0x0C], IN u1 g2[0x20*0x0C], IN u1 g3[0x20*0x0C],
		IN u1 RA[0x20*2], IN u1 RB[0x20*2], IN u1 SA[0x20]);

#ifdef __cplusplus
};
#endif

#endif
