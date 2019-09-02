#ifndef _ES_AOTOMIC_OPERATION_H_
#define _ES_AOTOMIC_OPERATION_H_

//////////////////////////////////////////////////////////////////////////
// ԭ�Ӳ���
//////////////////////////////////////////////////////////////////////////

_BEGIN_C_DECL_

/**
 *  @brief ԭ�Ӳ���:32λ���ͼ�
 *  
 *  @param[in,out] pu4Value ������/�������
 *  @param[in] u4Count ����
 *  
 *  @return *pu4Valueִ�мӷ�����ǰ�ĳ�ʼֵ�����pu4Value=NULL������0
 *
 *  @details �����߿���ͨ���Ƚ�*pu4Value�ĳ�ʼֵ�Լ�����������ֵ���жϳɹ����
 */
u4 CALL_TYPE EsAtomicAdd32(INOUT volatile u4* pu4Value, IN u4 u4Count);
#define FAA_UINT32(pu4Value, u4Count) EsAtomicAdd32((volatile u4*)pu4Value, (u4)u4Count)

/**
 *  @brief ԭ�Ӳ���:32λ���ͼ�
 *  
 *  @param[in,out] pu4Value ������/�������
 *  @param[in] u4Count ����
 *  
 *  @return *pu4Valueִ�м�������ǰ�ĳ�ʼֵ�����pu4Value=NULL������0
 *
 *  @details �����߿���ͨ���Ƚ�*pu4Value�ĳ�ʼֵ�Լ�����������ֵ���жϳɹ����
 */
u4 CALL_TYPE EsAtomicSub32(INOUT volatile u4* pu4Value, IN u4 u4Count);
#define FAS_UINT32(pu4Value, u4Count) EsAtomicSub32((volatile u4*)pu4Value, (u4)u4Count)

/**
 *  @brief ԭ�Ӳ���:32λ���� �����ǰ��ֵ��ָ����ֵ���ͽ���Ϊ��ֵ-compare and swap
 *  
 *  @param[in,out] pu4Value ��������ֵ/�������
 *  @param[in] u4Old ��ֵ
 *  @param[in] u4New ��ֵ
 *  
 *  @return ESFALSE-ʧ��,����-�ɹ�
 *
 *  @details ���*pu4Value=u4Old����*pu4Value=u4New������ɶ��û��
 */
ESBOOL CALL_TYPE EsAtomicCAS32(INOUT volatile u4* pu4Value, IN u4 u4Old, IN u4 u4New);
#define CAS_UINT32(pu4Value, u4Old, u4New) EsAtomicCAS32((volatile u4*)pu4Value, (u4)u4Old, (u4)u4New)

/**
 *  @brief ԭ�Ӳ���:32λ���� �����ǰ��ֵ��ָ����ֵ���ͽ���Ϊ��ֵ-compare and swap
 *  
 *  @param[in,out] pu4Value ��������ֵ/�������
 *  @param[in] u4Old ��ֵ
 *  @param[in] u4New ��ֵ
 *  
 *  @return *pu4Value����ǰ�ĳ�ʼֵ�����pu4Value=NULL������0
 *
 *  @details ���*pu4Value=u4Old����*pu4Value=u4New������ɶ��û��
 *			 �����߿���ͨ���Ƚ�*pu4Value�ĳ�ʼֵ�Լ�����������ֵ���жϳɹ����
 */
u4 CALL_TYPE EsAtomicCASVal32(INOUT volatile u4* pu4Value, IN u4 u4Old, IN u4 u4New);
#define CAS_VAL_UINT32(pu4Value, u4Old, u4New) EsAtomicCASVal32((volatile u4*)pu4Value, (u4)u4Old, (u4)u4New)
	
_END_C_DECL_

#endif // !defined _ES_AOTOMIC_OPERATION_H_
