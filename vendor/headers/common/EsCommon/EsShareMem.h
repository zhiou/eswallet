#ifndef _ES_SHARE_MEM_H_
#define _ES_SHARE_MEM_H_

#ifdef __cplusplus
extern "C" 
{
#endif
    
    //////////////////////////////////////////////////////////////////////////
    // �����ڴ�
    //////////////////////////////////////////////////////////////////////////
    
    /**
     *  @brief ����/�򿪹����ڴ�
     *
     *  @param[in] szMapName �����ڴ�����
     *  @param[in] u4Size �����ڴ��С������0��ʾ���򿪣������ڴ�ʧ��ʱ�Զ�����
     *  @param[out] phMap �洢�����ڴ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemOpen(IN const char* szMapName, IN u4 u4Size, OUT ESHANDLE* phMap);
    
    /**
     *  @brief �رչ����ڴ�
     *
     *  @param[in] hMap �����ڴ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ���������ڴ����ü���Ϊ0ʱ��ϵͳ�Ż������ͷŹ����ڴ����
     */
	u4 CALL_TYPE EsShareMemClose(IN ESHANDLE hMap);
    
    /**
     *  @brief ��ȡ�����ڴ��ַ
     *
     *  @param[in] hMap �����ڴ���
     *  @param[out] ppvMap �洢��ַ
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemGetData(IN ESHANDLE hMap, OUT void** ppvMap);
    
    /**
     *  @brief ͬ��-��ȡ�����ڴ���
     *
	 *  @param[in] hMap �����ڴ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemLock(IN ESHANDLE hMap);
    
    /**
     *  @brief ͬ��-��ȡ�����ڴ���
     *
     *  @param[in] hMap �����ڴ���
     *  @param[in] u4Timeout �ȴ���ʱʱ��(ms)
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemLockTimeout(IN ESHANDLE hMap, IN u4 u4Timeout);
    
    /**
     *  @brief ͬ��-�ͷŹ����ڴ���
     *
     *  @param[in] hMap �����ڴ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemUnlock(IN ESHANDLE hMap);
    
    /**
     *  @brief ��ȡ�����ڴ��С
     *
     *  @param[in] hMap �����ڴ���
     *  @param[out] pu4Size �洢��С��������ΪNULL
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemGetSize(IN ESHANDLE hMap, OUT u4* pu4Size);
    
    /**
     *  @brief ��ȡ�����ڴ�����
     *
     *  @param[in] hMap �����ڴ���
     *  @param[in] u4Offset ƫ�Ƶ�ַ
     *  @param[out] pvData �洢���ݣ�������ΪNULL
     *  @param[in] u4DataLen ����ȡ�Ĵ�С������0ʱ������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemRead(IN ESHANDLE hMap, IN u4 u4Offset, OUT void* pvData, IN u4 u4DataLen);
    
    /**
     *  @brief д�빲���ڴ�����
     *
     *  @param[in] hMap �����ڴ���
     *  @param[in] u4Offset ƫ�Ƶ�ַ
     *  @param[in] pvData ���ݣ�������ΪNULL
     *  @param[in] u4DataLen ��д��Ĵ�С������0ʱ������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemWrite(IN ESHANDLE hMap, IN u4 u4Offset, IN const void* pvData, IN u4 u4DataLen);

#ifdef __cplusplus
};
#endif

#endif
