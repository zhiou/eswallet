#ifndef _ESTLV_H_
#define _ESTLV_H_

#include "../EsTypeDef.h"
#include "../EsCommon/EsCommon.h"

#define MIN_TAGLEN_SIZE	(2)

// 按固定的字节数对齐
#pragma pack(push, 1)

typedef struct BlkTlvTreeTag BlkTlvTree;
struct BlkTlvTreeTag
{
	u4          m_u4Tag;
	u4          m_u4ValueLen;
	u1*         m_pu1Value;		// 如果是复合对象，此值为NULL
	u1          m_u1TagLenSize;
	BlkTlvTree*	m_pblkNext;		// 同级的下一个节点
	BlkTlvTree*	m_pblkChild;	// 判断是否为复合对象的依据
	BlkTlvTree*	m_pblkParent;
};

#pragma pack(pop)

#ifdef __cplusplus
extern "C"
{
#endif
	
	//////////////////////////////////////////////////////////////////////////
	// 回调函数
	//////////////////////////////////////////////////////////////////////////
	/**
	 *  @brief 输入一段数据，返回数据中第一个TLV的T和L的信息。
	 *
	 *  @param[in] pu1Tlv			TLV数据中一段，以TLV中的Tag开始
	 *  @param[in] u4Len	 		数据的长度。长度理论上要保证能够从pu1Tlv中解析出T和L
	 *  @param[out] pu4Tag			从pu1Tlv中解析出来的Tag。可以为NULL
	 *  @param[out] pu4ValueLen		从pu1Tlv中解析出来的value length。可以为NULL
	 *  @param[out] pbIsComplex     0-非复合对象;其它-复合对象。可以为NULL
	 *
	 *  @return size of (Tag and value len) 0 表示数据无效.
	 */
	typedef u1 (CALL_TYPE *FunGetTagAndLen)(IN const u1* pu1Tlv, IN u4 u4Len, OUT u4* pu4Tag, OUT u4* pu4ValueLen, OUT ESBOOL* pbIsComplex);

	/**
	 *  @brief 对Tag进行编码，并输出为字节流
	 *
	 *  @param[in] u4Tag	Tag
	 *  @param[out] pu1Tag	编码后的Tag，需要支持传入NULL求长度
	 *
	 *  @return 编码后Tag占用的字节数
	 */
	typedef u1 (CALL_TYPE *FunEncodeTag)(IN u4 u4Tag, OUT u1* pu1Tag);
	
	/**
	 *  @brief 对Value的长度进行编码，并输出为字节流
	 *
	 *  @param[in] u4ValueLen Value的长度
	 *  @param[out] pu1ValueLen 编码后的长度字节流，即TLV中的L，需要支持传入NULL求长度
	 *
	 *  @return 编码后Length占用的字节数
	 */
	typedef u1 (CALL_TYPE *FunEncodeLength)(IN u4 u4ValueLen, OUT u1* pu1ValueLen);

	//////////////////////////////////////////////////////////////////////////
	// TLV
	//////////////////////////////////////////////////////////////////////////

	/**
	 *  @brief 初始化TLV
	 *
	 *  @param[in] funGetTagAndLen	TAG and len的解码接口。为NULL时使用标准TLV处理方式
	 *  @param[in] funEncodeTag		TAG编码解析接口。为NULL时使用标准TLV处理方式
	 *  @param[in] funEncodeLength	Value length的解码接口。为NULL时使用标准TLV处理方式
	 *  @param[out] phTlv			输出TLV的句柄
	 *
	 *  @return 0: 成功。其它: 失败
	 */
	u4 CALL_TYPE EsTlvInit(IN const FunGetTagAndLen funGetTagAndLen, IN const FunEncodeTag funEncodeTag, IN const FunEncodeLength funEncodeLength, OUT ESHANDLE* phTlv);

	/**
	 *  @brief 反初始化TLV
	 *
	 *  @param[in] hTlv TLV的句柄
	 *
	 *  @return 0: 成功。其它: 失败
	 */
	u4 CALL_TYPE EsTlvDeinit(IN ESHANDLE hTlv);

	/**
	 *  @brief 对tag进行编码
	 *
	 *  @param[in] hTlv				TLV的句柄
	 *  @param[in] u4Tag			tag
	 *  @param[out] pu1Tag			编码后的tag。编码时会回调，缓冲区的长度由调用者确定。可以为NULL。
	 *  @param[in,out] pu1TagSize	输入pu1Tag的长度，输出编码后tag占用的字节数。
	 *
	 *  @return 0: 成功。其它: 失败 
	 */
	u4 CALL_TYPE EsTlvEncodeTag(IN ESHANDLE hTlv, IN u4 u4Tag, OUT u1* pu1Tag, OUT u1* pu1TagSize);
	
	/**
	 *  @brief 对ValueLen进行编码
	 *
	 *  @param[in] hTlv	 TLV的句柄
	 *  @param[in] u4ValueLen ValueLentag
	 *  @param[out] pu1ValueLen 编码后的ValueLen。编码时会回调，缓冲区的长度由调用者确定。可以为NULL。
	 *  @param[in,out] pu1LenSize 输入pu1ValueLen的长度，输出编码后ValueLen占用的字节数。
	 *
	 *  @return 0: 成功。其它: 失败 
	 */
	u4 CALL_TYPE EsTlvEncodeLen(IN ESHANDLE hTlv, IN u4 u4ValueLen, OUT u1* pu1ValueLen, OUT u1* pu1LenSize);
	
	/**
	 *  @brief 由指定的TLV数据创建一个TLV节点。不处理嵌套或者兄弟节点
	 *
	 *  @param[in] hTlv				TLV的句柄
	 *  @param[in] pu1Tlv			TLV数据
	 *  @param[in] u4TlvLen			TLV数据长度
	 *  @param[out] ppblkTlvTree	非空。返回新创建的TLV节点
	 *
	 *  @return 0: 成功。其它: 失败 
	 */
	u4 CALL_TYPE EsTlvNewOneNode(IN ESHANDLE hTlv, IN const u1* pu1Tlv, IN u4 u4TlvLen, OUT BlkTlvTree** ppblkTlvTree);
	
	/**
	 *  @brief 创建一个TLV节点。不处理嵌套或者兄弟节点
	 *
	 *  @param[in] hTlv				TLV的句柄
	 *  @param[in] u4Tag			u4Tag
	 *  @param[in] u4ValueLen		value len
	 *  @param[in] pvValue			value内容
	 *  @param[out] ppblkTlvTree	非空。返回新创建的TLV节点
	 *
	 *  @return 0: 成功。其它: 失败 
	 */
	u4 CALL_TYPE EsTlvNewOneNode2(IN ESHANDLE hTlv, IN u4 u4Tag, IN u4 u4ValueLen, IN const void* pvValue, OUT BlkTlvTree** ppblkTlvTree);
	
	/**
	 *  @brief 释放TLV树内存
	 *
	 *  @param[in,out] ppblkTlvTree TLV数的地址
	 *
	 *  @return 0: 成功。其它: 失败 
	 */
	u4 CALL_TYPE EsTlvTreeFree(INOUT BlkTlvTree** ppblkTlvTree);
	
	/**
	 *  @brief 解码TLV数据，并转换为TLV树（会处理子节点和兄弟节点）
	 *
	 *  @param[in] hTlv				TLV的句柄
	 *  @param[in] pu1Tlv			TLV数据
	 *  @param[in] u4TlvLen			TLV数据长度
	 *  @param[out] ppblkTlvTree	非空。返回新创建的TLV节点
	 *
	 *  @return 0: 成功。其它: 失败 
	 */
	u4 CALL_TYPE EsTlvTreeDeflat(IN ESHANDLE hTlv, IN const u1* pu1Tlv, IN u4 u4TlvLen, OUT BlkTlvTree** ppblkTlvTree);
	
	/**
	 *  @brief 展开TLV数据
	 *
	 *  @param[in] hTlv				TLV的句柄
	 *  @param[in] pblkTlvTree		TLV树地址
	 *  @param[in] pu1Tlv			展开后的TLV数据。支持传入NULL求长度
	 *  @param[in,out] pu4TlvLen		非空。返回展开后的TLV数据占用的内存字节数
	 *
	 *  @return 0: 成功。其它: 失败 
	 */
	u4 CALL_TYPE EsTlvTreeFlat(IN ESHANDLE hTlv, IN const BlkTlvTree* pblkTlvTree, OUT u1* pu1Tlv, INOUT u4* pu4TlvLen);
	
	/**
	 *  @brief 在指定的节点添加子节点
	 *
	 *  @param[in] hTlv					TLV的句柄
	 *  @param[in,out] pblkTlvTreeParent	父节点
	 *  @param[in,out] pblkTlvTreeChild	子节点
	 *
	 *  @return 0: 成功。其它: 失败 
	 *
	 *  @details [注意]!!! 
	 *           1. 父节点直接链接了子节点，子节点已分配的内存会被接管，不需要单独释放
	 *           2. 添加成功后，会更新父节点的所有父节点的长度信息
	 */
	u4 CALL_TYPE EsTlvTreeAddChild(IN ESHANDLE hTlv, INOUT BlkTlvTree* pblkTlvTreeParent, INOUT BlkTlvTree* pblkTlvTreeChild);
	
	/**
	 *  @brief 添加兄弟节点
	 *
	 *  @param[in] hTlv						TLV的句柄
	 *  @param[in,out] pblkTlvTreeRoot		根节点
	 *  @param[in,out] pblkTlvTreeSibling	兄节点
	 *
	 *  @return 0: 成功。其它: 失败 
	 *
	 *  @details [注意]!!! 
	 *           1. 根节点链接了兄弟节点，兄弟节点已分配的内存会被接管，不需要单独释放
	 *           2. 添加成功后，会更新根节点的所有父节点的长度信息
	 */
	u4 CALL_TYPE EsTlvTreeAddSibling(IN ESHANDLE hTlv, INOUT BlkTlvTree* pblkTlvTreeRoot, INOUT BlkTlvTree* pblkTlvTreeSibling);
	
	/**
	 *  @brief 通过tag查找TLV节点
	 *
	 *  @param[in] pblkTlvTree	TLV节点
	 *  @param[in] u4Tag		tag
	 *
	 *  @return 查找到的TLV节点指针 
	 */
	const BlkTlvTree* EsTlvFindViaTag(IN const BlkTlvTree* pblkTlvTree, IN u4 u4Tag);
	
	/**
	 *  @brief 比较两个TLV树是否相等
	 *
	 *  @param[in] pblkTlvTree1	TLV树1
	 *  @param[in] pblkTlvTree2	TLV树2 
	 *
	 *  @return ESFALSE: 不相等
	 */
	ESBOOL CALL_TYPE EsTlvTreeCompare(IN const BlkTlvTree* pblkTlvTree1, IN const BlkTlvTree* pblkTlvTree2);

#ifdef __cplusplus
};
#endif

#endif	// !define _ESTLV_H_
