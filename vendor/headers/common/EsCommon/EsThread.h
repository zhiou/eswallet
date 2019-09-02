#ifndef _ES_THREAD_H_
#define _ES_THREAD_H_

#ifdef __cplusplus
extern "C" 
{
#endif

	// �߳�״̬
	typedef enum EnThreadState
	{
		ESTHREAD_STATE_UNKNOWN          = 0, // δ֪
		ESTHREAD_STATE_RUNNING          = 1, // ��������
		ESTHREAD_STATE_TO_BE_EXITED     = 2, // ׼���˳�
		ESTHREAD_STATE_EXITED           = 3  // ���˳�/ֹͣ
	}EnThreadState;

	/**
     *  @brief �̻߳ص�����
     *
	 *  @param[in] pvParam �������ɵ����ߵ���EsThreadBeginʱ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ����Ҫ�������Ҫ���߳̽���״̬�����ڻص��ӿ��ڲ���������Ӧ���ڽ���/�˳�ʱ������EsThreadSetState������Ӧ��״̬����
     */
	typedef u4 (CALL_TYPE *FunThreadCallBack)(void* pvParam);

	/**
     *  @brief ����һ���߳�
	 *
	 *  @param[in] funCallBack �̻߳ص�������������ΪNULL
	 *  @param[in] pvParam ��������������ֱ�Ӵ��ݸ�funCallBack
	 *  @param[out] phThread �洢�߳̾��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ����Ҫ�������Ҫ���߳̽���״̬�����ڻص��ӿ��ڲ���������Ӧ���ڽ���/�˳�ʱ������EsThreadSetState������Ӧ��״̬����
     */
	u4 CALL_TYPE EsThreadBegin(IN FunThreadCallBack funCallBack, IN void* pvParam, OUT ESHANDLE* phThread);

	/**
     *  @brief �ж��߳��Ƿ�������Ҫ���˳�״̬
	 *
	 *  @param[in] hThread �߳̾��
     *
     *  @return ESFALSE-û�� other-������״̬
     *
     *  @details none
     */
	ESBOOL CALL_TYPE EsThreadShouldExit(IN ESHANDLE hThread);

	/**
     *  @brief �������ȴ��߳��˳�
	 *
	 *  @param[in] hThread �߳̾��
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsThreadEnd(IN ESHANDLE hThread);
	
	/**
     *  @brief �ر��߳̾�������ȴ��߳��˳���
	 *
	 *  @param[in] hThread �߳̾��
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details �ر��߳̾�����޷������߳�״̬
     */
	u4 CALL_TYPE EsThreadCloseHandle(IN ESHANDLE hThread);
	
	/**
     *  @brief ��ȡ�߳�״̬
	 *
	 *  @param[in] hThread �߳̾��
	 *
	 *  @return ���EnThreadState��������Чʱ����ESTHREAD_STATE_UNKNOWN
     *
     *  @details none
     */
	u4 CALL_TYPE EsThreadGetState(IN ESHANDLE hThread);
	
	/**
     *  @brief �����߳�״̬
	 *
	 *  @param[in] hThread �߳̾��
	 *  @param[in] u4State �߳�״̬�����EnThreadState
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsThreadSetState(IN ESHANDLE hThread, IN u4 u4State);

#ifdef __cplusplus
};
#endif

#endif
