#ifndef _ESP7_H_
#define _ESP7_H_

#ifdef __cplusplus
extern "C"
{
#endif
    
    /**
     *  @brief P7-封装数字信封（CFCA双证下载）
     *
     *  @param[in] pblkAsymKeyTemp 临时的保护公钥
     *  @param[in] u4SymmAlgId 会话密钥算法Id，详见ES_SYMM_ALG_XXX
     *  @param[in] pblkAsymKey 私钥信息
     *  @param[out] ppu1EnvelopData 数字信封数据，内部分配内存，请使用EsMemFree释放-TLV
     *  @param[out] pu4EnvelopDataLen 数字信封数据长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details  RSA构建的格式为：
     *			  30 xx
     * 				  02 xx version
     * 				  06 xx AsymAlgOid
     * 				  06 xx SymAlgOid
     * 				  04 xx EncryptSymmKey证书
     * 				  04 xx EncryptAsymKey
     *  		  SM2构建的格式为：04 xx EncPrivateKey
     *  		  EncPrivateKey: C1||C3||C2(国密新标准)，C1||C2||C3(国密旧标准)
     */
	u4 CALL_TYPE EsCfcaEnvelopData(IN const BlkAsymKey* pblkAsymKeyTemp,
                                   IN u4 u4SymmAlgId,
                                   IN const BlkAsymKey* pblkAsymKey,
                                   OUT u1** ppu1EnvelopData, OUT u4* pu4EnvelopDataLen);
    
    /**
     *  @brief P7-数字信封解码（CFCA双证下载）
     *
     *  @param[in] pu1EnvelopData CFCA格式的数字信封，格式详见EsCfcaEnvelopData
     *  @param[in] u4EnvelopDataLen 数字信封数据长度
     *  @param[out] pu4AsymAlgId 非对称算法Id，详见ES_ASYM_XXX
     *  @param[out] pu4SymmAlgId 会话密钥算法Id，详见ES_SYMM_ALG_XXX
     *  @param[out] ppu1EncSymmKey 会话密钥密文，内部分配内存，请使用EsMemFree释放
     *  @param[out] pu4EncSymmKeyLen 会话密钥密文长度
     *  @param[out] ppu1EncAsymKey 非对称密钥密文，内部分配内存，请使用EsMemFree释放
     *  @param[out] pu4EncAsymKeyLen 非对称密钥密文长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details  none
     */
    u4 CALL_TYPE EsCfcaEnvelopDataDecode(IN const u1* pu1EnvelopData, IN u4 u4EnvelopDataLen,
                                         OUT u4* pu4AsymAlgId, OUT u4* pu4SymmAlgId,
                                         OUT u1** ppu1EncSymmKey, OUT u4* pu4EncSymmKeyLen,
                                         OUT u1** ppu1EncAsymKey, OUT u4* pu4EncAsymKeyLen);
    
	#define ESP7_TYPE_DATA				0x01
	#define ESP7_TYPE_SIGNED_DATA		0x02
	#define ESP7_TYPE_ENV_DATA			0x03
	#define ESP7_TYPE_SIGNED_ENV_DATA	0x04
	#define ESP7_TYPE_DIGESTED_DATA		0x05
	#define ESP7_TYPE_ENCRYPTED_DATA	0x06
    
    /**
     *  @brief P7-获取数据类型
     *
     *  @param[in] pvData 数据
     *  @param[in] u4DataLen 数据长度
     *  @param[out] pu4P7Type 存储P7类型
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 根据OID进行判断
     */
	u4 CALL_TYPE EsP7GetDataType(IN const void* pvData, IN u4 u4DataLen, OUT u4* pu4P7Type);

    /**
     *  @brief P7-数据编码
     *
     *  @param[in] pvData 数据
     *  @param[in] u4DataLen 数据长度
     *  @param[out] ppu1EncodeData 编码数据，内部分配内存，请使用EsMemFree释放
     *  @param[out] pu4EncodeDataLen 编码数据长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details  构建的格式为：
     *            30 xx
     *                06 xx OID(data)
     *                A0 xx
     *                    04 xx Data
     */
	u4 CALL_TYPE EsP7EncodeData(IN const void* pvData, IN u4 u4DataLen, OUT u1** ppu1EncodeData, OUT u4* pu4EncodeDataLen);
    
    /**
     *  @brief P7-签名数据编码
     *
     *  @param[in] u4AsymAlgId 签名使用的非对称算法，详见ES_ASYM_XXX
     *  @param[in] u4HashAlgId 签名使用的Hash算法，详见ES_HASH_XXX
     *  @param[in] pu1Content 签名数据，为NULL表示Detached签名，否则为Attached签名
     *  @param[in] u4ContentLen 签名数据长度
     *  @param[in] pu1SignerCert 签名所使用的证书
     *  @param[in] pu1Signature 签名值，允许为空
     *  @param[in] u4SignatureLen 签名值长度
     *  @param[out] ppu1EncodeData 编码数据，内部分配内存，请使用EsMemFree释放
     *  @param[out] pu4EncodeDataLen 编码数据长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details  Attach:
     *            30 xx
     *            	  06 xx OID(SignedData) 固定签名方法为:SignedData
     *            	  A0 xx
     *            		  30 xx
     *            			  02 xx Version
     *            			  31 xx
     *            				  30 xx [06 xx HashOid 05 00] (u4HashAlgId)
     *            				  ...
     *            			  30 xx
     *            				  06 xx data
     *            				  A0 xx
     *            						04 xx Content
     *            			  A0 xx Cert
     *            			  31 xx SignerInfo
     *            SignerInfo的结构为:
     *            30 xx
     *            	  02 xx version
     *            	  30 xx Issure (Cert的Issure)
     *            		    sn	   (Cert的sn)
     *            	  30 xx HashOid 05 00 (u4HashAlgId)
     *            	  30 xx SignOid 05 00 (u4AsymAlgId)
     *            	  04 xx Signature
     *            Detach:
     *            30 xx
     *            	  06 xx OID(SignedData) 固定签名方法为:SignedData
     *            	  A0 xx
     *            		  30 xx
     *            			  02 xx Version
     *            			  31 00 或 31 xx
     *            						   30 xx [06 xx HashOid 05 00] (u4HashAlgId)
     *            						   ...
     *            			  30 xx
     *            				  06 xx data
     *            			  A0 xx Cert
     *            			  31 00 或 31 xx SignerInfo
     *            SignerInfo的结构为:
     *            30 xx
     *            	  02 xx version
     *            	  30 xx Issure (Cert的Issure)
     *            		  sn	 (Cert的sn)
     *            	  30 xx HashOid 05 00 (u4HashAlgId)
     *            	  30 xx SignOid 05 00 (u4AsymAlgId)
     *            	  04 xx Signature
     */
	u4 CALL_TYPE EsP7EncodeSignedData(IN u4 u4AsymAlgId,
                                      IN u4 u4HashAlgId,
                                      IN const u1* pu1Content, IN u4 u4ContentLen,
                                      IN const u1* pu1SignerCert,
                                      IN const u1* pu1Signature, IN u4 u4SignatureLen,
                                      OUT u1** ppu1EncodeData, INOUT u4* pu4EncodeDataLen);
    
    /**
     *  @brief P7-签名数据解码
     *
     *  @param[in] pvP7 P7签名数据
     *  @param[in] u4P7Len P7签名数据长度
     *  @param[out] pu1Content 签名数据，支持传入NULL求长度
     *  @param[in,out] pu4ContentLen 签名数据长度
     *  @param[out] pu1SignerCert 签名所使用的证书
     *  @param[out] pu4CertCount 证书个数
     *  @param[out] pu4SignAlgId 签名非对称算法
     *  @param[out] pu4HashAlgId 签名Hash算法
     *  @param[out] pu1Signature 签名值，支持传入NULL求长度
     *  @param[in,out] pu4SignatureLen 缓冲区/签名值长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsP7DecodeSignedData(IN const void* pvP7, IN u4 u4P7Len,
                                      OUT void* pu1Content, INOUT u4* pu4ContentLen,
                                      OUT void* pu1SignerCert, OUT u4* pu4CertCount,
                                      OUT u4* pu4SignAlgId,
                                      OUT u4* pu4HashAlgId,
                                      OUT u1* pu1Signature, INOUT u4* pu4SignatureLen);
    /**
     *  @brief P7-数据解码
     *
     *  @param[in] pvP7Data P7数据，格式详见EsP7EncodeData
     *  @param[in] u4P7DataLen P7数据长度
     *  @param[out] ppu1Data 数据，内部分配内存，请使用EsMemFree释放
     *  @param[out] pu4DataLen 数据长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsP7DecodeData(IN const void* pvP7Data, IN u4 u4P7DataLen, OUT u1** ppu1Data, OUT u4* pu4DataLen);
	u4 CALL_TYPE EsP7Decode(IN const void* pvP7Data, IN u4 u4P7DataLen, OUT u1** ppu1Data, OUT u4* pu4DataLen); // deprecated

	/**
     *  @brief P7-解码加密数据
     *
     *  @param[in] pvP7Data P7数据，格式详见EsP7EncodeEncryptedData
     *  @param[in] u4P7DataLen P7数据长度
	 *  @param[in] pszPsw 保护数据的密码，不允许为空
     *  @param[out] ppu1Data 数据，内部分配内存，请使用EsMemFree释放
     *  @param[out] pu4DataLen 数据长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsP7DecodeEncryptedData(IN const void* pvP7Data, IN u4 u4P7DataLen, const char* pszPsw, OUT u1** ppu1Data, OUT u4* pu4DataLen);
    
#   define ES_CERTINFO_VERSION     (0x00000001)
    
    typedef struct BlkCertInfoTag
    {
        u4			m_u4Version;                // 结构体版本
		u1*			m_pu1DerIssure;				// 颁发者DER编码
		u4			m_u4DerIssureLen;
		u1*			m_pu1DerSubject;			// 颁发给DER编码
		u4			m_u4DerSubjectLen;
		u1*			m_pu1DerSn;
		u4			m_u4DerSnLen;
		u1*         m_pu1DerValidity;           // 有效期DER编码
		u4          m_u4DerValidityLen;
	// 	u4			m_u4SignAsymAlg;			// 给证书签名所用的非对称算法
	// 	u4			m_u4SignHashAlg;			// 给证书签名所用的哈希算法
	// 	u1			m_pu1Signature[0x200];		// 证书的签名值
	// 	u4			m_u4SignatureLen;
	// 	u1			m_pu1HashValue[0x80];		// 证书主体的哈希值
		BlkAsymKey	m_blkPubKey;				// 证书公钥
	
		u1*			m_pu1DerExtInfo;			// 证书扩展信息DER编码-结构体版本>=1有效
		u4			m_u4DerExtInfoLen;          // 证书扩展信息DER编码长度-结构体版本>=1有效
	}BlkCertInfo;
    
    /**
     *  @brief 证书-X509公钥证书解析
     *
     *  @param[in] pvCertData X509证书
     *  @param[in] u4CertDataLen 证书数据长度
     *  @param[in] pblkAsymKeyCa 证书颁发者公钥，如果为NULL，表示不需要验证证书签名
     *  @param[in] bCalcZa ESTRUE-计算Z值，SM2适用
     *  @param[in,out] pblkCertInfo 证书信息，输入必须对m_u4Version成员赋值
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertParse(IN const void* pvCertData, IN u4 u4CertDataLen, IN const BlkAsymKey* pblkAsymKeyCa, IN ESBOOL bCalcZa, INOUT BlkCertInfo* pblkCertInfo);
    
    /**
     *  @brief 证书-证书DN解码
     *
     *  @param[in] pvDerDn DER编码的DN数据
     *  @param[in] u4DerDnLen DN数据长度
     *  @param[out] pszDn 证书DN，支持NULL求长度
     *  @param[in,out] pu4DnLen DN缓冲区长度/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertParseDn(IN const void* pvDerDn, IN u4 u4DerDnLen, OUT char* pszDn, INOUT u4* pu4DnLen);
    
    /**
     *  @brief 证书-证书DN编码
     *
     *  @param[in] pszDn DN，格式为"CN=XX,E=XXX,OU=XXX"
     *  @param[out] pvDerDn DER编码证书DN，支持NULL求长度
     *  @param[in,out] pu4DerDnLen DER编码证书DN缓冲区长度/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertEncodeDn(IN const char* pszDn, OUT void* pvDerDn, INOUT u4* pu4DerDnLen);
    
    /**
     *  @brief 证书-证书有效期解码
     *
     *  @param[in] pvDerValidity DER编码的有效期数据
     *  @param[in] u4DerLen 有效期数据长度
     *  @param[out] pszNotBefore 生效时间，支持NULL求长度
     *  @param[in,out] pu4NotBeforeLen 生效时间缓冲区长度/结果长度--为NULL则不获取结果
     *  @param[out] pszNotAfter 过期时间，支持NULL求长度
     *  @param[in,out] pu4NotAfterLen 过期时间缓冲区长度/结果长度--为NULL则不获取结果
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertParseValidity(IN const void* pvDerValidity, IN u4 u4DerLen, OUT char* pszNotBefore, INOUT u4* pu4NotBeforeLen, OUT char* pszNotAfter, INOUT u4* pu4NotAfterLen);
    
    /**
     *  @brief 证书-证书SN解码
     *
     *  @param[in] pvDerSn DER编码的SN数据
     *  @param[in] u4DerSnLen SN数据长度
     *  @param[out] pu1Sn 证书SN，支持NULL求长度
     *  @param[in,out] pu4SnLen SN缓冲区长度/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertParseSn(IN const void* pvDerSn, IN u4 u4DerSnLen, OUT u1* pu1Sn, INOUT u4* pu4SnLen);
    
    /**
     *  @brief 证书CA-设置CA参数--接口慎用!!!
     *
     *  @param[in] pblkAsymKey CA公钥
     *  @param[in] pszDnCa CA的DN
     *  @param[in] u4DnCaLen CA的DN长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details !!接口慎用，内部使用了全局变量存储CA信息!!
     */
	u4 CALL_TYPE EsCaSetParam(IN const BlkAsymKey* pblkAsymKey, IN const char* pszDnCa, IN u4 u4DnCaLen);
	
	// 默认两种算法都支持
	#define ESCERT_TYPE_EXCHANGE	0x01 // 交换证书
	#define ESCERT_TYPE_SIGNATURE	0x02 // 签名证书
	#define ESCERT_TYPE_SIG_EX		0x03 // 两者证书
    /**
     *  @brief 证书CA-颁发证书
     *
     *  @param[in] pblkAsymKey 证书公钥
     *  @param[in] pszDn DN
     *  @param[in] u4DnLen DN长度
     *  @param[in] u4CertType 证书类型，详见ESCERT_TYPE_XXX
     *  @param[out] pvDerCert 证书
     *  @param[out] pu4DerCertLen 证书长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details !!接口慎用，内部使用了全局变量存储CA信息!!
     */
	u4 CALL_TYPE EsCertConstruct(IN const BlkAsymKey* pblkAsymKey,
                                 IN const char* pszDn, IN u4 u4DnLen,
                                 IN u4 u4CertType,
                                 OUT void* pvDerCert, OUT u4* pu4DerCertLen);
    
    /**
     *  @brief 证书CA-颁发证书
     *
     *  @param[in] pblkAsymKeyCa CA私钥
     *  @param[in] pu1DerCertCa CA DER编码证书
     *  @param[in] u4DerCertCaLen CA DER编码证书长度
     *  @param[in] pblkAsymKey 证书公钥
     *  @param[in] pu1DerDn 证书DN
     *  @param[in] u4DerDnLen 证书DN长度
     *  @param[in] u4CertType 证书类型，详见ESCERT_TYPE_XXX
     *  @param[out] pvDerCert 证书
     *  @param[out] pu4DerCertLen 证书长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertConstructEx(IN const BlkAsymKey* pblkAsymKeyCa, IN const u1* pu1DerCertCa, IN u4 u4DerCertCaLen,
                                   IN const BlkAsymKey* pblkAsymKey,
                                   IN const u1* pu1DerDn, IN u4 u4DerDnLen,
                                   IN u4 u4CertType,
                                   OUT void* pvDerCert, OUT u4* pu4DerCertLen);
	
	typedef enum EnKeyUsageInCertTag // 证书扩展信息里的密钥用法属性
	{
		EN_KEY_USAGE_DIGITAL_SIGN		= 0x01,		//!< digitalSignature
		EN_KEY_USAGE_NON_REPUDIATION	= 0x02,		//!< nonRepudiation
		EN_KEY_USAGE_KEY_ENC			= 0x04,		//!< keyEncipherment
		EN_KEY_USAGE_DATA_ENC			= 0x08,		//!< dataEncipherment
		EN_KEY_USAGE_KEY_AGREEMENT		= 0x10,		//!< keyAgreement
		EN_KEY_USAGE_KEY_CERT_SIGN		= 0x20,		//!< keyCertSign
		EN_KEY_USAGE_CRL_SIGN			= 0x40,		//!< cRLSign
		EN_KEY_USAGE_ENCIPHER_ONLY		= 0x80,		//!< encipherOnly
		EN_KEY_USAGE_DECIPHER_ONLY		= 0x100,	//!< decipherOnly
	}EnKeyUsageInCert;
	
    /**
     *  @brief 证书-获取密钥用法
     *
     *  @param[in] pvDerExtInfo DER编码的证书扩展信息，来自BlkCertInfo
     *  @param[in] u4DerExtInfoLen DER编码的证书扩展信息长度
     *  @param[out] pu4KeyUsage 输出密钥用法，详见EnKeyUsageInCert
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertParseKeyUsage(IN const void* pvDerExtInfo, IN u4 u4DerExtInfoLen, OUT u4* pu4KeyUsage);

	// Cert
	typedef enum EN_CERT_PARAM
	{
		CERT_PARAM_VALID_TIME_BEGIN = 0,	// YYMMDDHHMMSS
		CERT_PARAM_VALID_TIME_END = 1,		// YYMMDDHHMMSS
		CERT_PARAM_SIGN_ALG = 2,			// [4] ES_ASYM_ALG_XXX
	}EN_CERT_PARAM;

    /**
     *  @brief 证书-获取证书的相关信息
     *
     *  @param[in] pu1Cert x509证书数据
     *  @param[in] enCertParam 指定的参数，详见EN_CERT_PARAM
     *  @param[out] pu1Value 存储获取到的结果，支持传入NULL求长度
     *  @param[in,out] pu4ValueLen 入参为pu1Value缓冲区大小，出参为证书参数大小
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertGetParam(IN const u1* pu1Cert, IN EN_CERT_PARAM enCertParam, OUT u1* pu1Value, INOUT u4* pu4ValueLen);

#ifdef __cplusplus
};
#endif

#endif // !defined _ESP7_H_
