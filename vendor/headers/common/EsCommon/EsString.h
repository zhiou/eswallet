#ifndef _ES_STRING_H_
#define _ES_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

	// Ϊ�ַ�������һ�ֵ���Լ����CALL_TYPE��__stdcall���ǲ��ʺϱ�ε�API����ģ��ַ���API�������ڱ�ε���������Ա������ⶨ�塣
    #ifdef _OS_WINDOWS_
    #	define STR_CALL_TYPE __cdecl
    #else
    #	define STR_CALL_TYPE
    #endif

	/**
	 *  @brief �ַ�������
	 *  
	 *  @param[out] pszDest Ŀ�껺������������ΪNULL
	 *  @param[in] cchDest Ŀ�껺������С(���ַ�����)����ҪΪ�ս��'\0'Ԥ���ռ�
	 *  @param[in] pszSrc Դ��������������ΪNULL
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
	 * 
     *  @details 1. Դ/Ŀ�껺����ΪNULL������Ŀ�껺����С��Դ��������Сʱֱ�ӱ���
	 *           2. Ŀ���ַ��Դ��ַ�ص�ʱ���ӿ���Ϊδ֪
	 */
	ESBOOL STR_CALL_TYPE EsStrCopy(OUT char* pszDest, IN size_t cchDest, IN const char* pszSrc);
	ESBOOL STR_CALL_TYPE EsWcsCopy(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t* pszSrc);

	/**
	 *  @brief �ַ�������(��Դ�ַ������ӵ�Ŀ���ַ���ĩβ)
	 *  
	 *  @param[out] pszDest Ŀ�껺������������ΪNULL
	 *  @param[in] cchDest Ŀ�껺������С(���ַ�����)����ҪΪ�ս��'\0'Ԥ���ռ�
	 *  @param[in] pszSrc Դ��������������ΪNULL
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
	 * 
	 *  @details 1. pszDest/pszSrcΪNULL����cchDest=0��ֱ�ӱ���������ֻ��������㣬�ַ����ᱻ�ض�
	 *           2. Ŀ���ַ��Դ��ַ�ص�ʱ���ӿ���Ϊδ֪
	 */
	ESBOOL STR_CALL_TYPE EsStrCat(OUT char* pszDest, IN size_t cchDest, IN const char* pszSrc);
	ESBOOL STR_CALL_TYPE EsWcsCat(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t* pszSrc);

	/**
	 *  @brief ��ʽ���ַ�����ָ��������
	 *  
	 *  @param[out] pszDest Ŀ�껺������������ΪNULL
	 *  @param[in] cchDest Ŀ�껺������С(���ַ�����)����ҪΪ�ս��'\0'Ԥ���ռ�
	 *  @param[in] pszFormat ��ʽ��������ο�printf
	 *  @param[in] ... �ɱ����
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
	 * 
	 *  @details 1. pszDest/pszSrcΪNULL����cchDest=0��ֱ�ӱ���������ֻ��������㣬�ַ����ᱻ�ض�
	 *           2. Ŀ���ַ��Դ��ַ�ص�ʱ���ӿ���Ϊδ֪
	 */
	ESBOOL STR_CALL_TYPE EsStrPrintf(OUT char* pszDest, IN size_t cchDest, IN const char * pszFormat, ...);
	ESBOOL STR_CALL_TYPE EsWcsPrintf(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t * pszFormat, ...);

	/**
	 *  @brief ��ʽ���ַ�����ָ��������
	 *  
	 *  @param[out] pszDest Ŀ�껺������������ΪNULL
	 *  @param[in] cchDest Ŀ�껺������С(���ַ�����)����ҪΪ�ս��'\0'Ԥ���ռ�
	 *  @param[in] pszFormat ��ʽ��������ο�printf
	 *  @param[in] argList �����б�
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
	 * 
	 *  @details 1. pszDest/pszSrcΪNULL����cchDest=0��ֱ�ӱ���������ֻ��������㣬�ַ����ᱻ�ض�
	 *           2. Ŀ���ַ��Դ��ַ�ص�ʱ���ӿ���Ϊδ֪
	 */
	ESBOOL STR_CALL_TYPE EsStrVPrintf(OUT char* pszDest, IN size_t cchDest, IN const char * pszFormat, IN va_list argList);
	ESBOOL STR_CALL_TYPE EsWcsVPrintf(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t * pszFormat, IN va_list argList);

	/**
	 *  @brief �ַ����Ƚ�
	 *  
	 *  @param[in] str1 �ַ���1
	 *  @param[in] str2 �ַ���2
	 *  
	 *  @return < 0: str1 less than str2 
	 *          = 0: str1 identical to str2 
	 *          > 0: str2 greater than string2
	 * 
     *  @details str1��str2��һ��ΪNULLʱ����0
	 */
	s4 STR_CALL_TYPE EsStrICmp(IN const char* str1, IN const char* str2);
	s4 STR_CALL_TYPE EsWcsICmp(IN const wchar_t* str1, IN const wchar_t* str2);
	
	/**
	 *  @brief �ĳ�Сд
	 *  
	 *  @param[out] str ���޸ĵ��ַ���
	 *  @param[in] cchLen �ַ�����(�������ս��)
	 *  
	 *  @return str��ַ: �ɹ�
	 *          NULL: ʧ��
	 * 
     *  @details none
	 */
	char * STR_CALL_TYPE EsStrLower(OUT char* str, IN size_t cchLen);
	wchar_t * STR_CALL_TYPE EsWcsLower(OUT wchar_t* str, IN size_t cchLen);
    
	/**
	 *  @brief �ĳɴ�д
	 *  
	 *  @param[out] str ���޸ĵ��ַ���
	 *  @param[in] cchLen �ַ�����(�������ս��)
	 *  
	 *  @return str��ַ: �ɹ�
	 *          NULL: ʧ��
	 * 
     *  @details none
	 */
	char * STR_CALL_TYPE EsStrUpper(OUT char* str, IN size_t cchLen);
	wchar_t * STR_CALL_TYPE EsWcsUpper(OUT wchar_t* str, IN size_t cchLen);
    
	/**
	 *  @brief ������ƥ��(������)
	 *  
	 *  @param[in] pszPattern ƥ���ģʽ��
	 *  @param[in] u4PatternLen ģʽ���ĳ���
	 *  @param[in] pszString Ŀ�괮
	 *  @param[in] u4StringLen Ŀ�괮����
	 *  @param[in] bNoCase �Ƿ����ִ�Сд
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
	 *  
	 *  @note ������ʽ����
	 *	     ��1��'*'ƥ���������ַ�
	 *	     ��2��'?'ƥ�䵥���ַ�
	 *	     ��3��'[]'ƥ���б��е��ַ���'[]'�У�'^'��ʾ�ų��б��е��ַ���'-'��ʾ�ַ����䣬'\'��ʾת��
	 * 
     *  @details none
	 */
    ESBOOL CALL_TYPE EsStrMatchLen(IN const char *pszPattern, IN u4 u4PatternLen, IN const char *pszString, IN u4 u4StringLen, IN ESBOOL bNoCase);
	
	/**
	 *  @brief ������ƥ��
	 *  
	 *  @param[in] pszPattern ƥ���ģʽ��
	 *  @param[in] pszString Ŀ�괮
	 *  @param[in] bNoCase �Ƿ����ִ�Сд
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
	 *  
	 *  @note ������ʽ����
	 *	     ��1��'*'ƥ���������ַ�
	 *	     ��2��'?'ƥ�䵥���ַ�
	 *	     ��3��'[]'ƥ���б��е��ַ���'[]'�У�'^'��ʾ�ų��б��е��ַ���'-'��ʾ�ַ����䣬'\'��ʾת��
     *
     *  @details none
	 */
    ESBOOL CALL_TYPE EsStrMatch(IN const char *pszPattern, IN const char *pszString, IN ESBOOL bNoCase);

	/**
	 *  @brief �ڴ濽��
	 *  
	 *  @param[out] pvDest Ŀ���ַ��������ΪNULL
	 *  @param[in] cchDest Ŀ���ַ��С(�ֽ�)��>0
	 *  @param[in] pvSrc Դ��ַ��������ΪNULL
	 *  @param[in] cchCount Դ���ݴ�С(�ֽ�),Ҫ��<=cchDest������0ʱ������������
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
     *
     *  @details Ŀ���ַ��Դ��ַ�ص�ʱ���ӿ���Ϊδ֪
	 */
	ESBOOL STR_CALL_TYPE EsMemCopy(OUT void* pvDest, IN size_t cchDest, IN const void* pvSrc, IN size_t cchCount);
	
	/**
	 *  @brief �ڴ濽��
	 *  
	 *  @param[out] pszDest Ŀ���ַ��������ΪNULL
	 *  @param[in] cchDest Ŀ���ַ��С(�ַ�����)��>0
	 *  @param[in] pszSrc Դ��ַ��������ΪNULL
	 *  @param[in] cchCount Դ���ݴ�С(�ַ�����),Ҫ��<=cchDest������0ʱ������������
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
     *
     *  @details Ŀ���ַ��Դ��ַ�ص�ʱ���ӿ���Ϊδ֪
	 */
	ESBOOL STR_CALL_TYPE EsWMemCopy(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t* pszSrc, IN size_t cchCount);

	/**
	 *  @brief �ڴ濽��-����Դ��Ŀ���ַ�ص�
	 *  
	 *  @param[out] pvDest Ŀ���ַ��������ΪNULL
	 *  @param[in] cchDest Ŀ���ַ��С(�ֽ�)��>0
	 *  @param[in] pvSrc Դ��ַ��������ΪNULL
	 *  @param[in] cchCount Դ���ݴ�С(�ֽ�),Ҫ��<=cchDest������0ʱ������������
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
     *
     *  @details none
	 */
	ESBOOL STR_CALL_TYPE EsMemMove(OUT void* pvDest, IN size_t cchDest, IN const void* pvSrc, IN size_t cchCount);
	
	/**
	 *  @brief �ڴ濽��-����Դ��Ŀ���ַ�ص�
	 *  
	 *  @param[out] pszDest Ŀ���ַ��������ΪNULL
	 *  @param[in] cchDest Ŀ���ַ��С(�ַ�����)��>0
	 *  @param[in] pszSrc Դ��ַ��������ΪNULL
	 *  @param[in] cchCount Դ���ݴ�С(�ַ�����),Ҫ��<=cchDest������0ʱ������������
	 *  
	 *  @return ESTRUE: �ɹ�
	 *          ESFALSE: ʧ��
     *
     *  @details none
	 */
	ESBOOL STR_CALL_TYPE EsWMemMove(OUT wchar_t* pszDest, IN size_t cchDest, IN const wchar_t* pszSrc, IN size_t cchCount);
	
	/**
	 *  @brief ���ո����ķָ������ַ������зֽ�[�̰߳�ȫ]
	 *  
	 *  @param[in,out] pszToken ���ֽ���ַ�������1�ε���ʱ������ΪNULL���������ÿ��Դ���NULL��
	 *  @param[in] pszDelimit �ָ������ϣ���������ַ�������ÿ���ַ����Ƿָ�����������ΪNULL
	 *  @param[out] ppszContext ���pszToken���ָ��ʣ�µ��ַ�����������Ϻ���Ҫ�ͷţ�������ΪNULL
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details pszToken��*ppszContext������ͬʱΪNULL
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
