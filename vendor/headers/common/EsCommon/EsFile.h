#ifndef _ESFILE_H_
#define _ESFILE_H_

#ifdef __cplusplus
extern "C"
{
#endif
	//////////////////////////////////////////////////////////////////////////
	// 【重要】zhouxiao added at 20180612 
	// 1. 以下接口如果有A(ANSI)和W(UNICODE)两个版本，只有Windows会提供W的版本
	// 2. 部分A的接口仅提供Windows版本，其他平台需要时可添加实现
	//////////////////////////////////////////////////////////////////////////

#ifdef _OS_WINDOWS_
    /**
     *  @brief 文件搜索-初始化
     *
	 *  @param[in] pszFolder 所在文件夹，最大MAX_PATH
	 *  @param[in] pszFileName 要搜索的文件名，最大MAX_PATH
	 *  @param[out] phFileSearch 句柄
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileSearchInitA(IN const char* pszFolder, IN const char* pszFileName, OUT ESHANDLE* phFileSearch);
	u4 CALL_TYPE EsFileSearchInitW(IN const wchar_t* pszFolder, IN const wchar_t* pszFileName, OUT ESHANDLE* phFileSearch);

    /**
     *  @brief 文件搜索-初次搜索
     *
	 *  @param[in] hFileSearch 搜索句柄
	 *
	 *  @return 搜索到的文件名
     *
     *  @details none
     */
	const char* CALL_TYPE EsFileSearchFirstA(IN ESHANDLE hFileSearch);
	const wchar_t* CALL_TYPE EsFileSearchFirstW(IN ESHANDLE hFileSearch);

    /**
     *  @brief 文件搜索-继续搜索
     *
	 *  @param[in] hFileSearch 搜索句柄
     *
     *  @return 搜索到的文件名
     *
     *  @details none
     */
	const char* CALL_TYPE EsFileSearchNextA(IN ESHANDLE hFileSearch);
	const wchar_t* CALL_TYPE EsFileSearchNextW(IN ESHANDLE hFileSearch);
	
    /**
     *  @brief 文件搜索-释放
     *
	 *  @param[in,out] phFileSearch 搜索句柄
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileSearchReleaseA(IN ESHANDLE* phFileSearch);
	u4 CALL_TYPE EsFileSearchReleaseW(IN ESHANDLE* phFileSearch);

#endif // defined _OS_WINDOWS_

    /**
     *  @brief 打开文件
     *
	 *  @param[in,out] ppFile 指向文件句柄指针的指针
	 *  @param[in] pszFileName 文件名(相对或绝对路径)
	 *  @param[in] pszMode 文件打开模式，详见fopen
     *
     *  @return 0-成功 other-失败，详见EsError.h或同fopen返回码
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileOpenA(OUT FILE** ppFile, IN const char* pszFileName, IN const char* pszMode);
	u4 CALL_TYPE EsFileOpenW(OUT FILE** ppFile, IN const wchar_t* pszFileName, IN const wchar_t* pszMode);

    /**
     *  @brief 以二进制方式读取文件全部内容
     *
	 *  @param[in] pszFileName 文件名(相对或绝对路径)
	 *  @param[in,out] ppvFileData 读取到的数据，内部分配内存
	 *  @param[out] pu4FileDataLen 读取到的文件数据大小
     *
     *  @return 0-成功 other-失败，详见EsError.h或同fopen返回码
     *
     *  @details 以二进制方式打开，注意，如果文件比较大，慎用此接口
     */
	u4 CALL_TYPE EsFileReadA(IN const char* pszFileName, OUT void** ppvFileData, OUT u4* pu4FileDataLen);
	u4 CALL_TYPE EsFileReadW(IN const wchar_t* pszFileName, OUT void** ppvFileData, OUT u4* pu4FileDataLen);

    /**
     *  @brief 以二进制方式写入文件(不存在则创建)
     *
	 *  @param[in] pszFileName 文件名(相对或绝对路径)
	 *  @param[in] pvFileData 待写入的数据
	 *  @param[in] u4FileDataLen 待写入的数据大小
	 *  @param[in] bAppend ESTRUE-内容附加到末尾 否则覆盖
     *
     *  @return 0-成功 other-失败，详见EsError.h或同fopen返回码
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileWriteA(IN const char* pszFileName, IN const void* pvFileData, IN u4 u4FileDataLen, IN ESBOOL bAppend);
	u4 CALL_TYPE EsFileWriteW(IN const wchar_t* pszFileName, IN const void* pvFileData, IN u4 u4FileDataLen, IN ESBOOL bAppend);

    /**
     *  @brief 读取指定偏移位置的文件内存(以二进制方式)
     *
	 *  @param[in] pszFileName 文件名(相对或绝对路径)
	 *  @param[in] u4Offset 写入的文件偏移位置
	 *  @param[out] pvData 读取到的数据
	 *  @param[in] u4DataLen 待读取数据大小
     *
     *  @return 0-成功 other-失败，详见EsError.h或同fopen返回码
     *
     *  @details 指定的文件必须存在
     */
	u4 CALL_TYPE EsFileReadPartA(IN const char* pszFileName, IN u4 u4Offset, OUT void* pvData, IN u4 u4DataLen);
	u4 CALL_TYPE EsFileReadPartW(IN const wchar_t* pszFileName, IN u4 u4Offset, IN void* pvData, IN u4 u4DataLen);

	/**
	 *  @brief 写入指定偏移位置的文件内存(以二进制方式)
     *
	 *  @param[in] pszFileName 文件名(相对或绝对路径)
	 *  @param[in] u4Offset 写入的文件偏移位置
	 *  @param[in] pvData 待写入数据
	 *  @param[in] u4DataLen 写入的数据大小
     *
     *  @return 0-成功 other-失败，详见EsError.h或同fopen返回码
     *
     *  @details 指定的文件必须存在
     */
	u4 CALL_TYPE EsFileWritePartA(IN const char* pszFileName, IN u4 u4Offset, IN const void* pvData, IN u4 u4DataLen);
	u4 CALL_TYPE EsFileWritePartW(IN const wchar_t* pszFileName, IN u4 u4Offset, IN const void* pvData, IN u4 u4DataLen);

	/**
	 *  @brief 判断文件是否存在
     *
	 *  @param[in] pszFileName 文件名(相对或绝对路径)
     *
     *  @return ESFALSE-不存在 other-存在
     *
     *  @details none
     */
	ESBOOL CALL_TYPE EsFileIsExistA(IN const char *pszFileName);
	ESBOOL CALL_TYPE EsFileIsExistW(IN const wchar_t *pszFileName);

	/**
	 *  @brief 拷贝文件到新位置
	 *
	 *  @param[in] pszFileOrg 源文件名(相对或绝对路径)
	 *  @param[in] pszFileDst 目标文件名(相对或绝对路径)
	 *  @param[in] bCanExist ESFALSE-目标存在则报错，否则覆盖
	 *
	 *  @return 0-成功 other-失败，详见EsError.h或同fopen返回码
     *
     *  @details none
     */
#if 0 // 这两个接口与EsSetupBase冲突了，暂时屏蔽，有了解决方案再说
	u4 CALL_TYPE EsFileCopyA(IN const char* pszFileOrg, IN const char* pszFileDst, IN ESBOOL bCanExist);
	u4 CALL_TYPE EsFileCopyW(IN const wchar_t* pszFileOrg, IN const wchar_t* pszFileDst, IN ESBOOL bCanExist);
#endif 
	/**
	 *  @brief 创建文件/文件夹
	 *
	 *  @param[in] pszFileName 文件名(相对或绝对路径)
	 *  @param[in] bFolder ESFALSE-文件，否则文件夹
	 *
	 *  @return 0-成功 other-失败，详见EsError.h或同fopen返回码
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileCreateA(IN const char* pszFileName, IN ESBOOL bFolder);
	u4 CALL_TYPE EsFileCreateW(IN const wchar_t* pszFileName, IN ESBOOL bFolder);

    /**
     *  @brief 关闭文件
     *
	 *  @param[in,out] ppFile 指向文件句柄指针的指针
     *
     *  @return 0-成功 other-失败，详见EsError.h或同fopen返回码
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileClose(INOUT FILE** ppFile);

#if defined(UNICODE)|| defined(_UNICODE)
#	ifndef _OS_WINDOWS_
#		pragma error "UNICODE or _UNICODE shoule be used at _OS_WINDOWS_!!"
#	endif

#	define	EsFileSearchInit	EsFileSearchInitW
#	define	EsFileSearchFirst	EsFileSearchFirstW
#	define	EsFileSearchNext	EsFileSearchNextW
#	define	EsFileSearchRelease	EsFileSearchReleaseW
#	define	EsFileOpen			EsFileOpenW
#	define	EsFileRead			EsFileReadW
#	define	EsFileWrite			EsFileWriteW
#	define	EsFileReadPart		EsFileReadPartW
#	define	EsFileWritePart		EsFileWritePartW
#	define	EsFileIsExist		EsFileIsExistW
#	define	EsFileCopy			EsFileCopyW
#	define	EsFileCreate		EsFileCreateW
#else
#	define	EsFileSearchInit	EsFileSearchInitA
#	define	EsFileSearchFirst	EsFileSearchFirstA
#	define	EsFileSearchNext	EsFileSearchNextA
#	define	EsFileSearchRelease	EsFileSearchReleaseA
#	define	EsFileOpen			EsFileOpenA
#	define	EsFileRead			EsFileReadA
#	define	EsFileWrite			EsFileWriteA
#	define	EsFileReadPart		EsFileReadPartA
#	define	EsFileWritePart		EsFileWritePartA
#	define	EsFileIsExist		EsFileIsExistA
#	define	EsFileCopy			EsFileCopyA
#	define	EsFileCreate		EsFileCreateA
#endif

#ifdef __cplusplus
}
#endif
#endif
