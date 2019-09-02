#ifndef _ES_THREAD_H_
#define _ES_THREAD_H_

#ifdef __cplusplus
extern "C" 
{
#endif

	// 线程状态
	typedef enum EnThreadState
	{
		ESTHREAD_STATE_UNKNOWN          = 0, // 未知
		ESTHREAD_STATE_RUNNING          = 1, // 正在运行
		ESTHREAD_STATE_TO_BE_EXITED     = 2, // 准备退出
		ESTHREAD_STATE_EXITED           = 3  // 已退出/停止
	}EnThreadState;

	/**
     *  @brief 线程回调函数
     *
	 *  @param[in] pvParam 参数，由调用者调用EsThreadBegin时传入
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 【重要】如果需要对线程进行状态管理，在回调接口内部，调用者应该在进入/退出时，调用EsThreadSetState设置相应的状态！！
     */
	typedef u4 (CALL_TYPE *FunThreadCallBack)(void* pvParam);

	/**
     *  @brief 启动一个线程
	 *
	 *  @param[in] funCallBack 线程回调函数，不允许为NULL
	 *  @param[in] pvParam 参数，不解析，直接传递给funCallBack
	 *  @param[out] phThread 存储线程句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 【重要】如果需要对线程进行状态管理，在回调接口内部，调用者应该在进入/退出时，调用EsThreadSetState设置相应的状态！！
     */
	u4 CALL_TYPE EsThreadBegin(IN FunThreadCallBack funCallBack, IN void* pvParam, OUT ESHANDLE* phThread);

	/**
     *  @brief 判断线程是否被设置了要求退出状态
	 *
	 *  @param[in] hThread 线程句柄
     *
     *  @return ESFALSE-没有 other-设置了状态
     *
     *  @details none
     */
	ESBOOL CALL_TYPE EsThreadShouldExit(IN ESHANDLE hThread);

	/**
     *  @brief 结束并等待线程退出
	 *
	 *  @param[in] hThread 线程句柄
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsThreadEnd(IN ESHANDLE hThread);
	
	/**
     *  @brief 关闭线程句柄（不等待线程退出）
	 *
	 *  @param[in] hThread 线程句柄
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 关闭线程句柄后，无法管理线程状态
     */
	u4 CALL_TYPE EsThreadCloseHandle(IN ESHANDLE hThread);
	
	/**
     *  @brief 获取线程状态
	 *
	 *  @param[in] hThread 线程句柄
	 *
	 *  @return 详见EnThreadState，参数无效时返回ESTHREAD_STATE_UNKNOWN
     *
     *  @details none
     */
	u4 CALL_TYPE EsThreadGetState(IN ESHANDLE hThread);
	
	/**
     *  @brief 设置线程状态
	 *
	 *  @param[in] hThread 线程句柄
	 *  @param[in] u4State 线程状态，详见EnThreadState
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsThreadSetState(IN ESHANDLE hThread, IN u4 u4State);

#ifdef __cplusplus
};
#endif

#endif
