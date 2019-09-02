#ifndef _ES_AOTOMIC_OPERATION_H_
#define _ES_AOTOMIC_OPERATION_H_

//////////////////////////////////////////////////////////////////////////
// 原子操作
//////////////////////////////////////////////////////////////////////////

_BEGIN_C_DECL_

/**
 *  @brief 原子操作:32位整型加
 *  
 *  @param[in,out] pu4Value 被加数/操作结果
 *  @param[in] u4Count 加数
 *  
 *  @return *pu4Value执行加法运算前的初始值，如果pu4Value=NULL，返回0
 *
 *  @details 调用者可以通过比较*pu4Value的初始值以及运算结束后的值来判断成功与否
 */
u4 CALL_TYPE EsAtomicAdd32(INOUT volatile u4* pu4Value, IN u4 u4Count);
#define FAA_UINT32(pu4Value, u4Count) EsAtomicAdd32((volatile u4*)pu4Value, (u4)u4Count)

/**
 *  @brief 原子操作:32位整型减
 *  
 *  @param[in,out] pu4Value 被减数/操作结果
 *  @param[in] u4Count 加数
 *  
 *  @return *pu4Value执行减法运算前的初始值，如果pu4Value=NULL，返回0
 *
 *  @details 调用者可以通过比较*pu4Value的初始值以及运算结束后的值来判断成功与否
 */
u4 CALL_TYPE EsAtomicSub32(INOUT volatile u4* pu4Value, IN u4 u4Count);
#define FAS_UINT32(pu4Value, u4Count) EsAtomicSub32((volatile u4*)pu4Value, (u4)u4Count)

/**
 *  @brief 原子操作:32位整型 如果当前的值是指定的值，就交换为新值-compare and swap
 *  
 *  @param[in,out] pu4Value 被操作的值/操作结果
 *  @param[in] u4Old 旧值
 *  @param[in] u4New 新值
 *  
 *  @return ESFALSE-失败,其他-成功
 *
 *  @details 如果*pu4Value=u4Old，则*pu4Value=u4New，否则啥都没干
 */
ESBOOL CALL_TYPE EsAtomicCAS32(INOUT volatile u4* pu4Value, IN u4 u4Old, IN u4 u4New);
#define CAS_UINT32(pu4Value, u4Old, u4New) EsAtomicCAS32((volatile u4*)pu4Value, (u4)u4Old, (u4)u4New)

/**
 *  @brief 原子操作:32位整型 如果当前的值是指定的值，就交换为新值-compare and swap
 *  
 *  @param[in,out] pu4Value 被操作的值/操作结果
 *  @param[in] u4Old 旧值
 *  @param[in] u4New 新值
 *  
 *  @return *pu4Value运算前的初始值，如果pu4Value=NULL，返回0
 *
 *  @details 如果*pu4Value=u4Old，则*pu4Value=u4New，否则啥都没干
 *			 调用者可以通过比较*pu4Value的初始值以及运算结束后的值来判断成功与否
 */
u4 CALL_TYPE EsAtomicCASVal32(INOUT volatile u4* pu4Value, IN u4 u4Old, IN u4 u4New);
#define CAS_VAL_UINT32(pu4Value, u4Old, u4New) EsAtomicCASVal32((volatile u4*)pu4Value, (u4)u4Old, (u4)u4New)
	
_END_C_DECL_

#endif // !defined _ES_AOTOMIC_OPERATION_H_
