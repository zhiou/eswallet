#ifndef _ES_MUTEX_H_
#define _ES_MUTEX_H_

#ifdef __cplusplus
extern "C" 
{
#endif

    /**
     *  @brief 创建互斥量
     *
     *  @param[in] szMutexName 事件名称，为NULL或""表示无名互斥量
     *  @param[out] phMutex 存储mutex句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details 如果信号量已存在，则直接打开
     */
    u4 CALL_TYPE EsMutexCreate(IN const char* szMutexName, OUT ESHANDLE* phMutex); // szMutexName can be NULL
    
#ifndef _OS_WINDOWS_
    /**
     *  @brief 创建互斥量
     *
     *  @param[in] szMutexName 事件名称，为NULL或""表示无名互斥量
     *  @param[out] pnCreated 0-打开方式获得的;其他-创建方式获得的
     *  @param[out] phMutex 存储mutex句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details 如果信号量已存在，则直接打开
     */
    u4 CALL_TYPE EsMutexCreateEx(IN const char* szMutexName, int* pnCreated, OUT ESHANDLE* phMutex);
#endif
    
    /**
     *  @brief 打开有名互斥量
     *
     *  @param[in] szMutexName 事件名称，不允许为空
     *  @param[out] phMutex 存储mutex句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details 不存在会报错
     */
	u4 CALL_TYPE EsMutexOpen(IN const char* szMutexName, OUT ESHANDLE* phMutex);

    /**
     *  @brief 获取互斥量
     *
     *  @param[in] hMutex 句柄
     *  @param[in] u4TimeOut 超时时间（ms）
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsMutexWait(IN ESHANDLE hMutex, IN u4 u4TimeOut);
	
    /**
     *  @brief 释放互斥量
     *
     *  @param[in] hMutex 句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details none
     */
	u4 CALL_TYPE EsMutexRelease(IN ESHANDLE hMutex);
	
    /**
     *  @brief 销毁互斥量
     *
     *  @param[in] hMutex 句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details none
     */
	u4 CALL_TYPE EsMutexDestroy(IN ESHANDLE hMutex);

#ifdef __cplusplus
};
#endif

#endif

	
