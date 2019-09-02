#ifndef _ES_LIST_H_
#define _ES_LIST_H_

#ifdef __cplusplus
extern "C" 
{
#endif

	/**
     *  @brief List�ص�-�½�
     *
	 *  @param[in] pvData ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	typedef void*	(CALL_TYPE *FunNewData)(IN const void* pvData);
	
	/**
     *  @brief List�ص�-�ͷ�
     *
	 *  @param[in] pvNodeData �ڵ�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	typedef int		(CALL_TYPE *FunFreeData)(IN void* pvNodeData);
	
	/**
     *  @brief List�ص�-�Ƚ�
	 *
	 *  @param[in] pvData1 ����1
	 *  @param[in] pvData2 ����2
     *
     *  @return ����0-��ȣ�С��0-pvData1<pvData2������0-pvData1>pvData2
     *
     *  @details �ȽϽڵ����Ƿ����ͬ��������
     */
	typedef int		(CALL_TYPE *FunCompareData)(const void* pvData1, IN const void* pvData2);

	/**
     *  @brief List�ص�-����
	 *
	 *  @param[in] pvNodeData �ڵ�
	 *  @param[in] pvParam �������ɵ����ߴ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	typedef u4		(CALL_TYPE *FunTravelData)(IN const void* pvNodeData, IN const void* pvParam);

	/**
     *  @brief List-��ʼ��
	 *
	 *  @param[out] phList �洢��ʼ���ɹ���ľ��
	 *  @param[in] funNewData �����ڵ�ص�������ΪNULLֱ�ӱ����û���������ݣ���EsListAddXXX����
	 *  @param[in] funFreeData �ͷŽڵ�ص�������ΪNULLʱ����������EsListRelease/EsListDeleteXXX����
	 *  @param[in] funCompareData �ȽϽڵ����ݻص�������ΪNULLʱֱ�ӱȽ�Ҫ���ҵ�����ָ��ֵ����EsListFind/EsListDeleteXXX����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListInit(OUT ESHANDLE* phList, IN FunNewData funNewData, IN FunFreeData funFreeData, IN FunCompareData funCompareData);

	/**
     *  @brief List-�ͷ�
	 *
	 *  @param[in] hList ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details �ͷŻص�����ΪNULLʱ�����Խڵ��������κδ���
     */
	u4 CALL_TYPE EsListRelease(IN ESHANDLE hList);

	/**
     *  @brief List-�����½��ڵ�ص�����
	 *
	 *  @param[in] hList ���
	 *  @param[in] funNewData �����ڵ�ص�������ΪNULLֱ�ӱ����û���������ݣ���EsListAddXXX����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListSetFunNew(IN ESHANDLE hList, IN FunNewData funNewData);
    
    /**
     *  @brief List-�����ͷŽڵ�ص�����
     *
     *  @param[in] hList ���
     *  @param[in] funFreeData �ͷŽڵ�ص�������ΪNULLʱ����������EsListRelease/EsListDeleteXXX����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListSetFunFree(IN ESHANDLE hList, IN FunFreeData funFreeData);
    
    /**
     *  @brief List-���ñȽϽڵ�ص�����
     *
     *  @param[in] hList ���
     *  @param[in] funCompareData �ȽϽڵ����ݻص�������ΪNULLʱֱ�ӱȽ�Ҫ���ҵ�����ָ��ֵ����EsListFind/EsListDeleteXXX����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListSetFunCompare(IN ESHANDLE hList, IN FunCompareData funCompareData);

	/**
     *  @brief List-��ͷ���������
	 *
	 *  @param[in] hList ���
	 *  @param[in] pvNodeData ����ӵ�����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *
	 *  @details �޴����ڵ�ص�����ʱ��ֻ��pvNodeData������������
     */
	u4 CALL_TYPE EsListAdd(IN ESHANDLE hList, IN const void* pvNodeData);

	/**
     *  @brief List-��β���������
	 *
	 *  @param[in] hList ���
	 *  @param[in] pvNodeData ����ӵ�����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details �޴����ڵ�ص�����ʱ��ֻ��pvNodeData������������
     */
	u4 CALL_TYPE EsListAddTail(IN ESHANDLE hList, IN const void* pvNodeData);

	/**
     *  @brief List-����ָ���ڵ�
	 *
	 *  @param[in] hList ���
	 *  @param[in] pvFindData ���в��ҵ�����
	 *  @param[out] ppvNodeData ���ҵ��Ľڵ����ݣ������߲���Ҫ�ͷ��ڴ�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details �ޱȽϻص�����ʱ��ֻ�Ǽ򵥵رȽ����ݵ�ַ
     */
	u4 CALL_TYPE EsListFind(IN ESHANDLE hList, IN const void* pvFindData, OUT void** ppvNodeData);
	
	/**
     *  @brief List-ɾ��ָ���ڵ㣨��ɾ��һ����
	 *
	 *  @param[in] hList ���
	 *  @param[in] pvFindData ���в��ҵ�����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details �ޱȽϻص�����ʱ��ֻ�Ǽ򵥵رȽ����ݵ�ַ
     */
	u4 CALL_TYPE EsListDelete(IN ESHANDLE hList, IN const void* pvFindData);
	
	/**
     *  @brief List-ɾ��ָ�������нڵ�
	 *
	 *  @param[in] hList ���
	 *  @param[in] pvFindData ���в��ҵ�����
	 *  @param[in] funCompareData �Ƚϻص�������ΪNULLʱʹ��hList�ڲ��ıȽϻص�����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details �ޱȽϻص�����ʱ��ֻ�Ǽ򵥵رȽ����ݵ�ַ
     */
	u4 CALL_TYPE EsListDeleteAll(IN ESHANDLE hList, IN const void* pvFindData, IN FunCompareData funCompareData);

	/**
     *  @brief List-����һ��IDֵ������ֽڱ���Ϊ00��ֻ���������ֽڵ����ݣ���ʹ�����IDֵ��������������Ψһ��
	 *
	 *  @param[in] hList ���
	 *  @param[out] pu4Id Idֵ
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListGenNewId(IN ESHANDLE hList, OUT u4* pu4Id);
	
	/**
     *  @brief List-���������������������е��ô���Ļص�����������ص���������ʧ�ܣ��򲻼�������
	 *
	 *  @param[in] hList ���
	 *  @param[in] funTravelData �����ص�������������ΪNULL
	 *  @param[in] pvParam �����������ڲ���������ֱ�Ӵ��ݸ�funTravelData
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListTravel(IN ESHANDLE hList, IN FunTravelData funTravelData, IN const void* pvParam);
	
	/**
     *  @brief List-����һ���ڵ�����ݣ��ڲ����ñȽϡ�ɾ�����½��Ļص�����
	 *
	 *  @param[in] hList ���
	 *  @param[in] pvUpdateData �����µ�����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ע�⣺�˺�����ı�ڵ����ݵ�ָ��
     */
	u4 CALL_TYPE EsListUpdate(IN ESHANDLE hList, IN const void* pvUpdateData);

	/**
     *  @brief List-�ϲ������������ս���洢��hListDst��
	 *
	 *  @param[in] hListSrc ��Դ���
	 *  @param[in] hListDst Ŀ����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ע�⣺�˺�����ı�ڵ����ݵ�ָ��
     */
	u4 CALL_TYPE EsListMerge(IN ESHANDLE hListSrc, IN ESHANDLE hListDst);

	/**
     *  @brief List-������������һ���ڵ㱣�������
	 *
	 *  @param[in] hList ���
	 *  @param[in] pvDataPrev �����ҵ����ݣ�ΪNULLʱ��������ͷ
	 *  @param[out] ppvDataNext ����pvDataPrev��������һ���ڵ�����ݣ������߲���Ҫ�ͷ��ڴ�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsListGetNext(IN ESHANDLE hList, IN const void* pvDataPrev, OUT void** ppvDataNext);

	/**
     *  @brief List-��������ʽ������ݵ��ڵ�
	 *
	 *  @param[in] hList ���
	 *  @param[in] pvNodeData ����ӵ�����
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details ��ͷ����ʼ������������ԭ������������ģ��ޱȽϻص�����ʱ��Ĭ����ӵ�ͷ��
     */
	u4 CALL_TYPE EsListAddAscend(IN ESHANDLE hList, IN const void* pvNodeData);

#ifdef __cplusplus
};
#endif

#endif
