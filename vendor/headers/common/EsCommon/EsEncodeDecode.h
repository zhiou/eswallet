#ifndef _ES_ENCODE_DECODE_H_
#define _ES_ENCODE_DECODE_H_

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 *  @brief Base64����
	 *  
	 *  @param[in] pu1Src ����������
	 *  @param[in] u4SrcLen ���������ݳ���>0
	 *  @param[out] pszBase64code �洢������
	 *  @param[in] u4Base64BufLen pszBase64code���������ȣ�����>=u4SrcLen*4/3
	 *  
     *  @return �����ĳ��ȣ�ʧ��ʱ����0
     *
     *  @details none
	 */
	u4 CALL_TYPE EsBase64Encode(IN const u1* pu1Src, IN u4 u4SrcLen, OUT char* pszBase64code, IN u4 u4Base64BufLen);

	/**
	 *  @brief Base64����
	 *  
	 *  @param[in] pszBase64code Base64�����ʽ������
	 *  @param[in] u4Base64CodeLen Base64�����ʽ�����ݳ���
	 *  @param[out] pu1Buf �洢����������������С���뱣֤>=u4Base64CodeLen*3/4
	 *  
     *  @return �����ĳ��ȣ�ʧ��ʱ����0
     *
     *  @details none
	 */
	u4 CALL_TYPE EsBase64Decode(IN const char* pszBase64code, IN u4 u4Base64CodeLen, OUT u1* pu1Buf);

	/**
	 *  @brief Base58����
	 *  
	 *  @param[in] pu1Data ����������
	 *  @param[in] u4DataLen ���������ݳ���,>0
	 *  @param[out] pszEncode �洢������
	 *  @param[in,out] pu4EncodedLen ���ΪpszBase64code���������ȣ�����Ϊ����������
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details TODO!!��ʱ��֧��pszEncode����NULL����볤��
	 */
	u4 CALL_TYPE EsBase58Encode(IN const u1* pu1Data, IN u4 u4DataLen, OUT char* pszEncode, INOUT u4* pu4EncodedLen);

	/**
	 *  @brief ��ָ�����ݽ���DER����
	 *  
	 *  @param[in] u4Tag Tag����T(1�ֽ�)
	 *  @param[in] pvValue Value����V�����u4Tag=0x06(OID)����Ϊ�ַ���������u4ValueLen
	 *  @param[in] u4ValueLen Len����L���������󳤶ȿ���Ϊ6�ֽ�(85 aa bb cc dd ee)
	 *  @param[out] pu1Der �����������������ΪNULL�����������ȱ�֤>=7+u4ValueLen
	 *  
	 *  @return ���������ݳ���
     *
     *  @details TODO!!��ʱ��֧��pu1Der����NULL����볤��
	 */
	u4 CALL_TYPE EsDerEncode(IN u4 u4Tag, IN const void* pvValue, IN u4 u4ValueLen, OUT u1* pu1Der);

	/**
	 *  @brief ��ȡDER����Ԫ�صĴ�С����TLV
	 *  
	 *  @param[in] pu1Der DER��������ݣ�=NULLʱ����0
	 *  @param[out] pu4LenLen ���Ԫ�ش�С��=NULLʱ����0
	 *  
	 *  @return Ԫ�ش�С
     *
     *  @details TODO�������Ĭ��TAGֻ��һ���ֽ�
	 */
	u4 CALL_TYPE EsDerGetElementLen(IN const u1* pu1Der, OUT u4* pu4LenLen);

	/**
	 *  @brief ��ȡDER�������ݵ�T��L��ֵ�Լ�T��L�ı��볤��
	 *  
	 *  @param[in] pu1DerData DER��������ݣ�������ΪNULL
	 *  @param[in] u4DerDataLen DER��������ݳ��ȣ�����>=��Ч��DER�������ݳ���
	 *  @param[out] pu2Tag T��ֵ������ΪNULL
	 *  @param[out] pu4ValueLen L��ֵ������ΪNULL
	 *  @param[out] pu4TagLenSize T��L�����ĳ��ȣ�����ΪNULL
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsDerGetTagAndLen(IN const u1* pu1DerData, IN u4 u4DerDataLen, OUT u2* pu2Tag, OUT u4* pu4ValueLen, OUT u4* pu4TagLenSize);
	
	/**
	 *  @brief ����ָ����TLV��λָ�뵽��һ��TLV
	 *  
	 *  @param[in,out] ppu1Data DER���������ָ��ĵ�ַ��������ΪNULL
	 *  @param[in,out] pu4DataLen ���Ϊ*ppu1Data���ݳ��ȣ�����Ϊ��λ��ʣ�µ����ݳ���
	 *  @param[in] u2ExpectTag ָ����T
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsDerSkipTlv(INOUT u1** ppu1Data, INOUT u4* pu4DataLen, u2 u2ExpectTag);
    
    /**
     *  @brief ��ԿDER����
     *
     *  @param[in] pblkAsymPubKey ��Կ
     *  @param[out] pu1PubKeyEncoded �洢�����Ĺ�Կ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4PubKeyEncodedLen ���Ϊpu1PubKeyEncoded��������С������Ϊ�����Կ����
     *
     *  @details none
     */
	void CALL_TYPE EsDerEncodePubKey(IN const BlkAsymKey *pblkAsymPubKey, OUT u1* pu1PubKeyEncoded, INOUT u4 * pu4PubKeyEncodedLen);
    
    /**
     *  @brief ��ԿDER����
     *
     *  @param[in] pu1PubKeyEncoded DER���빫Կ
     *  @param[in] u4PubKeyEncodedLen DER���빫Կ����
     *  @param[out] pblkAsymKey �����Ĺ�Կ
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ��֧��rsa��sm2!!!���һ��ģ��Ϊ32��ecc�㷨����sm2!!!
     */
	u4 CALL_TYPE EsDerDecodePubKey(IN const u1* pu1PubKeyEncoded, IN u4 u4PubKeyEncodedLen, OUT BlkAsymKey * pblkAsymKey);
    
    /**
     *  @brief ��ԿDER����
     *
     *  @param[in] pu1PubKeyEncoded DER���빫Կ
	 *  @param[in] u4PubKeyEncodedLen DER���빫Կ����
	 *  @param[in] u4AsymAlgId ��֪�ķǶԳ��㷨Id
     *  @param[out] pblkAsymKey �����Ĺ�Կ
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     */
	u4 CALL_TYPE EsDerDecodePubKeyEx(IN const u1* pu1PubKeyEncoded, IN u4 u4PubKeyEncodedLen, u4 u4AsymAlgId, OUT BlkAsymKey * pblkAsymKey);

    /**
     *  @brief ˽ԿDER����
     *
     *  @param[in] pblkAsymKey ˽Կ
     *  @param[out] pu1DerPriKey �洢DER������˽Կ��֧�ִ���NULL�󳤶�
     *  @param[out] pu4DerPriKeyLen ���Ϊpu1DerPriKey��������С������Ϊ�����˽Կ����
     *
     *  @details none
     */
	void CALL_TYPE EsDerEncodePriKey(IN const BlkAsymKey* pblkAsymKey, OUT u1* pu1DerPriKey, INOUT u4* pu4DerPriKeyLen);
    
    /**
     *  @brief ˽ԿDER����
     *
     *  @param[in] pu1DerPriKey DER����˽Կ
     *  @param[in] u4DerPriKeyLen DER����˽Կ����
     *  @param[out] pblkAsymKey ������˽Կ
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ��֧��rsa��sm2!!!���һ��ģ��Ϊ32��ecc�㷨����sm2!!!
     */
	u4 CALL_TYPE EsDerDecodePriKey(IN const u1* pu1DerPriKey, IN u4 u4DerPriKeyLen, OUT BlkAsymKey* pblkAsymKey);
    
	/**
     *  @brief SM2ǩ��ֵDER����
     *
     *  @param[in] pu1Sm2Signature SM2ǩ��ֵ R||S����ͷ��
     *  @param[in] u4SignatureLen SM2ǩ��ֵ���ȣ�64�ֽ�
     *  @param[out] pu1DerSignature �洢������ǩ��ֵ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4DerSignatureLen ���Ϊpu1DerSignature��������С������Ϊ����������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsDerEncodeSm2Signature(IN const u1* pu1Sm2Signature, IN u4 u4SignatureLen, OUT u1* pu1DerSignature, INOUT u4* pu4DerSignatureLen);
    
    /**
     *  @brief SM2ǩ��ֵDER����
     *
     *  @param[in] pu1DerSignature ������ǩ��ֵ
     *  @param[in] u4DerSignatureLen ǩ��ֵ����
     *  @param[out] pu1Signature SM2ǩ��ֵ R||S����ͷ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4SignatureLen ���Ϊpu1Signature��������С������Ϊ�����ĳ���64�ֽ�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsDerDecodeSm2Signature(IN const u1* pu1DerSignature, IN u4 u4DerSignatureLen, OUT u1* pu1Signature, INOUT u4* pu4SignatureLen);

    /**
     *  @brief ECCǩ��ֵDER���루Ҳ֧��SM2��
     *
     *  @param[in] pu1Signature ECCǩ��ֵ R||S����ͷ��
     *  @param[in] u4SignatureLen ECCǩ��ֵ���ȣ�ģ��2��
     *  @param[out] pu1DerSignature �洢������ǩ��ֵ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4DerSignatureLen ���Ϊpu1DerSignature��������С������Ϊ����������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsDerEncodeEccSignature(IN const u1* pu1Signature, IN u4 u4SignatureLen, OUT u1* pu1DerSignature, INOUT u4* pu4DerSignatureLen);
    
    /**
     *  @brief ECCǩ��ֵDER����
     *
     *  @param[in] pu1DerSignature ������ǩ��ֵ
	 *  @param[in] u4DerSignatureLen ������ǩ��ֵ����
     *  @param[out] pu1Signature ECCǩ��ֵ R||S����ͷ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4SignatureLen ���Ϊpu1Signature��������С������ΪR|S�ĳ��ȣ�R��S����һ��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ע�⣡ǩ��ֵ���Ȳ���֤��ģ����2������ʹ��ǩ��ֵǰ����������Ҫ���и���ģ����R��Sǰ��ֱ�00����ģ��
     */
	u4 CALL_TYPE EsDerDecodeEccSignature(IN const u1* pu1DerSignature, IN u4 u4DerSignatureLen, OUT u1* pu1Signature, INOUT u4* pu4SignatureLen);

    /**
     *  @brief ECCǩ��ֵDER����
     *
     *  @param[in] pu1DerSignature ������ǩ��ֵ
	 *  @param[in] u4DerSignatureLen ������ǩ��ֵ����
	 *  @param[in] u4ModLen ǩ��ֵ����
     *  @param[out] pu1Signature ECCǩ��ֵ R||S����ͷ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4SignatureLen ���Ϊpu1Signature��������С������Ϊ������2*u4ModLen
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details �����߱�֤u4ModLen�ĺϷ��ԣ��ӿ��ڲ�Ҳ����
     */
	u4 CALL_TYPE EsDerDecodeEccSignatureEx(IN const u1* pu1DerSignature, IN u4 u4DerSignatureLen, IN u4 u4ModLen, OUT u1* pu1Signature, INOUT u4* pu4SignatureLen);

#ifdef __cplusplus
};
#endif

#endif
