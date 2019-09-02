#ifndef _COMMON_LOG_H_
#define _COMMON_LOG_H_

#include "../EsTypeDef.h"
#include <wchar.h>

#ifdef __cplusplus
extern "C"
{
#endif
	#define LOG_TYPE_INFORMATION	1
	#define LOG_TYPE_WARNING		2
	#define LOG_TYPE_ERROR			3
	#define FILE_AND_LINE __FILE__, __LINE__
	// ʹ��ʾ���� ES_LOG((LOG_TYPE_ERROR, FILE_AND_LINE, "My Name is %s!", "Simon")); // ע������������С���ţ�
	// ���������������DebugPrint�Ķ��壬����Ȥ�Ŀ��Բο�
#ifdef _LOG_
	#define ES_LOG(x) EsLogEx x
	#define ES_LOG_W(x) EsLogExW x
	#define ES_LOG_BYTE_ARRAY(x) EsLogByteArrayEx x
	// ��չ��־��
	// funcΪ��������(vc6��֧��__FUNCTION_)
	// XXX enter
	#define ES_LOG_START(func)	do{\
										ES_LOG((LOG_TYPE_INFORMATION, FILE_AND_LINE, "%s enter", (const char*)(func)));\
									}while ( 0x00 )
	// XXX enter, aa = 0x%08X
	#define ES_LOG_START_U4(func, u4Value)	do{\
											ES_LOG((LOG_TYPE_INFORMATION, FILE_AND_LINE, "%s enter, %s = 0x%08X", (char*)(func), #u4Value, (u4)(u4Value)));\
										}while ( 0x00 )
	// XXX enter, aa = 0x%016llX
	#define ES_LOG_START_U64(func, u64Value)	do{\
												ES_LOG((LOG_TYPE_INFORMATION, FILE_AND_LINE, "%s enter, %s = 0x%016llX", (const char*)(func), #u64Value, (u64t)(u64Value)));\
											}while ( 0x00 )
	// XXX leave, aa = 0x%08X
	#define ES_LOG_END(func)	do{\
									u4 u4LogType = (ERROR_NO_ERROR!=(u4)u4Result)?LOG_TYPE_ERROR:LOG_TYPE_INFORMATION; \
									ES_LOG((u4LogType, FILE_AND_LINE, "%s leave, ret = 0x%08X", (const char*)(func), (u4)u4Result));\
								}while (0x00)
#else
	#define ES_LOG(x) 
	#define ES_LOG_W(x) 
	#define ES_LOG_BYTE_ARRAY(x) 
	// ��չ��־��
	#define ES_LOG_START(func)
	#define ES_LOG_START_U4(func, u4Value)
	#define ES_LOG_START_U64(func, u64Value)	
	#define ES_LOG_END(func)
#endif

	extern void EsLogEx(u4 u4LogType, const char* szFile, u4 u4Line, const char *szMsgFormat, ...);
	extern void EsLogExW(u4 u4LogType, const char* szFile, u4 u4Line, const wchar_t *szMsgFormat, ...);
	extern void EsLogByteArrayEx(u4 u4LogType, const char* szFile, u4 u4Line, const u1* pu1Data, u4 u4DataLen);
#ifdef __cplusplus
}
#endif	
#endif