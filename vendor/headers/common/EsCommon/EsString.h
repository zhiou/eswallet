#ifndef _ES_STRING_H_
#define _ES_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

	// 为字符串定义一种调用约定。CALL_TYPE（__stdcall）是不适合变参的API定义的，字符串API参数存在变参的情况，所以必须另外定义。
    #ifdef _OS_WINDOWS_
    #	define STR_CALL_TYPE __cdecl
    #else
    #	define STR_CALL_TYPE
    #endif

	/**
	 *  @brief 字符串拷贝
	 *  
	 *  @param[out] pszDest 目标缓冲区，不允许为NULL
	 *  @param[in] cchDest 目标缓冲区大小(以字符计数)，需要为终结符'\0'预留空间
	 *  @param[in] pszSrc 源缓冲区，不允许为NULL
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
	 * 
     *  @details 1. 源/目标缓冲区为NULL，或者目标缓冲区小于源缓冲区大小时直接报错
	 *           2. 目标地址和源地址重叠时，接口行为未知
	 */
	ESBOOL STR_CALL_TYPE EsStrCopy(OUT char* pszDest, IN size_t cchDest, IN const char* pszSrc);
	ESBOOL STR_CALL_TYPE EsWcsCopy(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t* pszSrc);

	/**
	 *  @brief 字符串连接(把源字符串附加到目标字符串末尾)
	 *  
	 *  @param[out] pszDest 目标缓冲区，不允许为NULL
	 *  @param[in] cchDest 目标缓冲区大小(以字符计数)，需要为终结符'\0'预留空间
	 *  @param[in] pszSrc 源缓冲区，不允许为NULL
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
	 * 
	 *  @details 1. pszDest/pszSrc为NULL，或cchDest=0，直接报错。如果出现缓冲区不足，字符串会被截断
	 *           2. 目标地址和源地址重叠时，接口行为未知
	 */
	ESBOOL STR_CALL_TYPE EsStrCat(OUT char* pszDest, IN size_t cchDest, IN const char* pszSrc);
	ESBOOL STR_CALL_TYPE EsWcsCat(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t* pszSrc);

	/**
	 *  @brief 格式化字符串到指定缓冲区
	 *  
	 *  @param[out] pszDest 目标缓冲区，不允许为NULL
	 *  @param[in] cchDest 目标缓冲区大小(以字符计数)，需要为终结符'\0'预留空间
	 *  @param[in] pszFormat 格式化串，请参考printf
	 *  @param[in] ... 可变参数
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
	 * 
	 *  @details 1. pszDest/pszSrc为NULL，或cchDest=0，直接报错。如果出现缓冲区不足，字符串会被截断
	 *           2. 目标地址和源地址重叠时，接口行为未知
	 */
	ESBOOL STR_CALL_TYPE EsStrPrintf(OUT char* pszDest, IN size_t cchDest, IN const char * pszFormat, ...);
	ESBOOL STR_CALL_TYPE EsWcsPrintf(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t * pszFormat, ...);

	/**
	 *  @brief 格式化字符串到指定缓冲区
	 *  
	 *  @param[out] pszDest 目标缓冲区，不允许为NULL
	 *  @param[in] cchDest 目标缓冲区大小(以字符计数)，需要为终结符'\0'预留空间
	 *  @param[in] pszFormat 格式化串，请参考printf
	 *  @param[in] argList 参数列表
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
	 * 
	 *  @details 1. pszDest/pszSrc为NULL，或cchDest=0，直接报错。如果出现缓冲区不足，字符串会被截断
	 *           2. 目标地址和源地址重叠时，接口行为未知
	 */
	ESBOOL STR_CALL_TYPE EsStrVPrintf(OUT char* pszDest, IN size_t cchDest, IN const char * pszFormat, IN va_list argList);
	ESBOOL STR_CALL_TYPE EsWcsVPrintf(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t * pszFormat, IN va_list argList);

	/**
	 *  @brief 字符串比较
	 *  
	 *  @param[in] str1 字符串1
	 *  @param[in] str2 字符串2
	 *  
	 *  @return < 0: str1 less than str2 
	 *          = 0: str1 identical to str2 
	 *          > 0: str2 greater than string2
	 * 
     *  @details str1或str2任一个为NULL时返回0
	 */
	s4 STR_CALL_TYPE EsStrICmp(IN const char* str1, IN const char* str2);
	s4 STR_CALL_TYPE EsWcsICmp(IN const wchar_t* str1, IN const wchar_t* str2);
	
	/**
	 *  @brief 改成小写
	 *  
	 *  @param[out] str 待修改的字符串
	 *  @param[in] cchLen 字符个数(不包括终结符)
	 *  
	 *  @return str地址: 成功
	 *          NULL: 失败
	 * 
     *  @details none
	 */
	char * STR_CALL_TYPE EsStrLower(OUT char* str, IN size_t cchLen);
	wchar_t * STR_CALL_TYPE EsWcsLower(OUT wchar_t* str, IN size_t cchLen);
    
	/**
	 *  @brief 改成大写
	 *  
	 *  @param[out] str 待修改的字符串
	 *  @param[in] cchLen 字符个数(不包括终结符)
	 *  
	 *  @return str地址: 成功
	 *          NULL: 失败
	 * 
     *  @details none
	 */
	char * STR_CALL_TYPE EsStrUpper(OUT char* str, IN size_t cchLen);
	wchar_t * STR_CALL_TYPE EsWcsUpper(OUT wchar_t* str, IN size_t cchLen);
    
	/**
	 *  @brief 正则表达匹配(带长度)
	 *  
	 *  @param[in] pszPattern 匹配的模式串
	 *  @param[in] u4PatternLen 模式串的长度
	 *  @param[in] pszString 目标串
	 *  @param[in] u4StringLen 目标串长度
	 *  @param[in] bNoCase 是否区分大小写
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
	 *  
	 *  @note 正则表达式规则：
	 *	     （1）'*'匹配任意多个字符
	 *	     （2）'?'匹配单个字符
	 *	     （3）'[]'匹配列表中的字符，'[]'中，'^'表示排除列表中的字符，'-'表示字符区间，'\'表示转义
	 * 
     *  @details none
	 */
    ESBOOL CALL_TYPE EsStrMatchLen(IN const char *pszPattern, IN u4 u4PatternLen, IN const char *pszString, IN u4 u4StringLen, IN ESBOOL bNoCase);
	
	/**
	 *  @brief 正则表达匹配
	 *  
	 *  @param[in] pszPattern 匹配的模式串
	 *  @param[in] pszString 目标串
	 *  @param[in] bNoCase 是否区分大小写
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
	 *  
	 *  @note 正则表达式规则：
	 *	     （1）'*'匹配任意多个字符
	 *	     （2）'?'匹配单个字符
	 *	     （3）'[]'匹配列表中的字符，'[]'中，'^'表示排除列表中的字符，'-'表示字符区间，'\'表示转义
     *
     *  @details none
	 */
    ESBOOL CALL_TYPE EsStrMatch(IN const char *pszPattern, IN const char *pszString, IN ESBOOL bNoCase);

	/**
	 *  @brief 内存拷贝
	 *  
	 *  @param[out] pvDest 目标地址，不允许为NULL
	 *  @param[in] cchDest 目标地址大小(字节)，>0
	 *  @param[in] pvSrc 源地址，不允许为NULL
	 *  @param[in] cchCount 源数据大小(字节),要求<=cchDest，等于0时不做处理不报错
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
     *
     *  @details 目标地址和源地址重叠时，接口行为未知
	 */
	ESBOOL STR_CALL_TYPE EsMemCopy(OUT void* pvDest, IN size_t cchDest, IN const void* pvSrc, IN size_t cchCount);
	
	/**
	 *  @brief 内存拷贝
	 *  
	 *  @param[out] pszDest 目标地址，不允许为NULL
	 *  @param[in] cchDest 目标地址大小(字符个数)，>0
	 *  @param[in] pszSrc 源地址，不允许为NULL
	 *  @param[in] cchCount 源数据大小(字符个数),要求<=cchDest，等于0时不做处理不报错
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
     *
     *  @details 目标地址和源地址重叠时，接口行为未知
	 */
	ESBOOL STR_CALL_TYPE EsWMemCopy(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t* pszSrc, IN size_t cchCount);

	/**
	 *  @brief 内存拷贝-允许源和目标地址重叠
	 *  
	 *  @param[out] pvDest 目标地址，不允许为NULL
	 *  @param[in] cchDest 目标地址大小(字节)，>0
	 *  @param[in] pvSrc 源地址，不允许为NULL
	 *  @param[in] cchCount 源数据大小(字节),要求<=cchDest，等于0时不做处理不报错
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
     *
     *  @details none
	 */
	ESBOOL STR_CALL_TYPE EsMemMove(OUT void* pvDest, IN size_t cchDest, IN const void* pvSrc, IN size_t cchCount);
	
	/**
	 *  @brief 内存拷贝-允许源和目标地址重叠
	 *  
	 *  @param[out] pszDest 目标地址，不允许为NULL
	 *  @param[in] cchDest 目标地址大小(字符个数)，>0
	 *  @param[in] pszSrc 源地址，不允许为NULL
	 *  @param[in] cchCount 源数据大小(字符个数),要求<=cchDest，等于0时不做处理不报错
	 *  
	 *  @return ESTRUE: 成功
	 *          ESFALSE: 失败
     *
     *  @details none
	 */
	ESBOOL STR_CALL_TYPE EsWMemMove(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t* pszSrc, IN size_t cchCount);
	
	/**
	 *  @brief 按照给定的分隔符把字符串进行分解[线程安全]
	 *  
	 *  @param[in,out] pszToken 待分解的字符串，第1次调用时不允许为NULL，后续调用可以传入NULL。
	 *  @param[in] pszDelimit 分隔符集合，如果传入字符串，则每个字符都是分隔符，不允许为NULL
	 *  @param[out] ppszContext 存放pszToken被分割后剩下的字符串，调用完毕后不需要释放，不允许为NULL
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details pszToken和*ppszContext不允许同时为NULL
	 */
	char * STR_CALL_TYPE EsStrTok(INOUT char* pszToken, IN const char* pszDelimit, OUT char** ppszContext);

	#ifdef UNICODE
	#	define EsTcsCopy EsWcsCopy
	#	define EsTcsCat EsWcsCat
	#	define EsTcsPrintf EsWcsPrintf
	#	define EsTcsVPrintf EsWcsVPrintf
	#	define EsTcsLower EsWcsLower
	#else
	#	define EsTcsCopy EsStrCopy
	#	define EsTcsCat EsStrCat
	#	define EsTcsPrintf EsStrPrintf
	#	define EsTcsVPrintf EsStrVPrintf
	#	define EsTcsLower EsStrLower
	#endif

#ifdef __cplusplus
};
#endif

#endif
