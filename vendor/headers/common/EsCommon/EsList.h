#ifndef _ES_LIST_H_
#define _ES_LIST_H_

#ifdef __cplusplus
extern "C" 
{
#endif

	/**
     *  @brief List回调-新建
     *
	 *  @param[in] pvData 数据
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	typedef void*	(CALL_TYPE *FunNewData)(IN const void* pvData);
	
	/**
     *  @brief List回调-释放
     *
	 *  @param[in] pvNodeData 节点
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	typedef int		(CALL_TYPE *FunFreeData)(IN void* pvNodeData);
	
	/**
     *  @brief List回调-比较
	 *
	 *  @param[in] pvData1 数据1
	 *  @param[in] pvData2 数据2
     *
     *  @return 等于0-相等；小于0-pvData1<pvData2；大于0-pvData1>pvData2
     *
     *  @details 比较节点中是否存在同样的数据
     */
	typedef int		(CALL_TYPE *FunCompareData)(const void* pvData1, IN const void* pvData2);

	/**
     *  @brief List回调-遍历
	 *
	 *  @param[in] pvNodeData 节点
	 *  @param[in] pvParam 参数，由调用者传入
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	typedef u4		(CALL_TYPE *FunTravelData)(IN const void* pvNodeData, IN const void* pvParam);

	/**
     *  @brief List-初始化
	 *
	 *  @param[out] phList 存储初始化成功后的句柄
	 *  @param[in] funNewData 创建节点回调函数，为NULL直接保存用户传入的数据，在EsListAddXXX调用
	 *  @param[in] funFreeData 释放节点回调函数，为NULL时不做处理，在EsListRelease/EsListDeleteXXX调用
	 *  @param[in] funCompareData 比较节点数据回调函数，为NULL时直接比较要查找的数据指针值，在EsListFind/EsListDeleteXXX调用
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListInit(OUT ESHANDLE* phList, IN FunNewData funNewData, IN FunFreeData funFreeData, IN FunCompareData funCompareData);

	/**
     *  @brief List-释放
	 *
	 *  @param[in] hList 句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 释放回调函数为NULL时，不对节点数据做任何处理
     */
	u4 CALL_TYPE EsListRelease(IN ESHANDLE hList);

	/**
     *  @brief List-设置新建节点回调函数
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] funNewData 创建节点回调函数，为NULL直接保存用户传入的数据，在EsListAddXXX调用
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListSetFunNew(IN ESHANDLE hList, IN FunNewData funNewData);
    
    /**
     *  @brief List-设置释放节点回调函数
     *
     *  @param[in] hList 句柄
     *  @param[in] funFreeData 释放节点回调函数，为NULL时不做处理，在EsListRelease/EsListDeleteXXX调用
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListSetFunFree(IN ESHANDLE hList, IN FunFreeData funFreeData);
    
    /**
     *  @brief List-设置比较节点回调函数
     *
     *  @param[in] hList 句柄
     *  @param[in] funCompareData 比较节点数据回调函数，为NULL时直接比较要查找的数据指针值，在EsListFind/EsListDeleteXXX调用
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListSetFunCompare(IN ESHANDLE hList, IN FunCompareData funCompareData);

	/**
     *  @brief List-在头部添加数据
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] pvNodeData 待添加的数据
     *
     *  @return 0-成功 other-失败，详见EsError.h
	 *
	 *  @details 无创建节点回调函数时，只将pvNodeData保存在链表中
     */
	u4 CALL_TYPE EsListAdd(IN ESHANDLE hList, IN const void* pvNodeData);

	/**
     *  @brief List-在尾部添加数据
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] pvNodeData 待添加的数据
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 无创建节点回调函数时，只将pvNodeData保存在链表中
     */
	u4 CALL_TYPE EsListAddTail(IN ESHANDLE hList, IN const void* pvNodeData);

	/**
     *  @brief List-查找指定节点
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] pvFindData 进行查找的数据
	 *  @param[out] ppvNodeData 查找到的节点数据，调用者不需要释放内存
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 无比较回调函数时，只是简单地比较数据地址
     */
	u4 CALL_TYPE EsListFind(IN ESHANDLE hList, IN const void* pvFindData, OUT void** ppvNodeData);
	
	/**
     *  @brief List-删除指定节点（仅删除一个）
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] pvFindData 进行查找的数据
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 无比较回调函数时，只是简单地比较数据地址
     */
	u4 CALL_TYPE EsListDelete(IN ESHANDLE hList, IN const void* pvFindData);
	
	/**
     *  @brief List-删除指定的所有节点
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] pvFindData 进行查找的数据
	 *  @param[in] funCompareData 比较回调函数，为NULL时使用hList内部的比较回调函数
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 无比较回调函数时，只是简单地比较数据地址
     */
	u4 CALL_TYPE EsListDeleteAll(IN ESHANDLE hList, IN const void* pvFindData, IN FunCompareData funCompareData);

	/**
     *  @brief List-生成一个ID值（最高字节保持为00，只生成三个字节的数据），使得这个ID值在整个链表中是唯一的
	 *
	 *  @param[in] hList 句柄
	 *  @param[out] pu4Id Id值
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListGenNewId(IN ESHANDLE hList, OUT u4* pu4Id);
	
	/**
     *  @brief List-遍历整个链表，遍历过程中调用传入的回调函数，如果回调函数返回失败，则不继续遍历
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] funTravelData 遍历回调函数，不允许为NULL
	 *  @param[in] pvParam 遍历参数，内部不做处理，直接传递给funTravelData
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListTravel(IN ESHANDLE hList, IN FunTravelData funTravelData, IN const void* pvParam);
	
	/**
     *  @brief List-更新一个节点的内容，内部调用比较、删除、新建的回调函数
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] pvUpdateData 待更新的数据
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 注意：此函数会改变节点数据的指针
     */
	u4 CALL_TYPE EsListUpdate(IN ESHANDLE hList, IN const void* pvUpdateData);

	/**
     *  @brief List-合并两个链表，最终结果存储在hListDst中
	 *
	 *  @param[in] hListSrc 来源句柄
	 *  @param[in] hListDst 目标句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 注意：此函数会改变节点数据的指针
     */
	u4 CALL_TYPE EsListMerge(IN ESHANDLE hListSrc, IN ESHANDLE hListDst);

	/**
     *  @brief List-查找链表中下一个节点保存的数据
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] pvDataPrev 待查找的数据，为NULL时查找链表头
	 *  @param[out] ppvDataNext 满足pvDataPrev条件的下一个节点的数据，调用者不需要释放内存
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListGetNext(IN ESHANDLE hList, IN const void* pvDataPrev, OUT void** ppvDataNext);

	/**
     *  @brief List-按照升序方式添加数据到节点
	 *
	 *  @param[in] hList 句柄
	 *  @param[in] pvNodeData 待添加的数据
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details 从头部开始检索，不考虑原有数据是无序的，无比较回调函数时，默认添加到头部
     */
	u4 CALL_TYPE EsListAddAscend(IN ESHANDLE hList, IN const void* pvNodeData);

#ifdef __cplusplus
};
#endif

#endif
