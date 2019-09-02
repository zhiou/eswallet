#ifndef _ES_EVENT_H_
#define _ES_EVENT_H_

#ifdef __cplusplus
extern "C" 
{
#endif

    /**
     *  @brief �����¼�
     *
     *  @param[in] szEventName �¼����ƣ�ΪNULL��""��ʾ�����¼�
     *  @param[out] phEvent �洢�¼����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details ����¼��Ѵ��ڣ���ֱ�Ӵ�
     */
	u4 CALL_TYPE EsEventCreate(IN const char* szEventName, OUT ESHANDLE* phEvent);
    
#ifndef _OS_WINDOWS_
    /**
     *  @brief �����¼�
     *
     *  @param[in] szEventName �¼����ƣ�ΪNULL��""��ʾ�����¼�
     *  @param[out] phEvent �洢�¼����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details ����¼��Ѵ��ڣ���ֱ�Ӵ�
     */
    u4 CALL_TYPE EsEventCreateEx(IN const char* szEventName, OUT int* pnCreated, OUT ESHANDLE* phEvent);
#endif
    
    /**
     *  @brief �������¼�
     *
     *  @param[in] szEventName �¼����ƣ�������ΪNULL��""
     *  @param[out] phEvent �洢�¼����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventOpen(IN const char* szEventName, OUT ESHANDLE* phEvent);

    /**
     *  @brief �ر��¼�
     *
     *  @param[in] hEvent �¼����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventClose(IN ESHANDLE hEvent);

    /**
     *  @brief �����¼�
     *
     *  @param[in] hEvent �¼����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventSet(IN ESHANDLE hEvent);
	
    /**
     *  @brief ��λ�¼�
     *
     *  @param[in] hEvent �¼����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventReset(IN ESHANDLE hEvent);
	
#define ES_WAIT_OBJ_0 0
	
    /**
     *  @brief �ȴ��¼�����
     *
	 *  @param[in] hEvent �¼����
	 *  @param[in] u4Timeout �ȴ���ʱʱ��(ms)
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��TODO Windows����ϵͳ�����룩
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventWait(IN ESHANDLE hEvent, IN u4 u4Timeout);

#ifdef __cplusplus
};
#endif
	
#endif
