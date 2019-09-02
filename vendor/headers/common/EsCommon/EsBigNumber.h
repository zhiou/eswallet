#ifndef _ES_BIG_NUMBER_H_
#define _ES_BIG_NUMBER_H_

#ifdef __cplusplus
extern "C" 
{
#endif

	//////////////////////////////////////////////////////////////////////////
	// �������㣨û���ر�˵��������£��ֽ������Ǵ�ͷ��
	//////////////////////////////////////////////////////////////////////////

	/**  
	 *  @brief ��������:������һ�������� R=A+n
     *
     *  @param[in] pu1A ������A
     *  @param[in] u4ALen ������A�ĳ���
	 *  @param[in] u4N ����
	 *  @param[out] pu1Carry �Ƿ�����λ��0-�޽�λ��1-�н�λ
	 *  @param[out] pu1R ������������=MAX(u4ALen, 4)
	 *  
	 *  @return 0-�ɹ�������-�������EsError.h
	 * 
	 * @note ���ӷ�������ȴ��ڼ����ͱ���������󳤶ȣ�����Ϊ��λ�������޽�λ����������������λ������
	 */
	u4 CALL_TYPE EsBnAddNum(IN const u1* pu1A, IN u4 u4ALen, IN u4 u4N, OUT u1* pu1Carry, OUT u1* pu1R);

	/**
	 *  @brief ��������:������һ�������� R=A-n
     *
     *  @param[in] pu1A ������A
     *  @param[in] u4ALen ������A�ĳ���
	 *  @param[in] u4N ����
	 *  @param[out] pu1Borrow 0-�޽�λ��1-�н�λ(���ܽ��˼�λ����ֵ����1)
	 *  @param[out] pu1R ������������=MAX(u4ALen, 4)
	 *  
	 *  @return 0-�ɹ�������-�������EsError.h
	 *  
	 *  @note ��pu1A<N������Ϊ��λ
	 */
	u4 CALL_TYPE EsBnSubNum(IN const u1* pu1A, IN u4 u4ALen, IN u4 u4N, OUT u1* pu1Borrow, OUT u1* pu1R);

	/**
	 *  @brief ��������:�˹��� R=A*n
	 *
     *  @param[in] pu1A ������A
     *  @param[in] u4ALen ������A�ĳ���
	 *  @param[in] u4N ����N
	 *  @param[out] pu1R ������������Ϊu4ALen+4
     *
     *  @return 0-�ɹ�������-�������EsError.h
	 */
	u4 CALL_TYPE EsBnMulNum(IN const u1* pu1A, IN u4 u4ALen, IN u4 u4N, OUT u1* pu1R);

	/**
	 *  @brief ��������:������ A=R*n+r
     *
     *  @param[in] pu1A ������A
     *  @param[in] u4ALen ������A�ĳ���
	 *  @param[in] u4N ����N
	 *  @param[out] pu1C ������-�̣�����Ϊu4ALen
	 *  @param[out] pu1R ������-����������Ϊ4
	 *
	 *  @return 0-�ɹ�������-�������EsError.h
	 */
	u4 CALL_TYPE EsBnDivNum(IN const u1* pu1A, IN u4 u4ALen, IN u4 u4N, OUT u1* pu1C, OUT u1* pu1R);

	/**
	 *  @brief ��������:�����ȽϹ���
     *
     *  @param[in] pu1A ����A
     *  @param[in] u4ALen ����A�ĳ���
     *  @param[in] pu1B ����B
     *  @param[in] u4BLen ����B�ĳ���
	 *  
	 *  @return 0��A==B 1��A>B -1��A<B
	 *  
	 *  @details ʧ��ʱ����0
	 */
	int CALL_TYPE EsBnCmp(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen);

	/**
	 *  @brief ��������:�����ӹ��� R=A+B
     *
     *  @param[in] pu1A ������A
     *  @param[in] u4ALen ������A�ĳ���
     *  @param[in] pu1B ����B
	 *  @param[in] u4BLen ����B�ĳ���
	 *  @param[out] pu1Carry �Ƿ�����λ��0-�޽�λ��1-�н�λ
	 *  @param[out] pu1R ������������=MAX(u4ALen, u4BLen)
	 *  
	 *  @return 0-�ɹ�������-�������EsError.h
	 * 
	 * @note ���ӷ�������ȴ��ڼ����ͱ���������󳤶ȣ�����Ϊ��λ�������޽�λ����������������λ������
	 */
	u4 CALL_TYPE EsBnAdd(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1Carry, OUT u1* pu1R);

	/**
	 *  @brief ��������:���������� R=A-B
     *
     *  @param[in] pu1A ������A
     *  @param[in] u4ALen ������A�ĳ���
     *  @param[in] pu1B ����B
	 *  @param[in] u4BLen ����B�ĳ���
	 *  @param[out] pu1Borrow 0-�޽�λ��1-�н�λ
	 *  @param[out] pu1R ������������=MAX(u4ALen, u4BLen)
	 *  
	 *  @return 0-�ɹ�������-�������EsError.h
	 *  
	 *  @note ��pu1A<pu1B������Ϊ��λ
	 */
	u4 CALL_TYPE EsBnSub(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1Borrow, OUT u1* pu1R);

	/**
	 *  @brief ��������:�˹��� R=A*B
     *
     *  @param[in] pu1A ����A
     *  @param[in] u4ALen ����A�ĳ���
     *  @param[in] pu1B ����B
     *  @param[in] u4BLen ����B�ĳ���
     *  @param[out] pu1R ������������Ϊu4ALen+u4BLen
     *
     *  @return 0-�ɹ�������-�������EsError.h
	 */
	u4 CALL_TYPE EsBnMul(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1R);

	/**
	 *  @brief ������ A/B=C...R
     *
     *  @param[in] pu1A ������A
     *  @param[in] u4ALen ������A�ĳ���
     *  @param[in] pu1B ����B
	 *  @param[in] u4BLen ����B�ĳ���
	 *  @param[out] pu1C ������-�̣�����Ϊu4ALen
	 *  @param[out] pu1R ������-����������Ϊu4BLen
	 *
	 *  @return 0-�ɹ�������-�������EsError.h
	 *  
	 *  @note ����������Ϊ0
	 */
	u4 CALL_TYPE EsBnDiv(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1C, OUT u1* pu1R);

	/**
	 *  @brief ��������:ģ���� R=A mod N
     *
     *  @param[in] pu1A ����A
     *  @param[in] u4ALen ����A�ĳ���
     *  @param[in] pu1N ģ��N
     *  @param[in] u4NLen ģ��N�ĳ���
	 *  @param[out] pu1R ����������N�ĳ�����ͬ
	 *  
	 *  @return 0-�ɹ�������-�������EsError.h
	 */
	u4 CALL_TYPE EsBnMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief ��������:ģ�ӹ��� R=(A+B) mod N
     *
     *  @param[in] pu1A ����A
     *  @param[in] u4ALen ����A�ĳ���
     *  @param[in] pu1B ����B
     *  @param[in] u4BLen ����B�ĳ���
     *  @param[in] pu1N ģ��N
     *  @param[in] u4NLen ģ��N�ĳ���
	 *  @param[out] pu1R ����������N�ĳ�����ͬ
	 *  
	 *  @return 0-�ɹ�������-�������EsError.h
	 */
	u4 CALL_TYPE EsBnAddMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief ��������:ģ������ R=(A-B) mod N
     *
     *  @param[in] pu1A ����A
     *  @param[in] u4ALen ����A�ĳ���
     *  @param[in] pu1B ����B
     *  @param[in] u4BLen ����B�ĳ���
     *  @param[in] pu1N ģ��N
     *  @param[in] u4NLen ģ��N�ĳ���
	 *  @param[out] pu1R ����������N�ĳ�����ͬ
	 *  
	 *  @return 0-�ɹ�������-�������EsError.h
	 */
	u4 CALL_TYPE EsBnSubMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief ��������:ģ�˹��� R=(A*B) mod N
     *
     *  @param[in] pu1A ����A
     *  @param[in] u4ALen ����A�ĳ���
     *  @param[in] pu1B ����B
     *  @param[in] u4BLen ����B�ĳ���
     *  @param[in] pu1N ģ��N
     *  @param[in] u4NLen ģ��N�ĳ���
	 *  @param[out] pu1R ����������N�ĳ�����ͬ
	 *  
	 *  @return 0-�ɹ�������-�������EsError.h
	 */
	u4 CALL_TYPE EsBnMulMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief ��������:ģ�ݹ��� R=M^E mod N
     *
     *  @param[in] pu1M ����M
     *  @param[in] u4MLen ����M�ĳ���
     *  @param[in] pu1E ָ��E
     *  @param[in] u4ELen ָ��E�ĳ���
     *  @param[in] pu1N ģ��N
     *  @param[in] u4NLen ģ��N�ĳ���
	 *  @param[out] pu1R ����������N�ĳ�����ͬ
	 *  
	 *  @return 0-�ɹ�������-�������EsError.h
	 */
	u4 CALL_TYPE EsBnExpMod(IN const u1* pu1M, IN u4 u4MLen, IN const u1* pu1E, IN u4 u4ELen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief ��������:R = A^-1 mod N
     *
     *  @param[in] pu1A ����A
     *  @param[in] u4ALen ����A�ĳ���
     *  @param[in] pu1N ģ��N
     *  @param[in] u4NLen ģ��N�ĳ���
	 *  @param[out] pu1R ����������N�ĳ�����ͬ
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 */
	u4 CALL_TYPE EsBnInvMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief ��������:�����Ƿ�Ϊ0
	 *
	 *  @param[in] pu1A ����
	 *  @param[in] u4ALen ��������
	 *  
	 *  @return ESFALSE-Ϊ0 ����-��Ϊ0
	 *  
	 *  @details pu1A=NULLʱ����ESTRUE
	 */
	ESBOOL CALL_TYPE EsBnIsZero(IN const u1* pu1A, IN u4 u4ALen);

	/**
	 *  @brief ��������:��� C=A^B
     *
     *  @param[in] pu1A ����A
     *  @param[in] u4ALen ����A����
     *  @param[in] pu1B ����B
     *  @param[in] u4BLen ����B����
	 *  @param[out] pu1C ���������=MAX(u4ALen,u4BLen)
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 */
	u4 CALL_TYPE EsBnXor(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1C);

#ifdef __cplusplus
};
#endif

#endif // !defined _ES_BIG_NUMBER_H_
