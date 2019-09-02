#ifndef _ES_MUTEX_H_
#define _ES_MUTEX_H_

#ifdef __cplusplus
extern "C" 
{
#endif

    /**
     *  @brief ����������
     *
     *  @param[in] szMutexName �¼����ƣ�ΪNULL��""��ʾ����������
     *  @param[out] phMutex �洢mutex���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details ����ź����Ѵ��ڣ���ֱ�Ӵ�
     */
    u4 CALL_TYPE EsMutexCreate(IN const char* szMutexName, OUT ESHANDLE* phMutex); // szMutexName can be NULL
    
#ifndef _OS_WINDOWS_
    /**
     *  @brief ����������
     *
     *  @param[in] szMutexName �¼����ƣ�ΪNULL��""��ʾ����������
     *  @param[out] pnCreated 0-�򿪷�ʽ��õ�;����-������ʽ��õ�
     *  @param[out] phMutex �洢mutex���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details ����ź����Ѵ��ڣ���ֱ�Ӵ�
     */
    u4 CALL_TYPE EsMutexCreateEx(IN const char* szMutexName, int* pnCreated, OUT ESHANDLE* phMutex);
#endif
    
    /**
     *  @brief ������������
     *
     *  @param[in] szMutexName �¼����ƣ�������Ϊ��
     *  @param[out] phMutex �洢mutex���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details �����ڻᱨ��
     */
	u4 CALL_TYPE EsMutexOpen(IN const char* szMutexName, OUT ESHANDLE* phMutex);

    /**
     *  @brief ��ȡ������
     *
     *  @param[in] hMutex ���
     *  @param[in] u4TimeOut ��ʱʱ�䣨ms��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsMutexWait(IN ESHANDLE hMutex, IN u4 u4TimeOut);
	
    /**
     *  @brief �ͷŻ�����
     *
     *  @param[in] hMutex ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details none
     */
	u4 CALL_TYPE EsMutexRelease(IN ESHANDLE hMutex);
	
    /**
     *  @brief ���ٻ�����
     *
     *  @param[in] hMutex ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details none
     */
	u4 CALL_TYPE EsMutexDestroy(IN ESHANDLE hMutex);

#ifdef __cplusplus
};
#endif

#endif

	
