#ifndef _ES_ARRAY_H_
#define _ES_ARRAY_H_

//////////////////////////////////////////////////////////////////////////
// ��̬����
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 *  @brief ��ʼ����̬����
	 *  
	 *  @param[out] phHandle �洢��̬��������������ΪNULL
	 *  
     *  @return 0-�ɹ�������-ʧ�ܣ����EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayInit(OUT ESHANDLE* phHandle);

	/**
	 *  @brief ��ʼ����̬����
	 *  
	 *  @param[out] phHandle �洢��̬��������������ΪNULL
	 *  
     *  @return 0-�ɹ�������-ʧ�ܣ����EsError.h
     *
     *  @details ֻ��*phHandle��Ϊ��ʱ���Ż���г�ʼ��
	 */
	u4 CALL_TYPE EsArrayAssureInit(OUT ESHANDLE* phHandle);

	/**
	 *  @brief ������ݵ���̬����ĩβ
	 *  
	 *  @param[in] hHandle ��̬��������������ΪNULL
	 *  @param[in] pvData ����ӵ����ݣ�ΪNULLʱ����u4DataSize=0�����򱨴�
	 *  @param[in] u4DataSize ����ӵ����ݳ���(�ֽ�),>=0��Ϊ0ʱpvData=NULL�����򱨴�
	 *  
     *  @return 0-�ɹ�������-ʧ�ܣ����EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayAppend(IN ESHANDLE hHandle, IN const void* pvData, IN u4 u4DataSize);

   	/**
	 *  @brief ��ָ��λ��ɾ������
	 *  
	 *  @param[in] hHandle ��̬��������������ΪNULL
	 *  @param[in] u4Offset Ҫɾ�������������е�ƫ���������ֽ�Ϊ��λ
	 *  @param[in] u4DataSize Ҫɾ�����ݵĳ���
	 *  
     *  @return 0-�ɹ�������-ʧ�ܣ����EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayRemoveData(IN ESHANDLE hHandle, IN u4 u4Offset, IN u4 u4DataSize);

	/**
	 *  @brief ��ȡ��̬���������
	 *  
	 *  @param[in] hHandle ��̬��������������ΪNULL
	 *  
     *  @return ��̬����ָ��-�ɹ���NULL-ʧ��
     *
     *  @details �����߲�Ӧ���ͷŷ��ص�ָ������޸���ָ�������
	 */
	void* CALL_TYPE EsArrayGetData(IN ESHANDLE hHandle);

	/**
	 *  @brief ��ȡ��̬�������ݴ��С(�ֽ�)
	 *  
	 *  @param[in] hHandle ��̬��������������ΪNULL
	 *  
     *  @return �Ѵ�������ݵĴ�С
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayGetSize(IN ESHANDLE hHandle);

	/**
	 *  @brief �ͷŶ�̬����
	 *  
	 *  @param[in] hHandle ��̬��������ΪNULLʱ������
	 *  
     *  @return 0-�ɹ�������-ʧ�ܣ����EsError.h
     *
     *  @details ��������Ҫ�������ͷųɹ���Set hHandle=NULL
	 */
	u4 CALL_TYPE EsArrayRelease(IN ESHANDLE hHandle);
	
	/**
	 *  @brief �ͷŶ�̬����
	 *  
	 *  @param[in,out] phHandle ��̬������ָ�룬ΪNULLʱ������
	 *  
     *  @return 0-�ɹ�������-ʧ�ܣ����EsError.h
     *
     *  @details ���óɹ���phHandleִ�еľ������ΪNULL
	 */
	u4 CALL_TYPE EsArrayReleaseEx(INOUT ESHANDLE* phHandle);
	
	/**
	 *  @brief ��ն�̬���������
	 *  
	 *  @param[in] hHandle ��̬��������������ΪNULL
	 *  
     *  @return 0-�ɹ�������-ʧ�ܣ����EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayClear(IN ESHANDLE hHandle);
	
	/**
	 *  @brief ��ȡ������ݵĴ���
	 *  
	 *  @param[in] hHandle ��̬��������������ΪNULL
	 *  
     *  @return ���������=NULLʱ����0
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayGetCount(IN ESHANDLE hHandle);

#ifdef __cplusplus
};
#endif


#endif
