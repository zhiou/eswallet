#ifndef _ES_SHARE_MEM_H_
#define _ES_SHARE_MEM_H_

#ifdef __cplusplus
extern "C" 
{
#endif
    
    //////////////////////////////////////////////////////////////////////////
    // 共享内存
    //////////////////////////////////////////////////////////////////////////
    
    /**
     *  @brief 创建/打开共享内存
     *
     *  @param[in] szMapName 共享内存名称
     *  @param[in] u4Size 共享内存大小，等于0表示仅打开，否则，在打开失败时自动创建
     *  @param[out] phMap 存储共享内存句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemOpen(IN const char* szMapName, IN u4 u4Size, OUT ESHANDLE* phMap);
    
    /**
     *  @brief 关闭共享内存
     *
     *  @param[in] hMap 共享内存句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 仅当共享内存引用计数为0时，系统才会真正释放共享内存对象
     */
	u4 CALL_TYPE EsShareMemClose(IN ESHANDLE hMap);
    
    /**
     *  @brief 获取共享内存地址
     *
     *  @param[in] hMap 共享内存句柄
     *  @param[out] ppvMap 存储地址
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemGetData(IN ESHANDLE hMap, OUT void** ppvMap);
    
    /**
     *  @brief 同步-获取共享内存锁
     *
	 *  @param[in] hMap 共享内存句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemLock(IN ESHANDLE hMap);
    
    /**
     *  @brief 同步-获取共享内存锁
     *
     *  @param[in] hMap 共享内存句柄
     *  @param[in] u4Timeout 等待超时时间(ms)
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemLockTimeout(IN ESHANDLE hMap, IN u4 u4Timeout);
    
    /**
     *  @brief 同步-释放共享内存锁
     *
     *  @param[in] hMap 共享内存句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemUnlock(IN ESHANDLE hMap);
    
    /**
     *  @brief 获取共享内存大小
     *
     *  @param[in] hMap 共享内存句柄
     *  @param[out] pu4Size 存储大小，不允许为NULL
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemGetSize(IN ESHANDLE hMap, OUT u4* pu4Size);
    
    /**
     *  @brief 读取共享内存数据
     *
     *  @param[in] hMap 共享内存句柄
     *  @param[in] u4Offset 偏移地址
     *  @param[out] pvData 存储数据，不允许为NULL
     *  @param[in] u4DataLen 待读取的大小，等于0时不报错
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemRead(IN ESHANDLE hMap, IN u4 u4Offset, OUT void* pvData, IN u4 u4DataLen);
    
    /**
     *  @brief 写入共享内存数据
     *
     *  @param[in] hMap 共享内存句柄
     *  @param[in] u4Offset 偏移地址
     *  @param[in] pvData 数据，不允许为NULL
     *  @param[in] u4DataLen 待写入的大小，等于0时不报错
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsShareMemWrite(IN ESHANDLE hMap, IN u4 u4Offset, IN const void* pvData, IN u4 u4DataLen);

#ifdef __cplusplus
};
#endif

#endif
