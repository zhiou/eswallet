#ifndef _ESTLV_H_
#define _ESTLV_H_

#include "../EsTypeDef.h"
#include "../EsCommon/EsCommon.h"

#define MIN_TAGLEN_SIZE	(2)

// ���̶����ֽ�������
#pragma pack(push, 1)

typedef struct BlkTlvTreeTag BlkTlvTree;
struct BlkTlvTreeTag
{
	u4          m_u4Tag;
	u4          m_u4ValueLen;
	u1*         m_pu1Value;		// ����Ǹ��϶��󣬴�ֵΪNULL
	u1          m_u1TagLenSize;
	BlkTlvTree*	m_pblkNext;		// ͬ������һ���ڵ�
	BlkTlvTree*	m_pblkChild;	// �ж��Ƿ�Ϊ���϶��������
	BlkTlvTree*	m_pblkParent;
};

#pragma pack(pop)

#ifdef __cplusplus
extern "C"
{
#endif
	
	//////////////////////////////////////////////////////////////////////////
	// �ص�����
	//////////////////////////////////////////////////////////////////////////
	/**
	 *  @brief ����һ�����ݣ����������е�һ��TLV��T��L����Ϣ��
	 *
	 *  @param[in] pu1Tlv			TLV������һ�Σ���TLV�е�Tag��ʼ
	 *  @param[in] u4Len	 		���ݵĳ��ȡ�����������Ҫ��֤�ܹ���pu1Tlv�н�����T��L
	 *  @param[out] pu4Tag			��pu1Tlv�н���������Tag������ΪNULL
	 *  @param[out] pu4ValueLen		��pu1Tlv�н���������value length������ΪNULL
	 *  @param[out] pbIsComplex     0-�Ǹ��϶���;����-���϶��󡣿���ΪNULL
	 *
	 *  @return size of (Tag and value len) 0 ��ʾ������Ч.
	 */
	typedef u1 (CALL_TYPE *FunGetTagAndLen)(IN const u1* pu1Tlv, IN u4 u4Len, OUT u4* pu4Tag, OUT u4* pu4ValueLen, OUT ESBOOL* pbIsComplex);

	/**
	 *  @brief ��Tag���б��룬�����Ϊ�ֽ���
	 *
	 *  @param[in] u4Tag	Tag
	 *  @param[out] pu1Tag	������Tag����Ҫ֧�ִ���NULL�󳤶�
	 *
	 *  @return �����Tagռ�õ��ֽ���
	 */
	typedef u1 (CALL_TYPE *FunEncodeTag)(IN u4 u4Tag, OUT u1* pu1Tag);
	
	/**
	 *  @brief ��Value�ĳ��Ƚ��б��룬�����Ϊ�ֽ���
	 *
	 *  @param[in] u4ValueLen Value�ĳ���
	 *  @param[out] pu1ValueLen �����ĳ����ֽ�������TLV�е�L����Ҫ֧�ִ���NULL�󳤶�
	 *
	 *  @return �����Lengthռ�õ��ֽ���
	 */
	typedef u1 (CALL_TYPE *FunEncodeLength)(IN u4 u4ValueLen, OUT u1* pu1ValueLen);

	//////////////////////////////////////////////////////////////////////////
	// TLV
	//////////////////////////////////////////////////////////////////////////

	/**
	 *  @brief ��ʼ��TLV
	 *
	 *  @param[in] funGetTagAndLen	TAG and len�Ľ���ӿڡ�ΪNULLʱʹ�ñ�׼TLV����ʽ
	 *  @param[in] funEncodeTag		TAG��������ӿڡ�ΪNULLʱʹ�ñ�׼TLV����ʽ
	 *  @param[in] funEncodeLength	Value length�Ľ���ӿڡ�ΪNULLʱʹ�ñ�׼TLV����ʽ
	 *  @param[out] phTlv			���TLV�ľ��
	 *
	 *  @return 0: �ɹ�������: ʧ��
	 */
	u4 CALL_TYPE EsTlvInit(IN const FunGetTagAndLen funGetTagAndLen, IN const FunEncodeTag funEncodeTag, IN const FunEncodeLength funEncodeLength, OUT ESHANDLE* phTlv);

	/**
	 *  @brief ����ʼ��TLV
	 *
	 *  @param[in] hTlv TLV�ľ��
	 *
	 *  @return 0: �ɹ�������: ʧ��
	 */
	u4 CALL_TYPE EsTlvDeinit(IN ESHANDLE hTlv);

	/**
	 *  @brief ��tag���б���
	 *
	 *  @param[in] hTlv				TLV�ľ��
	 *  @param[in] u4Tag			tag
	 *  @param[out] pu1Tag			������tag������ʱ��ص����������ĳ����ɵ�����ȷ��������ΪNULL��
	 *  @param[in,out] pu1TagSize	����pu1Tag�ĳ��ȣ���������tagռ�õ��ֽ�����
	 *
	 *  @return 0: �ɹ�������: ʧ�� 
	 */
	u4 CALL_TYPE EsTlvEncodeTag(IN ESHANDLE hTlv, IN u4 u4Tag, OUT u1* pu1Tag, OUT u1* pu1TagSize);
	
	/**
	 *  @brief ��ValueLen���б���
	 *
	 *  @param[in] hTlv	 TLV�ľ��
	 *  @param[in] u4ValueLen ValueLentag
	 *  @param[out] pu1ValueLen ������ValueLen������ʱ��ص����������ĳ����ɵ�����ȷ��������ΪNULL��
	 *  @param[in,out] pu1LenSize ����pu1ValueLen�ĳ��ȣ���������ValueLenռ�õ��ֽ�����
	 *
	 *  @return 0: �ɹ�������: ʧ�� 
	 */
	u4 CALL_TYPE EsTlvEncodeLen(IN ESHANDLE hTlv, IN u4 u4ValueLen, OUT u1* pu1ValueLen, OUT u1* pu1LenSize);
	
	/**
	 *  @brief ��ָ����TLV���ݴ���һ��TLV�ڵ㡣������Ƕ�׻����ֵܽڵ�
	 *
	 *  @param[in] hTlv				TLV�ľ��
	 *  @param[in] pu1Tlv			TLV����
	 *  @param[in] u4TlvLen			TLV���ݳ���
	 *  @param[out] ppblkTlvTree	�ǿա������´�����TLV�ڵ�
	 *
	 *  @return 0: �ɹ�������: ʧ�� 
	 */
	u4 CALL_TYPE EsTlvNewOneNode(IN ESHANDLE hTlv, IN const u1* pu1Tlv, IN u4 u4TlvLen, OUT BlkTlvTree** ppblkTlvTree);
	
	/**
	 *  @brief ����һ��TLV�ڵ㡣������Ƕ�׻����ֵܽڵ�
	 *
	 *  @param[in] hTlv				TLV�ľ��
	 *  @param[in] u4Tag			u4Tag
	 *  @param[in] u4ValueLen		value len
	 *  @param[in] pvValue			value����
	 *  @param[out] ppblkTlvTree	�ǿա������´�����TLV�ڵ�
	 *
	 *  @return 0: �ɹ�������: ʧ�� 
	 */
	u4 CALL_TYPE EsTlvNewOneNode2(IN ESHANDLE hTlv, IN u4 u4Tag, IN u4 u4ValueLen, IN const void* pvValue, OUT BlkTlvTree** ppblkTlvTree);
	
	/**
	 *  @brief �ͷ�TLV���ڴ�
	 *
	 *  @param[in,out] ppblkTlvTree TLV���ĵ�ַ
	 *
	 *  @return 0: �ɹ�������: ʧ�� 
	 */
	u4 CALL_TYPE EsTlvTreeFree(INOUT BlkTlvTree** ppblkTlvTree);
	
	/**
	 *  @brief ����TLV���ݣ���ת��ΪTLV�����ᴦ���ӽڵ���ֵܽڵ㣩
	 *
	 *  @param[in] hTlv				TLV�ľ��
	 *  @param[in] pu1Tlv			TLV����
	 *  @param[in] u4TlvLen			TLV���ݳ���
	 *  @param[out] ppblkTlvTree	�ǿա������´�����TLV�ڵ�
	 *
	 *  @return 0: �ɹ�������: ʧ�� 
	 */
	u4 CALL_TYPE EsTlvTreeDeflat(IN ESHANDLE hTlv, IN const u1* pu1Tlv, IN u4 u4TlvLen, OUT BlkTlvTree** ppblkTlvTree);
	
	/**
	 *  @brief չ��TLV����
	 *
	 *  @param[in] hTlv				TLV�ľ��
	 *  @param[in] pblkTlvTree		TLV����ַ
	 *  @param[in] pu1Tlv			չ�����TLV���ݡ�֧�ִ���NULL�󳤶�
	 *  @param[in,out] pu4TlvLen		�ǿա�����չ�����TLV����ռ�õ��ڴ��ֽ���
	 *
	 *  @return 0: �ɹ�������: ʧ�� 
	 */
	u4 CALL_TYPE EsTlvTreeFlat(IN ESHANDLE hTlv, IN const BlkTlvTree* pblkTlvTree, OUT u1* pu1Tlv, INOUT u4* pu4TlvLen);
	
	/**
	 *  @brief ��ָ���Ľڵ�����ӽڵ�
	 *
	 *  @param[in] hTlv					TLV�ľ��
	 *  @param[in,out] pblkTlvTreeParent	���ڵ�
	 *  @param[in,out] pblkTlvTreeChild	�ӽڵ�
	 *
	 *  @return 0: �ɹ�������: ʧ�� 
	 *
	 *  @details [ע��]!!! 
	 *           1. ���ڵ�ֱ���������ӽڵ㣬�ӽڵ��ѷ�����ڴ�ᱻ�ӹܣ�����Ҫ�����ͷ�
	 *           2. ��ӳɹ��󣬻���¸��ڵ�����и��ڵ�ĳ�����Ϣ
	 */
	u4 CALL_TYPE EsTlvTreeAddChild(IN ESHANDLE hTlv, INOUT BlkTlvTree* pblkTlvTreeParent, INOUT BlkTlvTree* pblkTlvTreeChild);
	
	/**
	 *  @brief ����ֵܽڵ�
	 *
	 *  @param[in] hTlv						TLV�ľ��
	 *  @param[in,out] pblkTlvTreeRoot		���ڵ�
	 *  @param[in,out] pblkTlvTreeSibling	�ֽڵ�
	 *
	 *  @return 0: �ɹ�������: ʧ�� 
	 *
	 *  @details [ע��]!!! 
	 *           1. ���ڵ��������ֵܽڵ㣬�ֵܽڵ��ѷ�����ڴ�ᱻ�ӹܣ�����Ҫ�����ͷ�
	 *           2. ��ӳɹ��󣬻���¸��ڵ�����и��ڵ�ĳ�����Ϣ
	 */
	u4 CALL_TYPE EsTlvTreeAddSibling(IN ESHANDLE hTlv, INOUT BlkTlvTree* pblkTlvTreeRoot, INOUT BlkTlvTree* pblkTlvTreeSibling);
	
	/**
	 *  @brief ͨ��tag����TLV�ڵ�
	 *
	 *  @param[in] pblkTlvTree	TLV�ڵ�
	 *  @param[in] u4Tag		tag
	 *
	 *  @return ���ҵ���TLV�ڵ�ָ�� 
	 */
	const BlkTlvTree* EsTlvFindViaTag(IN const BlkTlvTree* pblkTlvTree, IN u4 u4Tag);
	
	/**
	 *  @brief �Ƚ�����TLV���Ƿ����
	 *
	 *  @param[in] pblkTlvTree1	TLV��1
	 *  @param[in] pblkTlvTree2	TLV��2 
	 *
	 *  @return ESFALSE: �����
	 */
	ESBOOL CALL_TYPE EsTlvTreeCompare(IN const BlkTlvTree* pblkTlvTree1, IN const BlkTlvTree* pblkTlvTree2);

#ifdef __cplusplus
};
#endif

#endif	// !define _ESTLV_H_
