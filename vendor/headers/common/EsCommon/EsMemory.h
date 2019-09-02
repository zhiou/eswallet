#ifndef _ES_MEMORY_H_
#define _ES_MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif

	// memory operation
	void CALL_TYPE EsGenUuid(char* pszUuid, u4 u4UuidLen);
	void CALL_TYPE EsGetRand(void* pvRand, u4 u4RandLen);
	ESBOOL CALL_TYPE EsGetVidPidFromName(const char* pszDevName, u4* pu4VidPid);

	u4 CALL_TYPE EsMemAlloc(void** ppvMem, const void* pvData, u4 u4MemLen);
	void CALL_TYPE EsMemFree(void** ppvMem);
	u4 CALL_TYPE EsMemRealloc(void** ppvData, u4* pu4MaxLen, u4 u4AllocLen);
#	define MemAlloc(pMem,pData,len)				EsMemAlloc(((void**)(pMem)), pData, len)
#	define MemFree(pMem)						EsMemFree(((void**)(pMem)))
#	define MemRealloc(pMem,pMaxLen,len)			EsMemRealloc(((void**)(pMem)), pMaxLen, len)

	u4 CALL_TYPE EsMemCmpValue(const void* pvData, u1 u1Value, u4 u4DataLen);
	
	void CALL_TYPE EsMemRev(void* pvData, u4 u4DataLen);
	void CALL_TYPE EsMemXor(const void* pvInput, void* pvOutput, u4 u4DataLen);
	void CALL_TYPE EsMemNeg(void* pvData, u4 u4DataLen);
	void CALL_TYPE EsMemSafeClear(void* pvData, u4 u4DataLen);
	void CALL_TYPE EsMemSet(void * pvData, int nValue, size_t DataLen);
	u4 CALL_TYPE EsMemCpy(void* pvDest, size_t cchDest, const void* pvSrc, size_t cchCount);

	u4 CALL_TYPE EsGetReturnData(const void *pvRealData, u4 u4RealDataLen, void *pvOutData, u4* pu4OutDataLen);
	u4 CALL_TYPE EsGetReturnString(const char* szOrgString, u4 u4OrgStringLen, char* szReturnString, u4* pu4ReturnStringLen);
	u4 CALL_TYPE EsGetReturnCount(const void* pvOrgElement, u4 u4OrgElementLen, void* pvReturnElement, u4* pu4ReturnElementCount, u4 u4ElementSize);

	void CALL_TYPE EsStringReplaceChar(const char* szOrgName, char* szDstName);
	u4 CALL_TYPE EsAsc2Hex(const char* szAsc, void* pvHex);
	u4 CALL_TYPE EsHex2Asc(const void* pvHex, u4 u4HexLen, char* szAsc);
	u4 CALL_TYPE EsHex2AscNoZeroLeading(const void* pvHex, u4 u4HexLen, char* szAsc);

	u4 CALL_TYPE EsAscToHexA(const char* pszAsc, u4 u4AscLen, u1* pu1Hex, u4* pu4HexLen);
	u4 CALL_TYPE EsHexToAscA(const u1* pu1Hex, u4 u4HexLen, char* pszAsc, u4* pu4AscLen);
	char* CALL_TYPE EsStrTrimLeftA(const char* pszStr);
	
	/**
	 *  @brief 在指定字符串中查找子串(找到子串中任何一个字符就行)
	 *  
	 *  @param[in] pszStr 被查找的字符串
	 *  @param[in] pszFind 子串
	 *  
     *  @return pszStr的子串，比如pszStr="123456" pszFind="38962"，则返回3456
     *
     *  @details none
	 */
	char* CALL_TYPE EsStrFindOneOfCharA(const char* pszStr, const char* pszFind);

	u4 CALL_TYPE EsStrGetOneLineLengthA(const char* pszScript);

	ESBOOL CALL_TYPE EsIsHexrStringA(const char* pszAsc, u4 u4AscLen);

	// 宽字节的接口暂时限定为Windows平台
#ifdef _OS_WINDOWS_
	u4 CALL_TYPE EsAscToHexW(const wchar_t* pwszAsc, u4 u4AscLen, u1* pu1Hex, u4* pu4HexLen);
	u4 CALL_TYPE EsHexToAscW(const u1* pu1Hex, u4 u4HexLen, wchar_t* pwszAsc, u4* pu4AscLen);
	wchar_t* CALL_TYPE EsStrTrimLeftW(const wchar_t* pwszStr);
	wchar_t* CALL_TYPE EsStrFindOneOfCharW(const wchar_t* pwszStr, const wchar_t* pwszFind);
	ESBOOL CALL_TYPE EsIsHexStringW(const wchar_t* pwszAsc, u4 u4AscLen);
	u4 CALL_TYPE EsStrGetOneLineLengthW(const wchar_t* pwszScript);
#endif

#if defined(_UNICODE)||defined(UNICODE)
#	ifndef _OS_WINDOWS_
#		pragma error "UNICODE or _UNICODE shoule be used at _OS_WINDOWS_!!"
#	endif

#	define	EsAscToHex(ptszAsc, u4AscLen, pu1Hex, pu4HexLen)	EsAscToHexW(ptszAsc, u4AscLen, pu1Hex, pu4HexLen)
#	define	EsHexToAsc(pu1Hex, u4HexLen, ptszAsc, pu4AscLen)	EsHexToAscW(pu1Hex, u4HexLen, ptszAsc, pu4AscLen)
#	define	EsStrTrimLeft(pStr)									EsStrTrimLeftW(pStr)
#	define	EsStrFindOneOfChar(pStr, pFind)						EsStrFindOneOfCharW(pStr, pFind)
#	define	EsStrGetOneLineLength(pStr)							EsStrGetOneLineLengthW(pStr)
#	define	EsAscCheck(pAsc, u4AscLen)							EsAscCheckW(pAsc, u4AscLen)
#else
#	define	EsAscToHex(ptszAsc, u4AscLen, pu1Hex, pu4HexLen)	EsAscToHexA(ptszAsc, u4AscLen, pu1Hex, pu4HexLen)
#	define	EsHexToAsc(pu1Hex, u4HexLen, ptszAsc, pu4AscLen)	EsHexToAscA(pu1Hex, u4HexLen, ptszAsc, pu4AscLen)
#	define	EsStrTrimLeft(pStr)									EsStrTrimLeftA(pStr)
#	define	EsStrFindOneOfChar(pStr, pFind)						EsStrFindOneOfCharA(pStr, pFind)
#	define	EsStrGetOneLineLength(pStr)							EsStrGetOneLineLengthA(pStr)
#	define	EsAscCheck(pAsc, u4AscLen)							EsAscCheckA(pAsc, u4AscLen)
#endif

	typedef struct BlkIdNameInfoTag
	{
		u4			m_u4Id;
		char		m_szName[0x20];
	}BlkIdNameInfo;
	char* CALL_TYPE EsFindNameViaId(u4 u4Id, const BlkIdNameInfo* pblkIdNameInfo);
	u4 CALL_TYPE EsFindIdViaName(const char* szName, const BlkIdNameInfo* pblkIdNameInfo);
	typedef struct BlkIdValueTag
	{
		u4			m_u4Id;
		u4			m_u4Value;
	}BlkIdValue;
	u4 CALL_TYPE EsFindValueViaId(u4 u4Id, const BlkIdValue* pblkIdValue);
	u4 CALL_TYPE EsFindIdViaValue(u4 u4Value, const BlkIdValue* pblkIdValue);

#ifdef _OS_WINDOWS_
	u4 CALL_TYPE EsChar2WChar(const char* pszString, wchar_t* pszwString, u4* pu4WStringLen);
	u4 CALL_TYPE EsWChar2Char(const wchar_t* pszwString, char* pszString, u4* pu4StringLen);
	u4 CALL_TYPE EsChar2Char(const char* pszStringS, char* pszStringD, u4* pu4StringDLen);
#if defined(_UNICODE)||defined(UNICODE)
#	define	EsChar2TChar(pszStr, ptszStr, pu4TStrLen)	EsChar2WChar(pszStr, ptszStr, pu4TStrLen)
#	define	EsTChar2Char(ptszStr, pszStr, pu4StrLen)	EsWChar2Char(ptszStr, pszStr, pu4StrLen)
#else
#	define	EsChar2TChar(pszStr, ptszStr, pu4TStrLen)	EsChar2Char(pszStr, ptszStr, pu4TStrLen)
#	define	EsTChar2Char(ptszStr, pszStr, pu4StrLen)	EsChar2Char(ptszStr, pszStr, pu4StrLen)
#endif

#endif // defined _OS_WINDOWS_

	u4 CALL_TYPE EsGetCurrentPath(char* pszFilePath);

#ifdef __cplusplus
};
#endif

#endif
