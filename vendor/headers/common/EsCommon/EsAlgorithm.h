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
    // SHA3����keccak�㷨����2015��8��NIST���SHA3��׼��ʱ����������㷨
    #define ES_HASH_ALG_SHA3224         0x80000009
    #define ES_HASH_ALG_SHA3256         0x8000000A
    #define ES_HASH_ALG_SHA3384         0x8000000B
    #define ES_HASH_ALG_SHA3512         0x8000000C
    #define ES_HASH_ALG_KECCAK224       0x8000000D
    #define ES_HASH_ALG_KECCAK256       0x8000000E
    #define ES_HASH_ALG_KECCAK384       0x8000000F
    #define ES_HASH_ALG_KECCAK512       0x80000010
	#define ES_HASH_ALG_SET				0x800000FF	// ֻ��ͨ�����ýӿ����ù�ϣֵ

    /**
	 *  @brief Hash-��ʼ��
	 *
	 *  @param[out] phHash �洢Hash���
	 *  @param[in] pvIv ��ʼֵ������ΪNULL����Ч���ݳ�����Hash�㷨�������һ��
	 *  @param[in] u4HashAlgId �㷨Id�����ES_HASH_ALG_XXX
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashInit(OUT ESHANDLE* phHash, IN const void* pvIv, IN u4 u4HashAlgId);
    
    /**
	 *  @brief Hash-��������
	 *
	 *  @param[in] hHash Hash���
	 *  @param[in] pvInput ����������
	 *  @param[in] u4InputLen ���������ݳ���
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashUpdate(IN ESHANDLE hHash, IN const void *pvInput, IN u4 u4InputLen);
    
    /**
	 *  @brief Hash-��������
	 *
	 *  @param[in] hHash Hash���
	 *  @param[out] pvHashValue �洢������������������ȿ��Ե���EsHashGetInfo��ȡ
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashFinal(IN ESHANDLE hHash, OUT void* pvHashValue);
    
    /**
	 *  @brief Hash-�ͷž��
	 *
	 *  @param[in] hHash Hash���
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashRelease(IN ESHANDLE hHash);
    
    /**
	 *  @brief Hash-��ȡ��Ϣ
	 *
	 *  @param[in] hHash Hash���
	 *  @param[out] pu4BlockSize �����С������ΪNULL
	 *  @param[out] pu4IvSize IV��С������ΪNULL
	 *  @param[out] pu4ValueSize �����С������ΪNULL
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
    u4 CALL_TYPE EsHashGetInfo(IN ESHANDLE hHash, OUT u4* pu4BlockSize, OUT u4* pu4IvSize, OUT u4* pu4ValueSize);
    u4 CALL_TYPE EsHashGetParam(IN u4 u4AlgId, OUT u4* pu4BlockSize, OUT u4* pu4IvSize, OUT u4* pu4ValueSize);
    
    /**
	 *  @brief Hash-��ȡ�����Ӧ��Hash�㷨Id
	 *
	 *  @param[in] hHash Hash���
	 *  @param[out] pu4AlgId �㷨Id��������ΪNULL
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashGetAlgId(IN ESHANDLE hHash, u4* pu4AlgId);

    /**
	 *  @brief Hash-��ȡ�м�ֵ
	 *
	 *  @param[in] hHash Hash���
	 *  @param[out] pvMidValue Hash�м�ֵ��������IV����һ�£������߱�֤�㹻�Ļ���������ͨ��EsHashGetInfo��ȡ
	 *  @param[out] pu4DataLen �ѽ���Hash�����ݳ���(�������ΪEsHashUpdate�ۼƵ����ݳ���)������ΪNULL
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashGetMidValue(IN ESHANDLE hHash, OUT void* pvMidValue, OUT u4* pu4DataLen);
    
    /**
	 *  @brief Hash-�����м�ֵ
	 *
	 *  @param[in] hHash Hash���
	 *  @param[in] pvMidValue �м�ֵ��������IV����һ��
	 *  @param[in] u4DataLen �ѽ���Hash�����ݳ��ȣ����Ϊ0���൱������Ϊ��ʼֵ
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsHashSetMidValue(IN ESHANDLE hHash, IN const void* pvMidValue, IN u4 u4DataLen);
    
    /**
	 *  @brief Hash-��ȡHashOid
	 *
	 *  @param[in] u4AlgId �㷨Id�����ES_HASH_ALG_XXX
	 *  @param[out] pu1Oid Oid
	 *
	 *  @return Oid���ȣ�ʧ��ʱ����0
	 *
	 *  @details none
	 */
	u1 CALL_TYPE EsHashGetOid(IN u4 u4AlgId, OUT u1* pu1Oid);
    
    /**
	 *  @brief Hash-��Ϣ����
	 *
	 *  @param[in] u4HashAlgId �㷨Id�����ES_HASH_ALG_XXX
	 *  @param[in] pvMsg ���������Ϣ����
	 *  @param[in] u4MsgLen ���������Ϣ���ݳ���
	 *  @param[out] pvHashValue �洢Hash���
	 *
	 *  @return Hash���ȣ�ʧ��ʱ����0
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

	#define ES_OP_TYPE_ENC		(1) // ����
	#define ES_OP_TYPE_DEC		(0) // ����

    /**
     *  @brief �Գ��㷨: ����/���ܲ����ص��ӿڣ���������������
	 *
	 *  @param [in] u4AlgId �Գ��㷨Id�����ES_SYMM_ALG_XXX
	 *  @param [in] u4Mode �Գ��㷨ģʽ�����ES_SYMM_ALG_MODE_XXX������ES_SYMM_ALG_MODE_MAC���
	 *  @param [in] u4OperationType ����ģʽ��ES_OP_TYPE_ENC/ES_OP_TYPE_DEC
	 *  @param [in] pvKey �Գ���Կ���ڲ��Զ���ȡ���Ӧ���㷨��ƥ��ĳ��ȵ��ڴ�
	 *  @param [in] pvInput �������ݣ����ģ����������в���
	 *  @param [in] u4InputLen �������ݳ��ȣ�������������ı���
     *  @param [out] pvOutput ���ģ�֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4OutputLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
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
     *  @brief �Գ��㷨: �����㷨����/�����ⲿʵ��(Ŀǰ��֧������SM1/SSF33�㷨)
	 *
	 *  @param [in] u4AlgId �Գ��㷨Id�����ES_SYMM_ALG_XXX
	 *  @param [in] u4ModeFlag �Գ��㷨ģʽ��ǣ����ES_SYMM_ALG_MODE_XXX�����֧�ֶ����ǣ���ʹ��|���������
	 *  @param [in] funSymmOperation ���ΪNULL��ʾȡ��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details �ص��ӿڱ���ͬʱ֧�ּ��ܺͽ��ܡ������̰߳�ȫ�����������б�֤
     */
	u4 CALL_TYPE EsSymmSetExternImpl(IN u4 u4AlgId, IN u4 u4ModeFlag, IN FunSymmOperation funSymmOperation);

    /**
     *  @brief �Գ��㷨: ��ȡ�㷨��Ϣ��������鳤�ȣ���Կ����
     *
     *  @param [in] u4AlgId �Գ��㷨Id�����ES_SYMM_ALG_XXX������RC4�㷨��
     *  @param [out] pu4BlockLen ���鳤�ȣ�����ΪNULL
     *  @param [out] pu4KeyLen ��Կ���ȣ�����ΪNULL
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmGetInfo(IN u4 u4AlgId, OUT u4* pu4BlockLen, OUT u4* pu4KeyLen);

    /**
     *  @brief �Գ��㷨: ECB����
	 *
	 *  @param [in] u4AlgId �Գ��㷨Id�����ES_SYMM_ALG_XXX
	 *  @param [in] pvKey �Գ���Կ���ڲ��Զ���ȡ���Ӧ���㷨��ƥ��ĳ��ȵ��ڴ�
	 *  @param [in] pvInput �������ݣ����ģ����������в���
	 *  @param [in] u4InputLen �������ݳ��ȣ�������������ı���
     *  @param [out] pvOutput ���ģ�֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4OutputLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmEncEcb(IN u4 u4AlgId,
                              IN const void* pvKey,
                              IN const void* pvInput, IN u4 u4InputLen,
                              OUT void* pvOutput, INOUT u4* pu4OutputLen);
    
    /**
     *  @brief �Գ��㷨: ECB����
     *
     *  @param [in] u4AlgId �Գ��㷨Id�����ES_SYMM_ALG_XXX
     *  @param [in] pvKey �Գ���Կ���ڲ��Զ���ȡ���Ӧ���㷨��ƥ��ĳ��ȵ��ڴ�
     *  @param [in] pvInput �������ݣ�����
     *  @param [in] u4InputLen �������ݳ��ȣ�������������ı���
     *  @param [out] pvOutput ���ģ��ڲ���ȥ���㣬֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4OutputLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmDecEcb(IN u4 u4AlgId,
                              IN const void* pvKey,
                              IN const void* pvInput, IN u4 u4InputLen,
                              OUT void* pvOutput, INOUT u4* pu4OutputLen);
    
    /**
     *  @brief �Գ��㷨: CBC����
     *
     *  @param [in] u4AlgId �Գ��㷨Id�����ES_SYMM_ALG_XXX
     *  @param [in] pvKey �Գ���Կ���ڲ��Զ���ȡ���Ӧ���㷨��ƥ��ĳ��ȵ��ڴ�
     *  @param [in] pvIv ��ʼֵ����pvKey������ͬ
     *  @param [in] pvInput �������ݣ����ģ����������в���
     *  @param [in] u4InputLen �������ݳ��ȣ�������������ı���
     *  @param [out] pvOutput ���ģ�֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4OutputLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmEncCbc(IN u4 u4AlgId,
                              IN const void* pvKey,
                              IN const void* pvIv,
                              IN const void* pvInput, IN u4 u4InputLen,
                              OUT void* pvOutput, INOUT u4* pu4OutputLen);
    
    /**
     *  @brief �Գ��㷨: CBC����
     *
     *  @param [in] u4AlgId �Գ��㷨Id�����ES_SYMM_ALG_XXX
     *  @param [in] pvKey �Գ���Կ���ڲ��Զ���ȡ���Ӧ���㷨��ƥ��ĳ��ȵ��ڴ�
     *  @param [in] pvIv ��ʼֵ����pvKey������ͬ
     *  @param [in] pvInput �������ݣ�����
     *  @param [in] u4InputLen �������ݳ��ȣ�������������ı���
     *  @param [out] pvOutput ���ģ��ڲ���ȥ���㣬֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4OutputLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmDecCbc(IN u4 u4AlgId,
                              IN const void* pvKey,
                              IN const void* pvIv,
                              IN const void* pvInput, IN u4 u4InputLen,
                              OUT void* pvOutput, INOUT u4* pu4OutputLen);
    /**
     *  @brief �Գ��㷨: ����MAC��
     *
     *  @param [in] u4AlgId �Գ��㷨Id�����ES_SYMM_ALG_XXX
     *  @param [in] pvKey �Գ���Կ���ڲ��Զ���ȡ���Ӧ���㷨��ƥ��ĳ��ȵ��ڴ�
     *  @param [in] pvIv ��ʼֵ����pvKey������ͬ
     *  @param [in] pvInput �������ݣ����ģ�
     *  @param [in] u4InputLen �������ݳ��ȣ����ݳ��ȿ��Բ��������飬�ڲ����Զ�����
     *  @param [out] pvOutput ���ģ�4�ֽڣ���֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4OutputLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmMac(IN u4 u4AlgId,
                           IN const void* pvKey,
                           IN const void* pvIv,
                           IN const void* pvInput, IN u4 u4InputLen,
                           OUT void* pvOutput, INOUT u4* pu4OutputLen);

	////////////////////////////////////////////////////////////
	// EsSymmCipherXXX�ӿ���Ҫ����PKCS11�Ľӿ���ʽ�ṩ��CSP�����ڴ˽ӿ��Ϸ�װʵ�֡�
    ////////////////////////////////////////////////////////////

	/**
     *  @brief �Գ��㷨: �����ʼ��
	 *
	 *  @param [in] u4SlotId δʹ�ã�������Ϊ0
	 *  @param [in] u4AlgId �Գ��㷨Id�����ES_SYMM_ALG_XXX����֧��RC4
	 *  @param [in] u4BlockSize �Գ��㷨���鳤��
	 *  @param [in] pu1Key �Գ���Կ
	 *  @param [in] u4KeyLen �Գ���ԿpvKey���ȣ�����>=��Ӧ�㷨����Կ����
	 *  @param [in] pu1IV ��ʼֵ����pvKey������ͬ
	 *  @param [in] u4IVLen ��ʼֵ���ȣ�����>=��Ӧ�㷨����Կ����
     *  @param [in] u4Mode �Գ��㷨ģʽ�����ES_SYMM_ALG_MODE_XXX��Ŀǰ��֧��ECB/CBC
     *  @param [in] u4Padding �Ƿ��㣬0-�����㣬����-����
     *  @param [out] phCipher ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmCipherInit(IN u4 u4SlotId, IN u4 u4AlgId, IN u4 u4BlockSize, 
								  IN const u1* pu1Key, IN u4 u4KeyLen, 
								  IN const u1* pu1IV, IN u4 u4IVLen,
								  IN u4	u4Mode, IN u4 u4Padding, 
								  OUT ESHANDLE* phCipher);

	/**
     *  @brief �Գ��㷨: �ͷ�
	 *
	 *  @param [in] hCipher �������EsSymmCipherInit����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmCipherRelease(IN ESHANDLE hCipher);

	/**
     *  @brief �Գ��㷨: ����
	 *
	 *  @param [in] hCipher �������EsSymmCipherInit����
	 *  @param [in] u4OperationType ��������,����/����,���ES_OP_TYPE_XXX
	 *  @param [out] pu1Out �������
	 *  @param [in,out] pu4OutLen ������ݻ�����/�������
	 *  @param [in] pu1In ��������
	 *  @param [in] u4InLen �������ݳ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ���ܲ������߲�����ļ��ܲ���ʱ���������ݳ��ȱ����Ƿ��鳤�ȱ���
     */
	u4 CALL_TYPE EsSymmCipherOperation(IN ESHANDLE hCipher, IN u4 u4OperationType, 
									   OUT u1* pu1Out, INOUT u4* pu4OutLen, 
									   IN const u1* pu1In, IN u4 u4InLen);

	/**
     *  @brief �Գ��㷨: ���������-update
	 *
	 *  @param [in] hCipher �������EsSymmCipherInit����
	 *  @param [in] u4OperationType ��������,����/����,���ES_OP_TYPE_XXX
	 *  @param [out] pu1Out �������
	 *  @param [in,out] pu4OutLen ������ݻ�����/�������
	 *  @param [in] pu1In ��������
	 *  @param [in] u4InLen �������ݳ��ȣ����Բ��������鳤��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsSymmCipherUpdate(IN ESHANDLE hCipher, IN u4 u4OperationType, 
									OUT u1* pu1Out, INOUT u4* pu4OutLen, 
									IN const u1* pu1In, IN u4 u4InLen);

	/**
     *  @brief �Գ��㷨: ���������-final
	 *
	 *  @param [in] hCipher �������EsSymmCipherInit����
	 *  @param [in] u4OperationType ��������,����/����,���ES_OP_TYPE_XXX
	 *  @param [out] pu1Out �������
	 *  @param [in,out] pu4OutLen ������ݻ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details 1. ����������£���������Ӧ������˼���/���ܣ�RemainLen=0
	 *			 2. �����ҽ��ܲ�����Ӧ����RemainLen=BlockLen
     */
	u4 CALL_TYPE EsSymmCipherFinal(IN ESHANDLE hCipher, IN u4 u4OperationType, 
								   OUT u1* pu1Out, INOUT u4* pu4OutLen);

	// �������ӿ���Ҫ��CSP���ã�Ϊ�����ϲ����Ϊȡ���Ⱥ��ϲ㻺�������Ȳ���ʱ������ʵ���Ե����㣬���ṩ���������ӿ�
	// EsSymCipherEncryptGetOutLen,EsSymCipherDecryptGetOutLen are for CSP convenience
	/**
     *  @brief �Գ��㷨: ��ȡ��������������
	 *
	 *  @param [in] hCipher �������EsSymmCipherInit����
	 *  @param [in,out] pu4OutLen ����������
	 *  @param [in] u4InLen �������ݳ���
	 *  @param [in] u4FinalFlag 0:��final����������:final����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details �˽ӿ���Ҫ��CSP���ã�Ϊ�����ϲ����Ϊȡ���Ⱥ��ϲ㻺�������Ȳ���ʱ[������ʵ����]������
     */
	u4 CALL_TYPE EsSymCipherEncryptGetOutLen(IN ESHANDLE hCipher, OUT u4* pu4OutLen, IN u4 u4InLen, IN u4 u4FinalFlag);

	/**
     *  @brief �Գ��㷨: ��ȡ��������������
	 *
	 *  @param [in] hCipher �������EsSymmCipherInit����
	 *  @param [in,out] pu4OutLen ����������
	 *  @param [in] u4InLen �������ݳ���
	 *  @param [in] u4FinalFlag 0:��final����������:final����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details �˽ӿ���Ҫ��CSP���ã�Ϊ�����ϲ����Ϊȡ���Ⱥ��ϲ㻺�������Ȳ���ʱ[������ʵ����]������
     */
	u4 CALL_TYPE EsSymCipherDecryptGetOutLen(IN ESHANDLE hCipher, OUT u4* pu4OutLen, IN u4 u4InLen, IN u4 u4FinalFlag);
	//---------------------------------------------------------------------------------
    
	//////////////////////////////////////////////////////////////////////////
	// �ǶԳ��㷨
	//////////////////////////////////////////////////////////////////////////

#	define ES_ASYM_ALG_RSA				(0x0001)
#	define ES_ASYM_ALG_SM2				(0x0002)
#	define ES_ASYM_ALG_ECC				(0x0003) // ����ecc��sm2���㷨������Ϊ�ڲ�ʹ�ã�API��֧��
	// ecc ��Ҫ!!! 0x01xxΪECC�㷨
	// ecc ���߲���Ϊsecp r1ϵ�У�0x010x
#	define ES_ASYM_ALG_ECC_SECP192R1	(0x0101) // nistP192,ģ��24byte
#	define ES_ASYM_ALG_ECC_SECP224R1	(0x0102) // nistP224,ģ��28byte
#	define ES_ASYM_ALG_ECC_SECP256R1	(0x0103) // ECDSA_P256,ģ��32byte 
#	define ES_ASYM_ALG_ECC_SECP384R1	(0x0104) // ECDSA_P384,ģ��48byte
#	define ES_ASYM_ALG_ECC_SECP521R1	(0x0105) // ECDSA_P521,ģ��521bit,����68byte
	// ecc ���߲���Ϊsecp k1ϵ�У�0x011x
#	define ES_ASYM_ALG_ECC_SECP192K1	(0x0111) // secP192k1,ģ��24byte
#	define ES_ASYM_ALG_ECC_SECP224K1	(0x0112) // secP224k1,ģ��28byte
#	define ES_ASYM_ALG_ECC_SECP256K1	(0x0113) // secP256k1,ģ��32byte 
	// ecc �����ɵ�����ȷ�� 0x01FF
#	define ES_ASYM_ALG_ECC_SET			(0x01FF) // ecc�㷨���û��Զ������ߺ�ģ��

#	define IS_ECC_ALG(u4AlgId)			( 0x0100 == ((u4AlgId)&0x0100) )

#	define ES_RSA_MAX_MOD_LEN			(0x200) // ��֧�ֵ�rsa4096
#	define ES_ECC_MAX_MOD_LEN			(0x100)	// ��֧�ֵ�ecc2048

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
	}BlkSmallRsaKey; // ԭ����BlkRsaKey����ʱ����������ݾ���Կ�ṹ�Ĵ���ʹ��
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
     *  @brief �ǶԳ��㷨: ����RSA��Կ��
     *
     *  @param [in] iModBitLen ��Կģ��(λ)��1024/2048/4096
     *  @param [out] pvPubKeyN ��ԿN��iModBitLen/8
     *  @param [out] pvPubKeyE ��Կָ��E��4�ֽ�
     *  @param [out] pvPriKeyD ˽ԿD��iModBitLen/8
     *  @param [out] pvPriKeyP ˽ԿP��iModBitLen/16
	 *  @param [out] pvPriKeyQ ˽ԿQ��iModBitLen/16
     *  @param [out] pvPriKeyDP ˽ԿDP��iModBitLen/16
     *  @param [out] pvPriKeyDQ ˽ԿDQ��iModBitLen/16
     *  @param [out] pvPriKeyQInv ˽ԿQInv��iModBitLen/16
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
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
     *  @brief �ǶԳ��㷨RSA: ��Կ����
     *
     *  @param [in] u2ModLen ��Կģ��(�ֽ�)��0x80/0x100/0x200
     *  @param [in] pvPubKeyN ��ԿN��iModBitLen/8
     *  @param [in] pvPubKeyE ��ԿN
     *  @param [in] u4ELen E�ĳ���
     *  @param [in] pvInput �������ݣ�����Ҫ��Ϊģ��������ʹ��PKCS1 type2���㣨ʵ�ʲ��Խ����������Ĵ�ֵ���Գ�����������ģ�����ֽ�,��ʹ��PKCS1 type2����Ҳ���Խ��м��ܣ�
     *  @param [out] pvOutput ���������Ҫ��Ϊģ��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaEnc(IN u2 u2ModLen,
                          IN const void* pvPubKeyN,
                          IN const void* pvPubKeyE, IN u4 u4ELen,
                          IN const void* pvInput,
                          OUT void* pvOutput);
    
    /**
     *  @brief �ǶԳ��㷨RSA: ˽Կǩ��(�Ȳ�OID�����Ű���PKCS1 type1�����ǩ��)
     *
     *  @param [in] pblkAsymKey ˽Կ
     *  @param [in] u4HashAlgId Hash�㷨Id�����ES_HASH_ALG_XXXX
     *  @param [in] pvHashValue Hashֵ��������hash�㷨idƥ��
     *  @param [out] pvSignature ǩ��ֵ�����������ȱ�֤��ģ��
     *  @param [out] pu4SignatureLen ǩ��ֵ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaSign(IN const BlkAsymKey* pblkAsymKey,
                           IN u4 u4HashAlgId, IN const void* pvHashValue,
                           OUT void* pvSignature, OUT u4* pu4SignatureLen);
    
    /**
     *  @brief �ǶԳ��㷨RSA: ��Կ��ǩ
     *
     *  @param [in] u2ModLen ģ��(�ֽ�)
     *  @param [in] pvPubKeyN ��ԿN������Ϊģ��
     *  @param [in] pvPubKeyE ��ԿE
     *  @param [in] u4ELen ��ԿE����
     *  @param [in] pvHashValue Hashֵ
     *  @param [in] u4HashValueLen Hashֵ����
     *  @param [in] pvSignature ǩ��ֵ
     *  @param [in] u4SignatureLen ǩ��ֵ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaVerify(IN u2 u2ModLen,
                             IN const void* pvPubKeyN,
                             IN const void* pvPubKeyE, IN u4 u4ELen,
                             IN const void* pvHashValue, IN u4 u4HashValueLen,
                             IN const void* pvSignature, IN u4 u4SignatureLen);
    
    /**
     *  @brief �ǶԳ��㷨RSA: ��Կ����
     *
     *  @param [in] u2ModLen ģ��(�ֽ�)
     *  @param [in] pvPubKeyN ��ԿN������Ϊģ��
     *  @param [in] pvPubKeyE ��ԿE
	 *  @param [in] u4ELen ��ԿE����
	 *  @param [in] pvInput �������ݣ�����Ϊģ��
	 *  @param [out] pvOutput ���ܽ��������Ϊģ��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaPubKeyDec(IN u2 u2ModLen, 
								IN const void* pvPubKeyN, 
								IN const void* pvPubKeyE, IN u4 u4ELen, 
								IN const void* pvInput, 
								OUT void* pvOutput);
    
    /**
     *  @brief �ǶԳ��㷨RSA: ˽ԿD����
     *
     *  @param [in] u2ModLen ģ��(�ֽ�)
     *  @param [in] pvPubKeyN ��ԿN������Ϊģ��
     *  @param [in] pvPriKeyD ˽ԿD
     *  @param [in] pvInput �������ݣ�����Ϊģ��
     *  @param [out] pvOutput ���ܽ��������Ϊģ��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsRsaDecD(IN u2 u2ModLen,
                           IN const void* pvPubKeyN,
                           IN const void* pvPriKeyD,
                           IN const void* pvInput,
                           OUT void* pvOutput);
    
    /**
     *  @brief �ǶԳ��㷨RSA: ˽ԿP/Q/DP/DQ/QINV����
     *
     *  @param [in] u2ModLen ģ��(�ֽ�)
     *  @param [in] pvPriKeyP ˽ԿP������Ϊģ��/2
     *  @param [in] pvPriKeyQ ˽ԿQ������Ϊģ��/2
     *  @param [in] pvPriKeyDP ˽ԿDP������Ϊģ��/2
     *  @param [in] pvPriKeyDQ ˽ԿDQ������Ϊģ��/2
     *  @param [in] pvPriKeyQinv ˽ԿQInv������Ϊģ��/2
     *  @param [in] pvInput �������ݣ�����Ϊģ��
     *  @param [out] pvOutput ���ܽ��������Ϊģ��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
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
     *  @brief �ǶԳ��㷨RSA: ����P��Q�����˽Կȫ������
     *
     *  @param [in] u4E E��Ĭ��Ϊ0x10001
     *  @param [in] pu1P ˽ԿP������Ϊģ��/2
     *  @param [in] pu1Q ˽ԿQ������Ϊģ��/2
     *  @param [in] u4ModLen ģ��
     *  @param [out] pblkAsymKey ˽Կ
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
    u4 CALL_TYPE EsRsaPQGetKey(IN u4 u4E,
                               IN const u1* pu1P, IN const u1* pu1Q,
                               IN u4 u4ModLen,
                               OUT BlkAsymKey* pblkAsymKey);

    /**
     *  @brief �ǶԳ��㷨SM2: ������Կ��
     *
     *  @param [out] pu1PriKeyD ˽ԿD��32�ֽ�
     *  @param [out] pu1PubKeyQx ��ԿX��32�ֽ�
     *  @param [out] pu1PubKeyQy ��ԿY��32�ֽ�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2GenKey(OUT u1* pu1PriKeyD, OUT u1* pu1PubKeyQx, OUT u1* pu1PubKeyQy);
    
    /**
     *  @brief �ǶԳ��㷨SM2: ˽Կǩ��
     *
     *  @param [in] pu1PriKeyD ˽ԿD��32�ֽ�
     *  @param [in] pu1HashValue hashֵ��Ĭ��ΪSM3�����32�ֽ�
     *  @param [out] pu1SignatureR ǩ��ֵR��32�ֽ�
     *  @param [out] pu1SignatureS ǩ��ֵS��32�ֽ�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2Sign(IN const u1* pu1PriKeyD,
                           IN const u1* pu1HashValue,
                           OUT u1* pu1SignatureR,
                           OUT u1* pu1SignatureS);
    
    /**
     *  @brief �ǶԳ��㷨SM2: �ⲿ��Ϣ˽Կǩ��
     *
     *  @param [in] pu1PriKeyD ˽ԿD��32�ֽ�
     *  @param [in] pvMsg ��Ϣ
     *  @param [in] u4MsgLen ��Ϣ����
     *  @param [out] pvSignature ǩ��ֵ��R|S��64�ֽ�
     *  @param [out] pu4SignatureLen ǩ��ֵ���ȣ�64
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2SignMsg(IN const u1* pu1PriKeyD,
                              IN const void* pvMsg, IN u4 u4MsgLen,
                              OUT void* pvSignature, OUT u4* pu4SignatureLen);
    
    /**
     *  @brief �ǶԳ��㷨SM2: ��Կ��ǩ
     *
     *  @param [in] pu1PubKeyQx ��ԿX��32�ֽ�
     *  @param [in] pu1PubKeyQy ��ԿY��32�ֽ�
     *  @param [in] pu1HashValue Hashֵ��32�ֽ�
     *  @param [in] pu1SignatureR ǩ��ֵR��32�ֽ�
     *  @param [in] pu1SignatureS ǩ��ֵS��32�ֽ�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2Verify(IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy,
                             IN const u1* pu1HashValue,
                             IN const u1* pu1SignatureR, IN const u1* pu1SignatureS);
    
    /**
     *  @brief �ǶԳ��㷨SM2: ����Zֵ
     *
     *  @param [in] pszID IDֵ�����Ϊ�գ���ʹ��Ĭ�ϵ�"1234567812345678"
     *  @param [in] pu1PubKeyQx ��ԿX��32�ֽ�
     *  @param [in] pu1PubKeyQy ��ԿY��32�ֽ�
     *  @param [out] pu1Za Zֵ��32�ֽ�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2GetZa(IN const char* pszID, IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy, OUT u1* pu1Za);
    
    /**
     *  @brief �ǶԳ��㷨SM2: ��Կ����
     *
     *  @param [in] pu1PubKeyQx ��ԿX��32�ֽ�
     *  @param [in] pu1PubKeyQy ��ԿY��32�ֽ�
     *  @param [in] pu1Plain ����
     *  @param [in] u4PlainLen ���ĳ���
     *  @param [out] pu1Cipher ���� �����¹淶C1|C3|C2��֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4CipherLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2Enc(IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy,
                          IN const u1* pu1Plain, IN u4 u4PlainLen,
                          OUT u1* pu1Cipher, INOUT u4* pu4CipherLen);
    
    /**
     *  @brief �ǶԳ��㷨SM2: ˽Կ����
     *
     *  @param [in] pu1PriKeyD ˽ԿD��32�ֽ�
     *  @param [in] pu1Cipher ���� �����¹淶C1|C3|C2��ɹ淶C1|C2|C3������
     *  @param [in] u4CipherLen ���ĳ���
     *  @param [out] pu1Plain ���ģ�֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4PlainLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2Dec(IN const u1* pu1PriKeyD,
                          IN const u1* pu1Cipher, IN u4 u4CipherLen,
                          OUT u1* pu1Plain, INOUT u4* pu4PlainLen);
    
    /**
     *  @brief �ǶԳ��㷨SM2: ��˽Կ�������Կ
     *
     *  @param [in] pu1PriKeyD ˽ԿD��32�ֽ�
     *  @param [out] pu1PubKeyQx ��ԿX��32�ֽ�
     *  @param [out] pu1PubKeyQy ��ԿY��32�ֽ�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2ExportPubKey(IN const u1* pu1PriKeyD, OUT u1* pu1PubKeyQx, OUT u1* pu1PubKeyQy);

    /**
     *  @brief �ǶԳ��㷨SM2: SM3��ZֵHash����
     *
     *  @param [in] bCalcZa ESFALSE-������Zֵ,�������
     *  @param [in] pblkAsymKey ��Կ��bCalcZa=ESTRUEʱ��Ч
     *  @param [in] pvMsg ��Ϣ
     *  @param [in] u4MsgLen ��Ϣ����
     *  @param [out] pu1HashValue Hashֵ��32�ֽ�
     *
     *  @return SM3 Hash�������32�ֽڣ�ʧ��ʱ����0
     *
     *  @details MSB first
     */
	u1 CALL_TYPE EsSm3Hash(IN ESBOOL bCalcZa, IN const BlkAsymKey* pblkAsymKey,
                           IN const void* pvMsg, IN u4 u4MsgLen,
                           OUT u1* pu1HashValue);
    
    /**
     *  @brief �ǶԳ��㷨: ������Կ��
     *
     *  @param [in,out] pblkAsymKey ��Կ����˽Կ�������ʱ������m_u4AlgId��m_u4ModLen
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymGenKeyPair(INOUT BlkAsymKey* pblkAsymKey);
    
    /**
     *  @brief �ǶԳ��㷨: ˽Կǩ��
     *
     *  @param [in] pblkAsymKey ��Կ����˽Կ��
     *  @param [in] pvSignData ǩ�����ݣ�����Ϊģ����RSAĬ��Ϊ���������ݣ�SM2Ĭ��ΪSM3��Hashֵ,ECC���EsEccSign
     *  @param [out] pvSignature ǩ��ֵ,֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4SignatureLen ǩ��ֵ������/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymSign(IN const BlkAsymKey* pblkAsymKey,
                            IN const void* pvSignData,
                            OUT void* pvSignature, INOUT u4* pu4SignatureLen);
    
    /**
     *  @brief �ǶԳ��㷨: ��Կ����
     *
     *  @param [in] pblkAsymKey ��Կ
     *  @param [in] pvPlainText �������ݣ�RSA����Ҫ��Ϊģ��������ʹ��PKCS1 type2���㡣SM2û������Ҫ��
     *  @param [in] u4PlainTextLen �������ݳ���
     *  @param [out] pvCipherText ����,֧�ִ���NULL�󳤶ȣ�SM2����C1|C3|C2����
     *  @param [in,out] pu4CipherTextLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymEnc(IN const BlkAsymKey* pblkAsymKey,
                           IN const void* pvPlainText, IN u4 u4PlainTextLen,
                           OUT void* pvCipherText, INOUT u4* pu4CipherTextLen);
    
    /**
     *  @brief �ǶԳ��㷨: ˽Կ����
     *
     *  @param [in] pblkAsymKey ��Կ
     *  @param [in] pvCipherText ����,SM2֧��C1|C2|C3��C1|C3|C2
     *  @param [in] u4CipherTextLen ǩ��ֵ������/�������
     *  @param [out] pvPlainText ����,֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4PlainTextLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymDec(IN const BlkAsymKey* pblkAsymKey,
                           IN const void* pvCipherText, IN u4 u4CipherTextLen,
                           OUT void* pvPlainText, INOUT u4* pu4PlainTextLen);
    
    /**
     *  @brief �ǶԳ��㷨: ��ǩ
     *
     *  @param [in] pblkAsymKey ��Կ
     *  @param [in] u4HashAlgId hash�㷨�����ES_HASH_ALG_XXX����RSA��Ч
     *  @param [in] pvHashValue Hashֵ��������Hash�㷨��Ӧ
     *  @param [in] pvSignature ǩ��ֵ
     *  @param [in] u4SignatureLen ǩ��ֵ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymVerifySignature(IN const BlkAsymKey* pblkAsymKey,
                                       IN u4 u4HashAlgId,
                                       IN const void* pvHashValue,
                                       IN const void* pvSignature, IN u4 u4SignatureLen);
    
    /**
     *  @brief �ǶԳ��㷨: ��Ϣ��ǩ
     *
     *  @param [in] pblkAsymKey ��Կ
     *  @param [in] bCalcZa ESFALSE-������Zֵ��������㣬��SM2��Ч
     *  @param [in] u4HashAlgId hash�㷨�����ES_HASH_ALG_XXX
     *  @param [in] pvMsg ��Ϣ
     *  @param [in] u4MsgLen ��Ϣ����
     *  @param [in] pvSignature ǩ��ֵ
     *  @param [in] u4SignatureLen ǩ��ֵ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymVerifySignatureMsg(IN const BlkAsymKey* pblkAsymKey,
                                          IN ESBOOL bCalcZa,
                                          IN u4 u4HashAlgId,
                                          IN const void* pvMsg, IN u4 u4MsgLen,
                                          IN const void* pvSignature, IN u4 u4SignatureLen);
    
    /**
     *  @brief �ǶԳ��㷨: ��Ϣǩ��
     *
     *  @param [in] pblkAsymKey ��Կ
     *  @param [in] u4HashAlgId hash�㷨�����ES_HASH_ALG_XXX
     *  @param [in] pvMsg ��Ϣ
     *  @param [in] u4MsgLen ��Ϣ����
     *  @param [out] pvSignature ǩ��ֵ
     *  @param [in,out] pu4SignatureLen ǩ��ֵ������/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsAsymSignMsg(IN const BlkAsymKey* pblkAsymKey,
                               IN u4 u4HashAlgId,
                               IN const void* pvMsg, IN u4 u4MsgLen,
                               OUT void* pvSignature, INOUT u4* pu4SignatureLen);

	typedef struct BlkSm2CtxTag
	{
		u4 m_u4State;					// 0x00 δ�����C1, 0x01 �Ѿ������C1
		u1 m_pu1RFU[0x40+0x04+0x68];    // �����ڲ�ʹ�õĽṹ�������߲��ù���
	}BlkSm2Ctx;
    
    /**
     *  @brief �ǶԳ��㷨SM2: ��Կ����-֧�ַ���
     *
     *  @param [in] pu1PubKeyQx ��ԿX��32�ֽ�
     *  @param [in] pu1PubKeyQy ��ԿY��32�ֽ�
     *  @param [in] u4FinalFlag 0:�м�����,���ĳ��ȱ�����0x40������;����:final���㣬���ĳ�������
     *  @param [in] pblkSm2Ctx ������������Ϣ
     *  @param [in] pu1Plain ����
     *  @param [in] u4PlainLen ���ĳ���
     *  @param [out] pu1Cipher ���� ��C1C2C3����
     *                         ��1��update������C1|��һ�����ģ�
     *                         �������������update�������ض�Ӧ�����ģ�
     *                         final���������ģ�������C3
     *  @param [in,out] pu4CipherLen ���Ļ�����/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details 1. MSB first
     *           2. ���Ľ��: C1C2C3
     */
	u4 CALL_TYPE EsSm2EncEx(IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy,
                            IN u4 u4FinalFlag,
                            BlkSm2Ctx* pblkSm2Ctx,
                            IN const u1* pu1Plain, IN u4 u4PlainLen,
                            OUT u1* pu1Cipher, INOUT u4* pu4CipherLen);

	#define ES_SM2_CIPHER_MODE_C1C2C3	(0x01)
	#define ES_SM2_CIPHER_MODE_C1C3C2	(0x02)
    /**
     *  @brief �ǶԳ��㷨SM2: ��Կ����-��ָ�������ʽ��C1C2C3/C1C3C2
     *
     *  @param [in] pu1PubKeyQx ��ԿX��32�ֽ�
     *  @param [in] pu1PubKeyQy ��ԿY��32�ֽ�
     *  @param [in] pu1Plain ����
     *  @param [in] u4PlainLen ���ĳ���
     *  @param [out] pu1Cipher ���� �����¹淶C1|C3|C2��֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4CipherLen ���Ļ�����/�������
     *  @param [in] u4CipherMode ����ģʽ�����ES_SM2_CIPHER_MODE_XXX
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2EncWithMode(IN const u1* pu1PubKeyQx, IN const u1* pu1PubKeyQy,
                                  IN const u1* pu1Plain, IN u4 u4PlainLen,
                                  OUT u1* pu1Cipher, INOUT u4* pu4CipherLen,
                                  IN u4 u4CipherMode);
    
    /**
     *  @brief �ǶԳ��㷨SM2: ˽Կ����-��ָ�������ʽ��C1C2C3/C1C3C2
     *
     *  @param [in] pu1PriKeyD ˽ԿD��32�ֽ�
     *  @param [in] pu1Cipher ���� �����¹淶C1|C3|C2
     *  @param [in] u4CipherLen ���ĳ���
     *  @param [out] pu1Plain ���ģ�֧�ִ���NULL�󳤶�
     *  @param [in,out] pu4PlainLen ���Ļ�����/�������
     *  @param [in] u4CipherMode ����ģʽ�����ES_SM2_CIPHER_MODE_XXX
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details MSB first
     */
	u4 CALL_TYPE EsSm2DecWithMode(IN const u1* pu1PriKeyD,
                                  IN const u1* pu1Cipher, IN u4 u4CipherLen,
                                  OUT u1* pu1Plain, INOUT u4* pu4PlainLen,
                                  IN u4 u4CipherMode);
	
	//////////////////////////////////////////////////////////////////////////
	// ��ԿЭ��
	//////////////////////////////////////////////////////////////////////////
	/**
	 *  @brief ��Ϊ��ԿЭ�̵ķ������ȵ��ô˺�����ȡ����ʱ��Կ�Ժ�Э�̾����agt=agreement��
	 *
	 *  @param[in] pszSponsorId �����ߵ�ID��NULL��ʾʹ��ȱʡ��IDֵ��1234567812345678����
	 *  @param[in] pblkAsymKeySponsor �����ߵĹ�˽Կ�ԣ�һ��Ҫ��˽Կ�� 
	 *  @param[out] pblkAsymKeySponsorTemp �����ߵ���ʱ��Կ��
	 *  @param[out] phAgt �����ߵ�Э�̾�������������ô˾������Э����Կ��
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsSm2AgtSponsorInit(IN const char* pszSponsorId, 
									 IN const BlkAsymKey* pblkAsymKeySponsor, 
									 OUT BlkAsymKey* pblkAsymKeySponsorTemp, 
									 OUT ESHANDLE* phAgt);
	/**
	 *  @brief ��Ϊ��ԿЭ�̵���Ӧ�����ô˺�����ȡ���Ự��Կ��agt=agreement��
	 *
	 *  @param[in] pszSponsorId �����ߵ�ID��NULL��ʾʹ��ȱʡ��IDֵ��1234567812345678����
	 *  @param[in] pblkAsymKeySponsor �����ߵĹ�Կ�� 
	 *  @param[in] pblkAsymKeySponsorTemp �����ߵ���ʱ��Կ��
	 *  @param[in] pszResponderId ��Ӧ����ID��NULL��ʾʹ��ȱʡ��IDֵ��1234567812345678����
	 *  @param[in] pblkAsymKeyResponder ��Ӧ���Ĺ�˽Կ�ԣ�һ��Ҫ��˽Կ�� 
	 *  @param[out] pblkAsymKeyResponderTemp ��Ӧ������ʱ��˽Կ�ԡ�
	 *  @param[out] pu1SKey Э�̵��ĻỰ��Կ����֤0x20�Ļ���������
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
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
	 *  @brief ��ԿЭ�̷������ɻỰ��Կ��agt=agreement��
	 *
	 *  @param[in] hAgt ����Э�̾����
	 *  @param[in] pszResponderId ��Ӧ����ID��NULL��ʾʹ��ȱʡ��IDֵ��1234567812345678����
	 *  @param[in] pblkAsymKeyResponder ��Ӧ���Ĺ�Կ�� 
	 *  @param[in] pblkAsymKeyResponderTemp ��Ӧ������ʱ��Կ��
	 *  @param[out] pu1SKey Э�̵��ĻỰ��Կ����֤0x20�Ļ���������
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsSm2AgtSponsorGenSKey(IN ESHANDLE hAgt, IN const char* pszResponderId, 
										IN const BlkAsymKey* pblkAsymKeyResponder, 
										IN const BlkAsymKey* pblkAsymKeyResponderTemp, 
										OUT u1* pu1SKey);
	
    /**
	 *  @brief �Ự��ԿЭ��-�ͷŷ��𷽽�������ԿЭ�̾����agt=agreement��
	 *
	 *  @param[in,out] phAgt ����Э�̾��
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details none
	 */
	u4 CALL_TYPE EsSm2AgtSponsorFinal(INOUT ESHANDLE* phAgt);

	//////////////////////////////////////////////////////////////////////////
	// ����
	//////////////////////////////////////////////////////////////////////////
	#	define	PADDING_TYPE_ZERO		0x00	// ��ԭʼ����ǰ����00�������飬�����鲻����
	#	define	PADDING_TYPE_PKCS_1		0x01	// padding fmt: 00 01 FF...FF 00 Data pkcs padding type 1
	#	define	PADDING_TYPE_PKCS_2		0x02	// padding fmt: 00 02 PS 00 Data pkcs padding type 2
	#	define	PADDING_TYPE_PKCS_5		0x05	// padding fmt: XX[XX]
	#	define	PADDING_TYPE_PKCS_9		0x09	// padding fmt: 00 09 FF...FF 00 Data
	#	define	PADDING_TYPE_7816		0x80	// padding fmt: Data 80 00..00�����Data�������飬�Ͳ�����

	/**
	 *  @brief ���ݲ���
	 *  
	 *  @param [in] pvOrgData ԭʼ����
	 *  @param [in] u4OrgDataLen ԭʼ���ݳ��ȣ�>0
	 *  @param [in] u4PaddingType �������ͣ����PADDING_TYPE_XXX
	 *  @param [in] u4ModOrBlockLen ģ��/���鳤��
	 *  @param [out] pvOutData ���������ݣ���������Ҫ��֤��������С�㹻�����details
	 *  @param [out] pu4OutDataLen ���������ݳ��ȣ�����ΪNULL
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details ���鷽ʽ�ͳ���Ҫ��˵��
	 *           PADDING_TYPE_ZERO  :��ԭʼ����ǰ����00�������飬�����鲻���㣬���볤��<=���鳤�ȣ��������Ϊ���鳤��
	 *           PADDING_TYPE_PKCS_1:��ԭʼ����ǰ����00 01 FF...FF 00�������飬(���볤��+3)<=���鳤�ȣ��������Ϊ���鳤��
	 *           PADDING_TYPE_PKCS_2:��ԭʼ����ǰ����00 01 ��0����� 00�������飬(���볤��+3)<=���鳤�ȣ��������Ϊ���鳤��
	 *           PADDING_TYPE_PKCS_5:��ԭʼ���ݺ���N��N=ģ��-���볤��%ģ��)�������飬ԭʼ������������Ҳ��Ҫ���㣬�������Ϊ���鳤��(��+1���鳤��)
	 *           PADDING_TYPE_PKCS_9:��ԭʼ����ǰ����00 01 FF...FF 00�������飬(���볤��+3)<=���鳤�ȣ��������Ϊ���鳤��
	 *           PADDING_TYPE_7816  :��ԭʼ���ݺ���80 00...�������飬�����鲻���㣬�������Ϊ���鳤��
	 */
	u4 CALL_TYPE EsPadding(IN const void* pvOrgData, IN u4 u4OrgDataLen,
                           IN u4 u4PaddingType, IN u4 u4ModOrBlockLen,
                           OUT void* pvOutData, OUT u4* pu4OutDataLen);

	/**
	 *  @brief ����ȥ����
	 *  
	 *  @param [in] pvOrgData ��������
	 *  @param [in] u4OrgDataLen �������ݳ��ȣ�
	 *  @param [in] u4PaddingType �������ͣ����PADDING_TYPE_XXX
	 *  @param [in] u4ModOrBlockLen ģ��/���鳤��
	 *  @param [out] pvOutData ȥ���������ݣ���������Ҫ��֤��������С�㹻�������������������һ��
	 *  @param [out] pu4OutDataLen ���������ݳ��ȣ�����ΪNULL
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details ����ģʽ�Լ�������Ҫ�����EsPadding
	 */
	u4 CALL_TYPE EsPaddingRemove(IN const void* pvOrgData, IN u4 u4OrgDataLen,
                                 IN u4 u4PaddingType, IN u4 u4ModOrBlockLen,
                                 OUT void* pvOutData, OUT u4* pu4OutDataLen);

	//////////////////////////////////////////////////////////////////////////
	// HMAC
	//////////////////////////////////////////////////////////////////////////

	/**
	 *  @brief ��ʼ��Hmac���
	 *  
	 *  @param [in] u4HashAlgId Hash�㷨Id
	 *  @param [in] pu1IV Hash��ʼֵ
	 *  @param [in] pu1Key ��Կ
	 *  @param [in] u4KeyLen ��Կ����
	 *  @param [out] phEsHmac ����Hmac���
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacInit(IN u4 u4HashAlgId, IN const u1*pu1IV, IN const u1* pu1Key, IN u4 u4KeyLen, OUT ESHANDLE* phEsHmac);

	/**
	 *  @brief Hmac����
	 *  
	 *  @param [in] hEsHmac Hmac���
	 *  @param [in] pu1In ԭ��
	 *  @param [in] u4InLen ԭ�ĳ���
	 *  @param [out] pu1Out hmacֵ��֧�ִ���NULL�󳤶�
	 *  @param [out] pu4OutLen hmacֵ������/�������
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacOperation(IN ESHANDLE hEsHmac, IN const u1* pu1In, IN u4 u4InLen, OUT u1* pu1Out, u4* pu4OutLen);

	/**
	 *  @brief Hmac�м�����
	 *  
	 *  @param [in] hEsHmac Hmac���
	 *  @param [in] pu1In ԭ��
	 *  @param [in] u4InLen ԭ�ĳ���
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacUpdate(IN ESHANDLE hEsHmac, IN const u1* pu1In, IN u4 u4InLen);

	/**
	 *  @brief Hmac��β����
	 *  
	 *  @param [in] hEsHmac Hmac���
	 *  @param [out] pu1Out hmacֵ��֧�ִ���NULL�󳤶�
	 *  @param [in,out] pu4OutLen hmacֵ������/�������
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacFinal(IN ESHANDLE hEsHmac, OUT u1* pu1Out, INOUT u4* pu4OutLen);

	/**
	 *  @brief �ͷ�Hmac���
	 *  
	 *  @param [in] hEsHmac Hmac���
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmacRelease(IN ESHANDLE hEsHmac);

	/**
	 *  @brief Hmac����
	 *  
	 *  @param [in] u4HashAlgId Hash�㷨Id
	 *  @param [in] pu1Key ��Կ
	 *  @param [in] u4KeyLen ��Կ����
	 *  @param [in] pu1Msg ��Ϣ
	 *  @param [in] u4MsgLen ��Ϣ����
	 *  @param [out] pu1Hmac ���������֧�ִ���NULL�󳤶�
	 *  @param [in,out] pu4HmacLen �������������/�������
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsHmac(IN u4 u4HashAlgId, IN const u1* pu1Key, IN u4 u4KeyLen, IN const u1* pu1Msg, IN u4 u4MsgLen, OUT u1* pu1Hmac, INOUT u4* pu4HmacLen);

	//////////////////////////////////////////////////////////////////////////
	// ECC�㷨
	//////////////////////////////////////////////////////////////////////////

#	define ES_ECC_PRIME_FIELD				(0x01)	// Fp
#	define ES_ECC_CHARACTERISTIC_TWO_FIELD	(0x02)	// F2m

	typedef struct BlkEccCurveTag {
		u4		m_u4Version;						// version of the struct, should be 0x00
		u2		m_u2ModBitLen;						// real mod len of the curve in bit, u2ModBitLen is not always 8*u2ModLen, such as secp521r1
		u2		m_u2ModLen;							// mod len of the curve in byte, 
													// ���㷽ʽ: m_u2ModLen = (0==m_u2ModBitLen%8) ? m_u2ModBitLen/8 : (m_u2ModBitLen+8)/8
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
	 *  @brief ECC: ��ȡ�㷨ģ��(�ֽ�)
	 *
     *  @param[in] u4AlgId ecc�㷨�����ES_ASYM_ALG_SM2 or ES_ASYM_ALG_ECC_XXXX
	 *  
	 *  @return �㷨ģ��(�ֽ�)����֧�ֵ��㷨����0
	 *  
	 *  @details ��֧��ES_ASYM_ALG_ECC_SET
	 */
	u4 CALL_TYPE EsEccGetModLen(IN u4 u4AlgId);
	
	/**
	 *  @brief ECC: ��ȡ�㷨ģ��(����)
	 *
     *  @param[in] u4AlgId ecc�㷨�����ES_ASYM_ALG_SM2 or ES_ASYM_ALG_ECC_XXXX
	 *  
	 *  @return �㷨ģ��(����)����֧�ֵ��㷨����0
	 *  
	 *  @details ��֧��ES_ASYM_ALG_ECC_SET
	 */
	u4 CALL_TYPE EsEccGetModBitLen(IN u4 u4AlgId);

	/**
	 *  @brief ECC: ��ʼ��
	 *
     *  @param[in] u4AlgId ecc�㷨�����ES_ASYM_ALG_SM2 or ES_ASYM_ALG_ECC_XXXX
     *  @param[out] phEcc ecc���
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details ��֧��ES_ASYM_ALG_ECC_SET
	 */
	u4 CALL_TYPE EsEccInit(IN u4 u4AlgId, OUT ESHANDLE* phEcc);

	/**
	 *  @brief ECC: ��ʼ�����Զ������߲�������
	 *
	 *  @param[in] pblkCurve ����
	 *  @param[out] phEcc ecc���
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details ��ʼ�����̻��Զ�ƥ���㷨Id��δ֪���߶�Ӧ���㷨ΪES_ASYM_ALG_ECC_SET
	 */
	u4 CALL_TYPE EsEccInitWithCurve(IN const BlkEccCurve* pblkCurve, OUT ESHANDLE* phEcc);

	/**
	 *  @brief ECC:��ȡ���߲���
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[out] pu4AlgId �㷨id������ΪNULL����ʾ����Ҫ��ȡ�˲���
     *  @param[out] pblkCurve ���߲���������ΪNULL����ʾ����Ҫ��ȡ�˲���
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccGetCurveParam(IN ESHANDLE hEcc, OUT u4* pu4AlgId, OUT BlkEccCurve* pblkCurve);

	/**
	 *  @brief ECC: ����ʼ��
	 *
	 *  @param[in,out] phEcc ���
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccRelease(ESHANDLE* phEcc);

	/**
	 *  @brief ECC:����Ƿ��������ϵĵ�
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[in] pu1Point ������X||Y������Ϊ2��ģ������ͷ��
	 *  @param[in] u4PointLen ���������ݳ��ȣ�����Ϊ2��ģ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccPointCheck(IN ESHANDLE hEcc, IN const u1* pu1Point, IN u4 u4PointLen);

	/**
	 *  @brief ECC:�������
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[in] pu1K ���ϵ��K������Ϊģ������ͷ��
	 *  @param[in] pu1PointP �����˵ĵ�����Px|Py������Ϊ����ģ������ͷ��
	 *  @param[out] pu1PointQ ��˽��Q������Qx��Qy������Ϊ����ģ������ͷ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccPointMul(IN ESHANDLE hEcc, IN const u1* pu1K, IN const u1* pu1PointP, OUT u1* pu1PointQ);

	/**
	 *  @brief ECC:�������
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[in] pu1PointP ��������P x|y������Ϊ����ģ������ͷ��
	 *  @param[in] pu1PointQ ����������Q x|y������Ϊ����ģ������ͷ��
	 *  @param[out] pu1PointR �����������R x|y������Ϊ����ģ������ͷ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccPointAdd(IN ESHANDLE hEcc, IN const u1* pu1PointP, IN const u1* pu1PointQ, OUT u1* pu1PointR);

	/**
	 *  @brief ECC:��˽Կ��ȡ��Կ
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[in] pu1PriKeyD ˽ԿD������Ϊģ������ͷ��
	 *  @param[out] pu1PubKey ��Կ x|y������Ϊ����ģ������ͷ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccGetPubKey(IN ESHANDLE hEcc, IN const u1* pu1PriKeyD, OUT u1* pu1PubKey);

	/**
	 *  @brief ECC:������Կ��
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[out] pu1PriKeyD ˽ԿD������Ϊģ������ͷ��
	 *  @param[out] pu1PubKey ��Կ x|y������Ϊ����ģ������ͷ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccGenKey(IN ESHANDLE hEcc, OUT u1* pu1PriKeyD, OUT u1* pu1PubKey);

	/**
	 *  @brief ECC:����ǩ��
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[in] pu1PriKeyD ˽Կ��ģ������ͷ��
	 *  @param[in] pu1SignData hashֵ���߾�������/�ضϺ�ǩ�����ݣ������SM2������SM2�淶��������Ϊģ��
	 *  @param[out] pu1Signature ǩ��ֵ R|S������Ϊ2��ģ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details ����: ����С��ģ�����ڸ�λ��0��ģ�����ض�: ���ȴ���ģ����ȡ��λģ���ֽ�
	 */
	u4 CALL_TYPE EsEccSign(IN ESHANDLE hEcc, IN const u1* pu1PriKeyD, IN const u1* pu1SignData, OUT u1* pu1Signature);

	/**
	 *  @brief ECC:��Ϣǩ��
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[in] pu1PriKeyD ˽Կ��ģ������ͷ��
	 *  @param[in] u4HashAlgId hash�㷨
	 *  @param[in] pu1Msg ��Ϣԭ��
	 *  @param[in] u4MsgLen ��Ϣԭ�ĳ���
	 *  @param[out] pu1Signature ǩ��ֵ R|S������Ϊ2��ģ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details ����: ����С��ģ�����ڸ�λ��0��ģ�����ض�: ���ȴ���ģ����ȡ��λģ���ֽ�
	 */
	u4 CALL_TYPE EsEccSignMsg(IN ESHANDLE hEcc, IN const u1* pu1PriKeyD, IN u4 u4HashAlgId, IN const u1* pu1Msg, IN u4 u4MsgLen, OUT u1* pu1Signature);

	/**
	 *  @brief ECC:��ǩ
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[in] pu1PubKey ��Կ x|y,����Ϊ2��ģ��
	 *  @param[in] pu1SignData hashֵ���߾�������/�ضϺ�ǩ�����ݣ������SM2������SM2�淶��������Ϊģ��
	 *  @param[in] pu1Signature ǩ��ֵ R|S,����Ϊ2��ģ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccVerify(IN ESHANDLE hEcc, IN const u1* pu1PubKey, IN const u1* pu1SignData, IN const u1* pu1Signature);

	/**
	 *  @brief ECC:��ǩ
	 *
	 *  @param[in] hEcc ecc���
	 *  @param[in] pu1PubKey ��Կ x|y,����Ϊ2��ģ��
	 *  @param[in] u4HashAlgId hash�㷨
	 *  @param[in] pu1Msg ��Ϣԭ��
	 *  @param[in] u4MsgLen ��Ϣԭ�ĳ���
	 *  @param[in] pu1Signature ǩ��ֵ R|S,����Ϊ2��ģ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsEccVerifySignatureMsg(IN ESHANDLE hEcc, IN const u1* pu1PubKey, IN u4 u4HashAlgId, IN const u1* pu1Msg, IN u4 u4MsgLen, IN const u1* pu1Signature);

	//////////////////////////////////////////////////////////////////////////
	// sm9
	//////////////////////////////////////////////////////////////////////////

	// ����SM9�ӿ�ǰ����Ҫ���ô˺�����SM9��SM2�����ر�Ǯ�������߲�ͬ��
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
