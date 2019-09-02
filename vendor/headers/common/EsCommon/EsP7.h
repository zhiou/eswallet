#ifndef _ESP7_H_
#define _ESP7_H_

#ifdef __cplusplus
extern "C"
{
#endif
    
    /**
     *  @brief P7-��װ�����ŷ⣨CFCA˫֤���أ�
     *
     *  @param[in] pblkAsymKeyTemp ��ʱ�ı�����Կ
     *  @param[in] u4SymmAlgId �Ự��Կ�㷨Id�����ES_SYMM_ALG_XXX
     *  @param[in] pblkAsymKey ˽Կ��Ϣ
     *  @param[out] ppu1EnvelopData �����ŷ����ݣ��ڲ������ڴ棬��ʹ��EsMemFree�ͷ�-TLV
     *  @param[out] pu4EnvelopDataLen �����ŷ����ݳ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details  RSA�����ĸ�ʽΪ��
     *			  30 xx
     * 				  02 xx version
     * 				  06 xx AsymAlgOid
     * 				  06 xx SymAlgOid
     * 				  04 xx EncryptSymmKey֤��
     * 				  04 xx EncryptAsymKey
     *  		  SM2�����ĸ�ʽΪ��04 xx EncPrivateKey
     *  		  EncPrivateKey: C1||C3||C2(�����±�׼)��C1||C2||C3(���ܾɱ�׼)
     */
	u4 CALL_TYPE EsCfcaEnvelopData(IN const BlkAsymKey* pblkAsymKeyTemp,
                                   IN u4 u4SymmAlgId,
                                   IN const BlkAsymKey* pblkAsymKey,
                                   OUT u1** ppu1EnvelopData, OUT u4* pu4EnvelopDataLen);
    
    /**
     *  @brief P7-�����ŷ���루CFCA˫֤���أ�
     *
     *  @param[in] pu1EnvelopData CFCA��ʽ�������ŷ⣬��ʽ���EsCfcaEnvelopData
     *  @param[in] u4EnvelopDataLen �����ŷ����ݳ���
     *  @param[out] pu4AsymAlgId �ǶԳ��㷨Id�����ES_ASYM_XXX
     *  @param[out] pu4SymmAlgId �Ự��Կ�㷨Id�����ES_SYMM_ALG_XXX
     *  @param[out] ppu1EncSymmKey �Ự��Կ���ģ��ڲ������ڴ棬��ʹ��EsMemFree�ͷ�
     *  @param[out] pu4EncSymmKeyLen �Ự��Կ���ĳ���
     *  @param[out] ppu1EncAsymKey �ǶԳ���Կ���ģ��ڲ������ڴ棬��ʹ��EsMemFree�ͷ�
     *  @param[out] pu4EncAsymKeyLen �ǶԳ���Կ���ĳ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
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
     *  @brief P7-��ȡ��������
     *
     *  @param[in] pvData ����
     *  @param[in] u4DataLen ���ݳ���
     *  @param[out] pu4P7Type �洢P7����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ����OID�����ж�
     */
	u4 CALL_TYPE EsP7GetDataType(IN const void* pvData, IN u4 u4DataLen, OUT u4* pu4P7Type);

    /**
     *  @brief P7-���ݱ���
     *
     *  @param[in] pvData ����
     *  @param[in] u4DataLen ���ݳ���
     *  @param[out] ppu1EncodeData �������ݣ��ڲ������ڴ棬��ʹ��EsMemFree�ͷ�
     *  @param[out] pu4EncodeDataLen �������ݳ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details  �����ĸ�ʽΪ��
     *            30 xx
     *                06 xx OID(data)
     *                A0 xx
     *                    04 xx Data
     */
	u4 CALL_TYPE EsP7EncodeData(IN const void* pvData, IN u4 u4DataLen, OUT u1** ppu1EncodeData, OUT u4* pu4EncodeDataLen);
    
    /**
     *  @brief P7-ǩ�����ݱ���
     *
     *  @param[in] u4AsymAlgId ǩ��ʹ�õķǶԳ��㷨�����ES_ASYM_XXX
     *  @param[in] u4HashAlgId ǩ��ʹ�õ�Hash�㷨�����ES_HASH_XXX
     *  @param[in] pu1Content ǩ�����ݣ�ΪNULL��ʾDetachedǩ��������ΪAttachedǩ��
     *  @param[in] u4ContentLen ǩ�����ݳ���
     *  @param[in] pu1SignerCert ǩ����ʹ�õ�֤��
     *  @param[in] pu1Signature ǩ��ֵ������Ϊ��
     *  @param[in] u4SignatureLen ǩ��ֵ����
     *  @param[out] ppu1EncodeData �������ݣ��ڲ������ڴ棬��ʹ��EsMemFree�ͷ�
     *  @param[out] pu4EncodeDataLen �������ݳ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details  Attach:
     *            30 xx
     *            	  06 xx OID(SignedData) �̶�ǩ������Ϊ:SignedData
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
     *            SignerInfo�ĽṹΪ:
     *            30 xx
     *            	  02 xx version
     *            	  30 xx Issure (Cert��Issure)
     *            		    sn	   (Cert��sn)
     *            	  30 xx HashOid 05 00 (u4HashAlgId)
     *            	  30 xx SignOid 05 00 (u4AsymAlgId)
     *            	  04 xx Signature
     *            Detach:
     *            30 xx
     *            	  06 xx OID(SignedData) �̶�ǩ������Ϊ:SignedData
     *            	  A0 xx
     *            		  30 xx
     *            			  02 xx Version
     *            			  31 00 �� 31 xx
     *            						   30 xx [06 xx HashOid 05 00] (u4HashAlgId)
     *            						   ...
     *            			  30 xx
     *            				  06 xx data
     *            			  A0 xx Cert
     *            			  31 00 �� 31 xx SignerInfo
     *            SignerInfo�ĽṹΪ:
     *            30 xx
     *            	  02 xx version
     *            	  30 xx Issure (Cert��Issure)
     *            		  sn	 (Cert��sn)
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
     *  @brief P7-ǩ�����ݽ���
     *
     *  @param[in] pvP7 P7ǩ������
     *  @param[in] u4P7Len P7ǩ�����ݳ���
     *  @param[out] pu1Content ǩ�����ݣ�֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4ContentLen ǩ�����ݳ���
     *  @param[out] pu1SignerCert ǩ����ʹ�õ�֤��
     *  @param[out] pu4CertCount ֤�����
     *  @param[out] pu4SignAlgId ǩ���ǶԳ��㷨
     *  @param[out] pu4HashAlgId ǩ��Hash�㷨
     *  @param[out] pu1Signature ǩ��ֵ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4SignatureLen ������/ǩ��ֵ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
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
     *  @brief P7-���ݽ���
     *
     *  @param[in] pvP7Data P7���ݣ���ʽ���EsP7EncodeData
     *  @param[in] u4P7DataLen P7���ݳ���
     *  @param[out] ppu1Data ���ݣ��ڲ������ڴ棬��ʹ��EsMemFree�ͷ�
     *  @param[out] pu4DataLen ���ݳ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsP7DecodeData(IN const void* pvP7Data, IN u4 u4P7DataLen, OUT u1** ppu1Data, OUT u4* pu4DataLen);
	u4 CALL_TYPE EsP7Decode(IN const void* pvP7Data, IN u4 u4P7DataLen, OUT u1** ppu1Data, OUT u4* pu4DataLen); // deprecated

	/**
     *  @brief P7-�����������
     *
     *  @param[in] pvP7Data P7���ݣ���ʽ���EsP7EncodeEncryptedData
     *  @param[in] u4P7DataLen P7���ݳ���
	 *  @param[in] pszPsw �������ݵ����룬������Ϊ��
     *  @param[out] ppu1Data ���ݣ��ڲ������ڴ棬��ʹ��EsMemFree�ͷ�
     *  @param[out] pu4DataLen ���ݳ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsP7DecodeEncryptedData(IN const void* pvP7Data, IN u4 u4P7DataLen, const char* pszPsw, OUT u1** ppu1Data, OUT u4* pu4DataLen);
    
#   define ES_CERTINFO_VERSION     (0x00000001)
    
    typedef struct BlkCertInfoTag
    {
        u4			m_u4Version;                // �ṹ��汾
		u1*			m_pu1DerIssure;				// �䷢��DER����
		u4			m_u4DerIssureLen;
		u1*			m_pu1DerSubject;			// �䷢��DER����
		u4			m_u4DerSubjectLen;
		u1*			m_pu1DerSn;
		u4			m_u4DerSnLen;
		u1*         m_pu1DerValidity;           // ��Ч��DER����
		u4          m_u4DerValidityLen;
	// 	u4			m_u4SignAsymAlg;			// ��֤��ǩ�����õķǶԳ��㷨
	// 	u4			m_u4SignHashAlg;			// ��֤��ǩ�����õĹ�ϣ�㷨
	// 	u1			m_pu1Signature[0x200];		// ֤���ǩ��ֵ
	// 	u4			m_u4SignatureLen;
	// 	u1			m_pu1HashValue[0x80];		// ֤������Ĺ�ϣֵ
		BlkAsymKey	m_blkPubKey;				// ֤�鹫Կ
	
		u1*			m_pu1DerExtInfo;			// ֤����չ��ϢDER����-�ṹ��汾>=1��Ч
		u4			m_u4DerExtInfoLen;          // ֤����չ��ϢDER���볤��-�ṹ��汾>=1��Ч
	}BlkCertInfo;
    
    /**
     *  @brief ֤��-X509��Կ֤�����
     *
     *  @param[in] pvCertData X509֤��
     *  @param[in] u4CertDataLen ֤�����ݳ���
     *  @param[in] pblkAsymKeyCa ֤��䷢�߹�Կ�����ΪNULL����ʾ����Ҫ��֤֤��ǩ��
     *  @param[in] bCalcZa ESTRUE-����Zֵ��SM2����
     *  @param[in,out] pblkCertInfo ֤����Ϣ����������m_u4Version��Ա��ֵ
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertParse(IN const void* pvCertData, IN u4 u4CertDataLen, IN const BlkAsymKey* pblkAsymKeyCa, IN ESBOOL bCalcZa, INOUT BlkCertInfo* pblkCertInfo);
    
    /**
     *  @brief ֤��-֤��DN����
     *
     *  @param[in] pvDerDn DER�����DN����
     *  @param[in] u4DerDnLen DN���ݳ���
     *  @param[out] pszDn ֤��DN��֧��NULL�󳤶�
     *  @param[in,out] pu4DnLen DN����������/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertParseDn(IN const void* pvDerDn, IN u4 u4DerDnLen, OUT char* pszDn, INOUT u4* pu4DnLen);
    
    /**
     *  @brief ֤��-֤��DN����
     *
     *  @param[in] pszDn DN����ʽΪ"CN=XX,E=XXX,OU=XXX"
     *  @param[out] pvDerDn DER����֤��DN��֧��NULL�󳤶�
     *  @param[in,out] pu4DerDnLen DER����֤��DN����������/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertEncodeDn(IN const char* pszDn, OUT void* pvDerDn, INOUT u4* pu4DerDnLen);
    
    /**
     *  @brief ֤��-֤����Ч�ڽ���
     *
     *  @param[in] pvDerValidity DER�������Ч������
     *  @param[in] u4DerLen ��Ч�����ݳ���
     *  @param[out] pszNotBefore ��Чʱ�䣬֧��NULL�󳤶�
     *  @param[in,out] pu4NotBeforeLen ��Чʱ�仺��������/�������--ΪNULL�򲻻�ȡ���
     *  @param[out] pszNotAfter ����ʱ�䣬֧��NULL�󳤶�
     *  @param[in,out] pu4NotAfterLen ����ʱ�仺��������/�������--ΪNULL�򲻻�ȡ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertParseValidity(IN const void* pvDerValidity, IN u4 u4DerLen, OUT char* pszNotBefore, INOUT u4* pu4NotBeforeLen, OUT char* pszNotAfter, INOUT u4* pu4NotAfterLen);
    
    /**
     *  @brief ֤��-֤��SN����
     *
     *  @param[in] pvDerSn DER�����SN����
     *  @param[in] u4DerSnLen SN���ݳ���
     *  @param[out] pu1Sn ֤��SN��֧��NULL�󳤶�
     *  @param[in,out] pu4SnLen SN����������/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertParseSn(IN const void* pvDerSn, IN u4 u4DerSnLen, OUT u1* pu1Sn, INOUT u4* pu4SnLen);
    
    /**
     *  @brief ֤��CA-����CA����--�ӿ�����!!!
     *
     *  @param[in] pblkAsymKey CA��Կ
     *  @param[in] pszDnCa CA��DN
     *  @param[in] u4DnCaLen CA��DN����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details !!�ӿ����ã��ڲ�ʹ����ȫ�ֱ����洢CA��Ϣ!!
     */
	u4 CALL_TYPE EsCaSetParam(IN const BlkAsymKey* pblkAsymKey, IN const char* pszDnCa, IN u4 u4DnCaLen);
	
	// Ĭ�������㷨��֧��
	#define ESCERT_TYPE_EXCHANGE	0x01 // ����֤��
	#define ESCERT_TYPE_SIGNATURE	0x02 // ǩ��֤��
	#define ESCERT_TYPE_SIG_EX		0x03 // ����֤��
    /**
     *  @brief ֤��CA-�䷢֤��
     *
     *  @param[in] pblkAsymKey ֤�鹫Կ
     *  @param[in] pszDn DN
     *  @param[in] u4DnLen DN����
     *  @param[in] u4CertType ֤�����ͣ����ESCERT_TYPE_XXX
     *  @param[out] pvDerCert ֤��
     *  @param[out] pu4DerCertLen ֤�鳤��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details !!�ӿ����ã��ڲ�ʹ����ȫ�ֱ����洢CA��Ϣ!!
     */
	u4 CALL_TYPE EsCertConstruct(IN const BlkAsymKey* pblkAsymKey,
                                 IN const char* pszDn, IN u4 u4DnLen,
                                 IN u4 u4CertType,
                                 OUT void* pvDerCert, OUT u4* pu4DerCertLen);
    
    /**
     *  @brief ֤��CA-�䷢֤��
     *
     *  @param[in] pblkAsymKeyCa CA˽Կ
     *  @param[in] pu1DerCertCa CA DER����֤��
     *  @param[in] u4DerCertCaLen CA DER����֤�鳤��
     *  @param[in] pblkAsymKey ֤�鹫Կ
     *  @param[in] pu1DerDn ֤��DN
     *  @param[in] u4DerDnLen ֤��DN����
     *  @param[in] u4CertType ֤�����ͣ����ESCERT_TYPE_XXX
     *  @param[out] pvDerCert ֤��
     *  @param[out] pu4DerCertLen ֤�鳤��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertConstructEx(IN const BlkAsymKey* pblkAsymKeyCa, IN const u1* pu1DerCertCa, IN u4 u4DerCertCaLen,
                                   IN const BlkAsymKey* pblkAsymKey,
                                   IN const u1* pu1DerDn, IN u4 u4DerDnLen,
                                   IN u4 u4CertType,
                                   OUT void* pvDerCert, OUT u4* pu4DerCertLen);
	
	typedef enum EnKeyUsageInCertTag // ֤����չ��Ϣ�����Կ�÷�����
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
     *  @brief ֤��-��ȡ��Կ�÷�
     *
     *  @param[in] pvDerExtInfo DER�����֤����չ��Ϣ������BlkCertInfo
     *  @param[in] u4DerExtInfoLen DER�����֤����չ��Ϣ����
     *  @param[out] pu4KeyUsage �����Կ�÷������EnKeyUsageInCert
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
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
     *  @brief ֤��-��ȡ֤��������Ϣ
     *
     *  @param[in] pu1Cert x509֤������
     *  @param[in] enCertParam ָ���Ĳ��������EN_CERT_PARAM
     *  @param[out] pu1Value �洢��ȡ���Ľ����֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4ValueLen ���Ϊpu1Value��������С������Ϊ֤�������С
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsCertGetParam(IN const u1* pu1Cert, IN EN_CERT_PARAM enCertParam, OUT u1* pu1Value, INOUT u4* pu4ValueLen);

#ifdef __cplusplus
};
#endif

#endif // !defined _ESP7_H_
