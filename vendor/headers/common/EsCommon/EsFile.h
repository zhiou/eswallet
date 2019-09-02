#ifndef _ESFILE_H_
#define _ESFILE_H_

#ifdef __cplusplus
extern "C"
{
#endif
	//////////////////////////////////////////////////////////////////////////
	// ����Ҫ��zhouxiao added at 20180612 
	// 1. ���½ӿ������A(ANSI)��W(UNICODE)�����汾��ֻ��Windows���ṩW�İ汾
	// 2. ����A�Ľӿڽ��ṩWindows�汾������ƽ̨��Ҫʱ�����ʵ��
	//////////////////////////////////////////////////////////////////////////

#ifdef _OS_WINDOWS_
    /**
     *  @brief �ļ�����-��ʼ��
     *
	 *  @param[in] pszFolder �����ļ��У����MAX_PATH
	 *  @param[in] pszFileName Ҫ�������ļ��������MAX_PATH
	 *  @param[out] phFileSearch ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileSearchInitA(IN const char* pszFolder, IN const char* pszFileName, OUT ESHANDLE* phFileSearch);
	u4 CALL_TYPE EsFileSearchInitW(IN const wchar_t* pszFolder, IN const wchar_t* pszFileName, OUT ESHANDLE* phFileSearch);

    /**
     *  @brief �ļ�����-��������
     *
	 *  @param[in] hFileSearch �������
	 *
	 *  @return ���������ļ���
     *
     *  @details none
     */
	const char* CALL_TYPE EsFileSearchFirstA(IN ESHANDLE hFileSearch);
	const wchar_t* CALL_TYPE EsFileSearchFirstW(IN ESHANDLE hFileSearch);

    /**
     *  @brief �ļ�����-��������
     *
	 *  @param[in] hFileSearch �������
     *
     *  @return ���������ļ���
     *
     *  @details none
     */
	const char* CALL_TYPE EsFileSearchNextA(IN ESHANDLE hFileSearch);
	const wchar_t* CALL_TYPE EsFileSearchNextW(IN ESHANDLE hFileSearch);
	
    /**
     *  @brief �ļ�����-�ͷ�
     *
	 *  @param[in,out] phFileSearch �������
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileSearchReleaseA(IN ESHANDLE* phFileSearch);
	u4 CALL_TYPE EsFileSearchReleaseW(IN ESHANDLE* phFileSearch);

#endif // defined _OS_WINDOWS_

    /**
     *  @brief ���ļ�
     *
	 *  @param[in,out] ppFile ָ���ļ����ָ���ָ��
	 *  @param[in] pszFileName �ļ���(��Ի����·��)
	 *  @param[in] pszMode �ļ���ģʽ�����fopen
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��ͬfopen������
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileOpenA(OUT FILE** ppFile, IN const char* pszFileName, IN const char* pszMode);
	u4 CALL_TYPE EsFileOpenW(OUT FILE** ppFile, IN const wchar_t* pszFileName, IN const wchar_t* pszMode);

    /**
     *  @brief �Զ����Ʒ�ʽ��ȡ�ļ�ȫ������
     *
	 *  @param[in] pszFileName �ļ���(��Ի����·��)
	 *  @param[in,out] ppvFileData ��ȡ�������ݣ��ڲ������ڴ�
	 *  @param[out] pu4FileDataLen ��ȡ�����ļ����ݴ�С
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��ͬfopen������
     *
     *  @details �Զ����Ʒ�ʽ�򿪣�ע�⣬����ļ��Ƚϴ����ô˽ӿ�
     */
	u4 CALL_TYPE EsFileReadA(IN const char* pszFileName, OUT void** ppvFileData, OUT u4* pu4FileDataLen);
	u4 CALL_TYPE EsFileReadW(IN const wchar_t* pszFileName, OUT void** ppvFileData, OUT u4* pu4FileDataLen);

    /**
     *  @brief �Զ����Ʒ�ʽд���ļ�(�������򴴽�)
     *
	 *  @param[in] pszFileName �ļ���(��Ի����·��)
	 *  @param[in] pvFileData ��д�������
	 *  @param[in] u4FileDataLen ��д������ݴ�С
	 *  @param[in] bAppend ESTRUE-���ݸ��ӵ�ĩβ ���򸲸�
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��ͬfopen������
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileWriteA(IN const char* pszFileName, IN const void* pvFileData, IN u4 u4FileDataLen, IN ESBOOL bAppend);
	u4 CALL_TYPE EsFileWriteW(IN const wchar_t* pszFileName, IN const void* pvFileData, IN u4 u4FileDataLen, IN ESBOOL bAppend);

    /**
     *  @brief ��ȡָ��ƫ��λ�õ��ļ��ڴ�(�Զ����Ʒ�ʽ)
     *
	 *  @param[in] pszFileName �ļ���(��Ի����·��)
	 *  @param[in] u4Offset д����ļ�ƫ��λ��
	 *  @param[out] pvData ��ȡ��������
	 *  @param[in] u4DataLen ����ȡ���ݴ�С
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��ͬfopen������
     *
     *  @details ָ�����ļ��������
     */
	u4 CALL_TYPE EsFileReadPartA(IN const char* pszFileName, IN u4 u4Offset, OUT void* pvData, IN u4 u4DataLen);
	u4 CALL_TYPE EsFileReadPartW(IN const wchar_t* pszFileName, IN u4 u4Offset, IN void* pvData, IN u4 u4DataLen);

	/**
	 *  @brief д��ָ��ƫ��λ�õ��ļ��ڴ�(�Զ����Ʒ�ʽ)
     *
	 *  @param[in] pszFileName �ļ���(��Ի����·��)
	 *  @param[in] u4Offset д����ļ�ƫ��λ��
	 *  @param[in] pvData ��д������
	 *  @param[in] u4DataLen д������ݴ�С
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��ͬfopen������
     *
     *  @details ָ�����ļ��������
     */
	u4 CALL_TYPE EsFileWritePartA(IN const char* pszFileName, IN u4 u4Offset, IN const void* pvData, IN u4 u4DataLen);
	u4 CALL_TYPE EsFileWritePartW(IN const wchar_t* pszFileName, IN u4 u4Offset, IN const void* pvData, IN u4 u4DataLen);

	/**
	 *  @brief �ж��ļ��Ƿ����
     *
	 *  @param[in] pszFileName �ļ���(��Ի����·��)
     *
     *  @return ESFALSE-������ other-����
     *
     *  @details none
     */
	ESBOOL CALL_TYPE EsFileIsExistA(IN const char *pszFileName);
	ESBOOL CALL_TYPE EsFileIsExistW(IN const wchar_t *pszFileName);

	/**
	 *  @brief �����ļ�����λ��
	 *
	 *  @param[in] pszFileOrg Դ�ļ���(��Ի����·��)
	 *  @param[in] pszFileDst Ŀ���ļ���(��Ի����·��)
	 *  @param[in] bCanExist ESFALSE-Ŀ������򱨴����򸲸�
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��ͬfopen������
     *
     *  @details none
     */
#if 0 // �������ӿ���EsSetupBase��ͻ�ˣ���ʱ���Σ����˽��������˵
	u4 CALL_TYPE EsFileCopyA(IN const char* pszFileOrg, IN const char* pszFileDst, IN ESBOOL bCanExist);
	u4 CALL_TYPE EsFileCopyW(IN const wchar_t* pszFileOrg, IN const wchar_t* pszFileDst, IN ESBOOL bCanExist);
#endif 
	/**
	 *  @brief �����ļ�/�ļ���
	 *
	 *  @param[in] pszFileName �ļ���(��Ի����·��)
	 *  @param[in] bFolder ESFALSE-�ļ��������ļ���
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��ͬfopen������
     *
     *  @details none
     */
	u4 CALL_TYPE EsFileCreateA(IN const char* pszFileName, IN ESBOOL bFolder);
	u4 CALL_TYPE EsFileCreateW(IN const wchar_t* pszFileName, IN ESBOOL bFolder);

    /**
     *  @brief �ر��ļ�
     *
	 *  @param[in,out] ppFile ָ���ļ����ָ���ָ��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h��ͬfopen������
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
