#ifndef _ES_BIG_NUMBER_H_
#define _ES_BIG_NUMBER_H_

#ifdef __cplusplus
extern "C" 
{
#endif

	//////////////////////////////////////////////////////////////////////////
	// 大数运算（没有特别说明的情况下，字节流都是大头序）
	//////////////////////////////////////////////////////////////////////////

	/**  
	 *  @brief 大数运算:大数加一个数功能 R=A+n
     *
     *  @param[in] pu1A 被加数A
     *  @param[in] u4ALen 被加数A的长度
	 *  @param[in] u4N 加数
	 *  @param[out] pu1Carry 是否发生进位。0-无进位，1-有进位
	 *  @param[out] pu1R 计算结果，长度=MAX(u4ALen, 4)
	 *  
	 *  @return 0-成功，其他-错误，详见EsError.h
	 * 
	 * @note 若加法结果长度大于加数和被加数的最大长度，则认为进位，否则无进位。输出结果不包括进位的内容
	 */
	u4 CALL_TYPE EsBnAddNum(IN const u1* pu1A, IN u4 u4ALen, IN u4 u4N, OUT u1* pu1Carry, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:大数减一个数功能 R=A-n
     *
     *  @param[in] pu1A 被减数A
     *  @param[in] u4ALen 被减数A的长度
	 *  @param[in] u4N 减数
	 *  @param[out] pu1Borrow 0-无借位，1-有借位(不管借了几位返回值都是1)
	 *  @param[out] pu1R 计算结果，长度=MAX(u4ALen, 4)
	 *  
	 *  @return 0-成功，其他-错误，详见EsError.h
	 *  
	 *  @note 若pu1A<N，则认为借位
	 */
	u4 CALL_TYPE EsBnSubNum(IN const u1* pu1A, IN u4 u4ALen, IN u4 u4N, OUT u1* pu1Borrow, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:乘功能 R=A*n
	 *
     *  @param[in] pu1A 被乘数A
     *  @param[in] u4ALen 被乘数A的长度
	 *  @param[in] u4N 乘数N
	 *  @param[out] pu1R 计算结果，长度为u4ALen+4
     *
     *  @return 0-成功，其他-错误，详见EsError.h
	 */
	u4 CALL_TYPE EsBnMulNum(IN const u1* pu1A, IN u4 u4ALen, IN u4 u4N, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:除功能 A=R*n+r
     *
     *  @param[in] pu1A 被除数A
     *  @param[in] u4ALen 被除数A的长度
	 *  @param[in] u4N 除数N
	 *  @param[out] pu1C 计算结果-商，长度为u4ALen
	 *  @param[out] pu1R 计算结果-余数，长度为4
	 *
	 *  @return 0-成功，其他-错误，详见EsError.h
	 */
	u4 CALL_TYPE EsBnDivNum(IN const u1* pu1A, IN u4 u4ALen, IN u4 u4N, OUT u1* pu1C, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:大数比较功能
     *
     *  @param[in] pu1A 数字A
     *  @param[in] u4ALen 数字A的长度
     *  @param[in] pu1B 数字B
     *  @param[in] u4BLen 数字B的长度
	 *  
	 *  @return 0：A==B 1：A>B -1：A<B
	 *  
	 *  @details 失败时返回0
	 */
	int CALL_TYPE EsBnCmp(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen);

	/**
	 *  @brief 大数运算:大数加功能 R=A+B
     *
     *  @param[in] pu1A 被加数A
     *  @param[in] u4ALen 被加数A的长度
     *  @param[in] pu1B 加数B
	 *  @param[in] u4BLen 加数B的长度
	 *  @param[out] pu1Carry 是否发生进位。0-无进位，1-有进位
	 *  @param[out] pu1R 计算结果，长度=MAX(u4ALen, u4BLen)
	 *  
	 *  @return 0-成功，其他-错误，详见EsError.h
	 * 
	 * @note 若加法结果长度大于加数和被加数的最大长度，则认为进位，否则无进位。输出结果不包括进位的内容
	 */
	u4 CALL_TYPE EsBnAdd(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1Carry, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:大数减功能 R=A-B
     *
     *  @param[in] pu1A 被减数A
     *  @param[in] u4ALen 被减数A的长度
     *  @param[in] pu1B 减数B
	 *  @param[in] u4BLen 减数B的长度
	 *  @param[out] pu1Borrow 0-无借位，1-有借位
	 *  @param[out] pu1R 计算结果，长度=MAX(u4ALen, u4BLen)
	 *  
	 *  @return 0-成功，其他-错误，详见EsError.h
	 *  
	 *  @note 若pu1A<pu1B，则认为借位
	 */
	u4 CALL_TYPE EsBnSub(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1Borrow, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:乘功能 R=A*B
     *
     *  @param[in] pu1A 乘数A
     *  @param[in] u4ALen 乘数A的长度
     *  @param[in] pu1B 乘数B
     *  @param[in] u4BLen 乘数B的长度
     *  @param[out] pu1R 计算结果，长度为u4ALen+u4BLen
     *
     *  @return 0-成功，其他-错误，详见EsError.h
	 */
	u4 CALL_TYPE EsBnMul(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1R);

	/**
	 *  @brief 除功能 A/B=C...R
     *
     *  @param[in] pu1A 被除数A
     *  @param[in] u4ALen 被除数A的长度
     *  @param[in] pu1B 除数B
	 *  @param[in] u4BLen 除数B的长度
	 *  @param[out] pu1C 计算结果-商，长度为u4ALen
	 *  @param[out] pu1R 计算结果-余数，长度为u4BLen
	 *
	 *  @return 0-成功，其他-错误，详见EsError.h
	 *  
	 *  @note 除数不允许为0
	 */
	u4 CALL_TYPE EsBnDiv(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1C, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:模功能 R=A mod N
     *
     *  @param[in] pu1A 大数A
     *  @param[in] u4ALen 大数A的长度
     *  @param[in] pu1N 模数N
     *  @param[in] u4NLen 模数N的长度
	 *  @param[out] pu1R 计算结果，与N的长度相同
	 *  
	 *  @return 0-成功，其他-错误，详见EsError.h
	 */
	u4 CALL_TYPE EsBnMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:模加功能 R=(A+B) mod N
     *
     *  @param[in] pu1A 大数A
     *  @param[in] u4ALen 大数A的长度
     *  @param[in] pu1B 大数B
     *  @param[in] u4BLen 大数B的长度
     *  @param[in] pu1N 模数N
     *  @param[in] u4NLen 模数N的长度
	 *  @param[out] pu1R 计算结果，与N的长度相同
	 *  
	 *  @return 0-成功，其他-错误，详见EsError.h
	 */
	u4 CALL_TYPE EsBnAddMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:模减功能 R=(A-B) mod N
     *
     *  @param[in] pu1A 大数A
     *  @param[in] u4ALen 大数A的长度
     *  @param[in] pu1B 大数B
     *  @param[in] u4BLen 大数B的长度
     *  @param[in] pu1N 模数N
     *  @param[in] u4NLen 模数N的长度
	 *  @param[out] pu1R 计算结果，与N的长度相同
	 *  
	 *  @return 0-成功，其他-错误，详见EsError.h
	 */
	u4 CALL_TYPE EsBnSubMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:模乘功能 R=(A*B) mod N
     *
     *  @param[in] pu1A 大数A
     *  @param[in] u4ALen 大数A的长度
     *  @param[in] pu1B 大数B
     *  @param[in] u4BLen 大数B的长度
     *  @param[in] pu1N 模数N
     *  @param[in] u4NLen 模数N的长度
	 *  @param[out] pu1R 计算结果，与N的长度相同
	 *  
	 *  @return 0-成功，其他-错误，详见EsError.h
	 */
	u4 CALL_TYPE EsBnMulMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:模幂功能 R=M^E mod N
     *
     *  @param[in] pu1M 底数M
     *  @param[in] u4MLen 底数M的长度
     *  @param[in] pu1E 指数E
     *  @param[in] u4ELen 指数E的长度
     *  @param[in] pu1N 模数N
     *  @param[in] u4NLen 模数N的长度
	 *  @param[out] pu1R 计算结果，与N的长度相同
	 *  
	 *  @return 0-成功，其他-错误，详见EsError.h
	 */
	u4 CALL_TYPE EsBnExpMod(IN const u1* pu1M, IN u4 u4MLen, IN const u1* pu1E, IN u4 u4ELen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:R = A^-1 mod N
     *
     *  @param[in] pu1A 底数A
     *  @param[in] u4ALen 底数A的长度
     *  @param[in] pu1N 模数N
     *  @param[in] u4NLen 模数N的长度
	 *  @param[out] pu1R 计算结果，与N的长度相同
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 */
	u4 CALL_TYPE EsBnInvMod(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1N, IN u4 u4NLen, OUT u1* pu1R);

	/**
	 *  @brief 大数运算:大数是否为0
	 *
	 *  @param[in] pu1A 大数
	 *  @param[in] u4ALen 大数长度
	 *  
	 *  @return ESFALSE-为0 其他-不为0
	 *  
	 *  @details pu1A=NULL时返回ESTRUE
	 */
	ESBOOL CALL_TYPE EsBnIsZero(IN const u1* pu1A, IN u4 u4ALen);

	/**
	 *  @brief 大数运算:异或 C=A^B
     *
     *  @param[in] pu1A 大数A
     *  @param[in] u4ALen 大数A长度
     *  @param[in] pu1B 大数B
     *  @param[in] u4BLen 大数B长度
	 *  @param[out] pu1C 结果，长度=MAX(u4ALen,u4BLen)
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 */
	u4 CALL_TYPE EsBnXor(IN const u1* pu1A, IN u4 u4ALen, IN const u1* pu1B, IN u4 u4BLen, OUT u1* pu1C);

#ifdef __cplusplus
};
#endif

#endif // !defined _ES_BIG_NUMBER_H_
