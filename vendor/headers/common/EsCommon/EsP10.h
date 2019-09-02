#ifndef _ESP10_H_
#define _ESP10_H_

#ifdef __cplusplus
extern "C"
{
#endif
    
	typedef enum EnP10AttrTypeTag
	{
		ESP10_ATTR_TYPE_DEFAULT = 0, // Ĭ��
		ESP10_ATTR_TYPE_CFCA1 = 1,	// cfca��ʽ��������CGB��Ŀ��ʹ��
	}EnP10AttrType;

    /**
     *  @brief ����P10������Ϣ
     *
	 *  @param[in] pblkAsymKey ������Ϣ��Ӧ�Ĺ�Կ
     *  @param[in] pblkAsymKeyTemp ��ʱ��Կ��˫֤����ʱ��Ч
     *  @param[in] pszDn ֤��DN����ʽΪ"CN=XX,E=XXX,OU=XXX"
     *  @param[out] pvRequestInfo ���P10��ʽ��������Ϣ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4RequestInfoLen ����������/������Ϣ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
    u4 CALL_TYPE EsP10GenRequestInfo(IN const BlkAsymKey* pblkAsymKey,
                                     IN const BlkAsymKey* pblkAsymKeyTemp,
                                     IN const char* pszDn,
                                     OUT void* pvRequestInfo,
                                     INOUT u4* pu4RequestInfoLen);
    
    /**
     *  @brief ����P10������Ϣ
     *
     *  @param[in] u4P10AttrType P10�����ͣ���Ҫ��������Ϣ��һ���������EnP10AttrType
     *  @param[in] pblkAsymKey ������Ϣ��Ӧ�Ĺ�Կ
     *  @param[in] pblkAsymKeyTemp ��ʱ��Կ��˫֤����ʱ��Ч
     *  @param[in] pszDn ֤��DN����ʽΪ"CN=XX,E=XXX,OU=XXX"
     *  @param[out] pvRequestInfo ���P10��ʽ��������Ϣ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4RequestInfoLen ����������/������Ϣ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
    u4 CALL_TYPE EsP10GenRequestInfoWithType(IN u4 u4P10AttrType,
											 IN const BlkAsymKey* pblkAsymKey,
											 IN const BlkAsymKey* pblkAsymKeyTemp, 
											 IN const char* pszDn,
											 OUT void* pvRequestInfo,
											 INOUT u4* pu4RequestInfoLen);

    /**
     *  @brief ����P10����
     *
     *  @param[in] pvRequstInfo P10������Ϣ����EsP10GenRequestInfo����
     *  @param[in] u4RequestInfoLen ������Ϣ����
     *  @param[in] pvSignature ˽Կ��������Ϣ��ǩ��ֵ
     *  @param[in] u4HashAlgId ǩ��ʹ�õ�Hash�㷨Id
     *  @param[out] pvRequest P10������Ϣ��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4RequestLen ����������/P10���󳤶�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
    u4 CALL_TYPE EsP10GenRequest(IN const void* pvRequstInfo,
                                 IN u4 u4RequestInfoLen,
                                 IN const void* pvSignature,
                                 IN u4 u4HashAlgId,
                                 OUT void* pvRequest,
                                 INOUT u4* pu4RequestLen);
    /**
     *  @brief ����P10����(��������Ĭ��ΪESP10_ATTR_TYPE_DEFAULT)
     *
     *  @param[in] pvRequest P10������EsP10GenRequest����
     *  @param[in] u4RequestLen P10���󳤶�
     *  @param[out] pblkAsymKeySign P10��ǩ����Կ
     *  @param[out] pblkAsymTemp ��ʱ��Կ
     *  @param[out] pszDn ֤��DN��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4DnLen ����������/֤��DN����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details TODO: �����Ҫ����������ʽ��P10�����Բ�ͬ��������Ҫ����µķ���!!
     */
    u4 CALL_TYPE EsP10ParseRequest(IN const void* pvRequest, 
                                   IN u4 u4RequestLen, 
                                   OUT BlkAsymKey* pblkAsymKeySign,
                                   OUT BlkAsymKey* pblkAsymTemp,
                                   OUT char* pszDn, 
                                   INOUT u4* pu4DnLen);
#ifdef __cplusplus
};
#endif

#endif // !defined_ESP10_H_
