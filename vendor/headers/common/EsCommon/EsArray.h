#ifndef _ES_ARRAY_H_
#define _ES_ARRAY_H_

//////////////////////////////////////////////////////////////////////////
// 动态数组
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 *  @brief 初始化动态数组
	 *  
	 *  @param[out] phHandle 存储动态数组句柄，不允许为NULL
	 *  
     *  @return 0-成功；其他-失败，详见EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayInit(OUT ESHANDLE* phHandle);

	/**
	 *  @brief 初始化动态数组
	 *  
	 *  @param[out] phHandle 存储动态数组句柄，不允许为NULL
	 *  
     *  @return 0-成功；其他-失败，详见EsError.h
     *
     *  @details 只有*phHandle不为空时，才会进行初始化
	 */
	u4 CALL_TYPE EsArrayAssureInit(OUT ESHANDLE* phHandle);

	/**
	 *  @brief 添加数据到动态数组末尾
	 *  
	 *  @param[in] hHandle 动态数组句柄，不允许为NULL
	 *  @param[in] pvData 待添加的数据，为NULL时，若u4DataSize=0，否则报错
	 *  @param[in] u4DataSize 待添加的数据长度(字节),>=0，为0时pvData=NULL，否则报错
	 *  
     *  @return 0-成功；其他-失败，详见EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayAppend(IN ESHANDLE hHandle, IN const void* pvData, IN u4 u4DataSize);

   	/**
	 *  @brief 从指定位置删除数据
	 *  
	 *  @param[in] hHandle 动态数组句柄，不允许为NULL
	 *  @param[in] u4Offset 要删除数据在数组中的偏移量，以字节为单位
	 *  @param[in] u4DataSize 要删除数据的长度
	 *  
     *  @return 0-成功；其他-失败，详见EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayRemoveData(IN ESHANDLE hHandle, IN u4 u4Offset, IN u4 u4DataSize);

	/**
	 *  @brief 获取动态数组的数据
	 *  
	 *  @param[in] hHandle 动态数组句柄，不允许为NULL
	 *  
     *  @return 动态数组指针-成功；NULL-失败
     *
     *  @details 调用者不应该释放返回的指针或者修改其指向的数据
	 */
	void* CALL_TYPE EsArrayGetData(IN ESHANDLE hHandle);

	/**
	 *  @brief 获取动态数组数据大大小(字节)
	 *  
	 *  @param[in] hHandle 动态数组句柄，不允许为NULL
	 *  
     *  @return 已存入的数据的大小
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayGetSize(IN ESHANDLE hHandle);

	/**
	 *  @brief 释放动态数组
	 *  
	 *  @param[in] hHandle 动态数组句柄，为NULL时不报错
	 *  
     *  @return 0-成功；其他-失败，详见EsError.h
     *
     *  @details 调用者需要自行在释放成功后Set hHandle=NULL
	 */
	u4 CALL_TYPE EsArrayRelease(IN ESHANDLE hHandle);
	
	/**
	 *  @brief 释放动态数组
	 *  
	 *  @param[in,out] phHandle 动态数组句柄指针，为NULL时不报错
	 *  
     *  @return 0-成功；其他-失败，详见EsError.h
     *
     *  @details 调用成功后，phHandle执行的句柄被置为NULL
	 */
	u4 CALL_TYPE EsArrayReleaseEx(INOUT ESHANDLE* phHandle);
	
	/**
	 *  @brief 清空动态数组的数据
	 *  
	 *  @param[in] hHandle 动态数组句柄，不允许为NULL
	 *  
     *  @return 0-成功；其他-失败，详见EsError.h
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayClear(IN ESHANDLE hHandle);
	
	/**
	 *  @brief 获取添加数据的次数
	 *  
	 *  @param[in] hHandle 动态数组句柄，不允许为NULL
	 *  
     *  @return 次数。句柄=NULL时返回0
     *
     *  @details none
	 */
	u4 CALL_TYPE EsArrayGetCount(IN ESHANDLE hHandle);

#ifdef __cplusplus
};
#endif


#endif
