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
     *  @brief ���ò�Ʒ����
	 *
	 *  @param[in] pszProductName ��Ʒ����/ID����ͬ��ĿΨһ
	 *  @param[in] u4Param �������ͣ����ESPRODCUT_PARAM_XXX
	 *  @param[in] pvParamData ��������
	 *  @param[in] u4ParamDataLen �������ݳ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsSetProductParam(IN const char* pszProductName, IN u4 u4Param, IN const void* pvParamData, IN u4 u4ParamDataLen);
	
	/**
     *  @brief ���ò�Ʒ����
	 *
	 *  @param[in] pszProductName ��Ʒ����/ID����ͬ��ĿΨһ
	 *  @param[in] u4Param �������ͣ����ESPRODCUT_PARAM_XXX
	 *  @param[out] pvParamData ��������
	 *  @param[in,out] pu4ParamDataLen �������ݳ���
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
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
     *  @brief ��ȡ����ϵͳ�汾
     *
     *  @return OS_XXXX
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsGetVersion(void);

	/**
     *  @brief ��ȡ�ļ��汾(exe/dll���ļ����Դ��汾���ļ�)
	 *
	 *  @param[in] szFileName �ļ�·��
	 *  @param[out] szFileVersion �ļ��汾��֧�ִ���NULL�󳤶�
	 *  @param[in,out] pu4VersionLen �ļ��汾������/����
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h/WinError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsGetFileVersion(IN const char* szFileName, OUT char* szFileVersion, INOUT u4* pu4VersionLen);

	/**
     *  @brief ����/�ָ��ض���(��64λϵͳ��Ч)
	 *
	 *  @param[in] bDisable ESTRUE-����;Other-�ָ�
	 *
	 *  @return none
     *
     *  @details none
     */
	void CALL_TYPE EsOsDisOrRevRedirection(IN ESBOOL bDisable);

#endif // defined _OS_WINDOWS_

	/**
     *  @brief ���ϵͳ�Ƿ�Ϊ64λ
	 *
	 *  @return ESTRUE/ESFALSE
     *
     *  @details ��ȡʧ��ʱ����ESFALSE
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
     *  @brief ��ȡϵͳ����
	 *
	 *  @param[out] pu4LangId ����Id�����LANGE_XXX
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsGetLanguage(OUT u4* pu4LangId);

#endif // WINDOWS OR MACOSX

	/**
     *  @brief ˯��
	 *
	 *  @param[in] u4MiliSeconds ˯��ʱ�䣨ms��
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsSleep(IN u4 u4MiliSeconds);

	/**
     *  @brief ��ȡtickcount
	 *
	 *  @return ϵͳ�������ʱ�䣨ms��
     *
     *  @details ��ϵͳ����ʱ���޹أ�����ֵΪu4��������������ʱ��0xFFFFFFFF�����49.7103��
     */
    u4 CALL_TYPE EsOsGetTickCount(); // ms
    
	/**
     *  @brief ��ȡtickcount(64λ)
	 *
	 *  @return ϵͳ�������ʱ�䣨ms��
     *
     *  @details ��ϵͳ����ʱ���޹�
     */
    u64t CALL_TYPE EsOsGetTickCount64(); // ms
    
	/**
     *  @brief ��ȡ��ǰ����Id
	 *
	 *  @return ����Id
     *
     *  @details none
     */
    u4 CALL_TYPE EsOsGetCurrentPid();
#	define EsOsGetPid EsOsGetCurrentPid
    
	/**
     *  @brief ��ȡ��ǰ�߳�Id
	 *
	 *  @return �߳�Id
     *
     *  @details none
     */
    u64t CALL_TYPE EsOsGetCurrentTid();
#	define EsOsGetTid EsOsGetCurrentTid
    
	/**
     *  @brief ��ȡ��ǰ�߳����һ�εĴ�����
	 *
	 *  @return ������
     *
     *  @details none
     */
	u4 CALL_TYPE EsOsGetLastError();

#ifdef __cplusplus
};
#endif

#endif
