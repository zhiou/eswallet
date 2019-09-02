#ifndef _ES_EVENT_H_
#define _ES_EVENT_H_

#ifdef __cplusplus
extern "C" 
{
#endif

    /**
     *  @brief 创建事件
     *
     *  @param[in] szEventName 事件名称，为NULL或""表示无名事件
     *  @param[out] phEvent 存储事件句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details 如果事件已存在，则直接打开
     */
	u4 CALL_TYPE EsEventCreate(IN const char* szEventName, OUT ESHANDLE* phEvent);
    
#ifndef _OS_WINDOWS_
    /**
     *  @brief 创建事件
     *
     *  @param[in] szEventName 事件名称，为NULL或""表示无名事件
     *  @param[out] phEvent 存储事件句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details 如果事件已存在，则直接打开
     */
    u4 CALL_TYPE EsEventCreateEx(IN const char* szEventName, OUT int* pnCreated, OUT ESHANDLE* phEvent);
#endif
    
    /**
     *  @brief 打开有名事件
     *
     *  @param[in] szEventName 事件名称，不允许为NULL或""
     *  @param[out] phEvent 存储事件句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventOpen(IN const char* szEventName, OUT ESHANDLE* phEvent);

    /**
     *  @brief 关闭事件
     *
     *  @param[in] hEvent 事件句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventClose(IN ESHANDLE hEvent);

    /**
     *  @brief 设置事件
     *
     *  @param[in] hEvent 事件句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventSet(IN ESHANDLE hEvent);
	
    /**
     *  @brief 复位事件
     *
     *  @param[in] hEvent 事件句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventReset(IN ESHANDLE hEvent);
	
#define ES_WAIT_OBJ_0 0
	
    /**
     *  @brief 等待事件发生
     *
	 *  @param[in] hEvent 事件句柄
	 *  @param[in] u4Timeout 等待超时时间(ms)
     *
     *  @return 0-成功 other-失败，详见EsError.h（TODO Windows返回系统错误码）
     *
     *  @details none
     */
	u4 CALL_TYPE EsEventWait(IN ESHANDLE hEvent, IN u4 u4Timeout);

#ifdef __cplusplus
};
#endif
	
#endif
