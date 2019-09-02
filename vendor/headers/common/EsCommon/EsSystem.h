#ifndef _ES_SYSTEM_H_
#define _ES_SYSTEM_H_

#ifdef __cplusplus
extern "C" 
{
#endif

	#define LANG_ID_SIPMLE_CHINESE			2052
	#define LANG_ID_TRADITIONAL_CHINESE		1028
	#define LANG_ID_ENGLISH_US				1033

	#define ESPRODCUT_PARAM_LANG            0x01
	#define ESPRODCUT_PARAM_VIRTUAL_DESKTOP	0x02

#if defined(_OS_WINDOWS_)
	#pragma pack(push, 1)
	typedef struct BlkProductParamTag
	{
		int nLangId;
		u4 u4UseVirtualDesktop;
		unsigned char pucReserved[512-sizeof(int)-4];
	}BlkProductParam;
	#pragma pack(pop)

	/**
     *  @brief 设置产品配置
	 *
	 *  @param[in] pszProductName 产品名称/ID，不同项目唯一
	 *  @param[in] u4Param 配置类型，详见ESPRODCUT_PARAM_XXX
	 *  @param[in] pvParamData 配置数据
	 *  @param[in] u4ParamDataLen 配置数据长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsSetProductParam(IN const char* pszProductName, IN u4 u4Param, IN const void* pvParamData, IN u4 u4ParamDataLen);
	
	/**
     *  @brief 设置产品配置
	 *
	 *  @param[in] pszProductName 产品名称/ID，不同项目唯一
	 *  @param[in] u4Param 配置类型，详见ESPRODCUT_PARAM_XXX
	 *  @param[out] pvParamData 配置数据
	 *  @param[in,out] pu4ParamDataLen 配置数据长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsGetProductParam(IN const char* pszProductName, IN u4 u4Param, OUT void* pvParamData, INOUT u4* pu4ParamDataLen);

	#define OS_WIN95	0x0095
	#define OS_WIN98	0x0098
	#define OS_WINNT	0x0104
	#define OS_WIN2K	0x0120
	#define OS_WINXP	0x0121
	#define OS_WIN2K3	0x0123
	#define OS_VISTA	0x0225
	#define OS_WIN2K8	0x0228
	#define OS_WIN7		0x0229
	#define OS_WIN8		0x0408
	#define OS_WIN81	0x0603
	#define OS_WIN10	0x1000
	#define OS_UNKNOWN	0xFFFFFFFF

	/**
     *  @brief 获取操作系统版本
     *
     *  @return OS_XXXX
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsGetVersion(void);

	/**
     *  @brief 获取文件版本(exe/dll等文件属性带版本的文件)
	 *
	 *  @param[in] szFileName 文件路径
	 *  @param[out] szFileVersion 文件版本，支持传入NULL求长度
	 *  @param[in,out] pu4VersionLen 文件版本缓冲区/长度
	 *
	 *  @return 0-成功 other-失败，详见EsError.h/WinError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsGetFileVersion(IN const char* szFileName, OUT char* szFileVersion, INOUT u4* pu4VersionLen);

	/**
     *  @brief 禁用/恢复重定向(仅64位系统有效)
	 *
	 *  @param[in] bDisable ESTRUE-禁用;Other-恢复
	 *
	 *  @return none
     *
     *  @details none
     */
	void CALL_TYPE EsOsDisOrRevRedirection(IN ESBOOL bDisable);

#endif // defined _OS_WINDOWS_

	/**
     *  @brief 检查系统是否为64位
	 *
	 *  @return ESTRUE/ESFALSE
     *
     *  @details 获取失败时返回ESFALSE
     */
	ESBOOL CALL_TYPE EsOsIs64Bit();

#if defined(_OS_WINDOWS_) || defined(_OS_MAC_OSX_) || defined(_OS_LINUX_)
	#	define LANGE_ZH_CN				0x00
	#	define LANGE_ENGLISH			0x01
	#	define LANGE_ZH_TW				0x02
	#	define LANGE_FRANCE				0x03
	#	define LANGE_RUSSIAN			0x04
	#	define LANGE_JAPAN				0x05
	#	define LANGE_INDONESIA			0x06
	#	define LANGE_GERMAN				0x07
	#	define LANGE_ARAB				0x08
	#	define LANGE_KOREA				0x09
	#	define LANGE_KAZAK				0x0A
	#	define LANGE_THAI				0x0B

	/**
     *  @brief 获取系统语言
	 *
	 *  @param[out] pu4LangId 语言Id，详见LANGE_XXX
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsGetLanguage(OUT u4* pu4LangId);

#endif // WINDOWS OR MACOSX

	/**
     *  @brief 睡眠
	 *
	 *  @param[in] u4MiliSeconds 睡眠时间（ms）
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsSleep(IN u4 u4MiliSeconds);

	/**
     *  @brief 获取tickcount
	 *
	 *  @return 系统开机后的时间（ms）
     *
     *  @details 与系统绝对时间无关，返回值为u4，有溢出风险溢出时间0xFFFFFFFF毫秒≈49.7103天
     */
    u4 CALL_TYPE EsOsGetTickCount(); // ms
    
	/**
     *  @brief 获取tickcount(64位)
	 *
	 *  @return 系统开机后的时间（ms）
     *
     *  @details 与系统绝对时间无关
     */
    u64t CALL_TYPE EsOsGetTickCount64(); // ms
    
	/**
     *  @brief 获取当前进程Id
	 *
	 *  @return 进程Id
     *
     *  @details none
     */
    u4 CALL_TYPE EsOsGetCurrentPid();
#	define EsOsGetPid EsOsGetCurrentPid
    
	/**
     *  @brief 获取当前线程Id
	 *
	 *  @return 线程Id
     *
     *  @details none
     */
    u64t CALL_TYPE EsOsGetCurrentTid();
#	define EsOsGetTid EsOsGetCurrentTid
    
	/**
     *  @brief 获取当前线程最后一次的错误码
	 *
	 *  @return 错误码
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsGetLastError();

#ifdef __cplusplus
};
#endif

#endif
