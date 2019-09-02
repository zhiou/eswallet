//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2013,�������Ķ������ݰ�ȫ���޹�˾
// All rights reserved.
// 
// �ļ����ƣ�
//		EsXml.h
//
// �ļ���ʶ��
//
// ժ   Ҫ��
//		XML�ļ������ӿڡ����ӿ�ֻ�ṩ������Ľ������������ṩ����ת������չ���ȸ߼����ܡ�
//		���屾�ӿڵ��������ڣ���XML�������ܽ�����һ����󣬴Ӷ�ʹ��ʹ����������ı���ӿڵľ���ʵ�ַ�ʽ��
//		����ʵ�ַ�ʽ���������·�ʽ��ʹ�õ������⣬ʹ��ƽ̨SDK����������ȡ�
//		���ǵ�ѡ����ʹ�õ�������Դ�⡣
//		ע�⣺
//		1�����ӿ����ṩ�ĺ����������ַ��������ת������
//		2�����ӿڵ�ĳЩ��������������Ƕ����ַ���ָ�룬�������߲���Ҫ����ָ����ڴ�����ͷŲ�����
//		�����ڡ�EsXmlFree��������֮��������ص��ڴ涼�������ͷš�
// 
// ��ǰ�汾��
//		1.0
//
// ��   �ߣ�
//		������
//
// ������ڣ�
//		2013��06��28��
//////////////////////////////////////////////////////////////////////////

#ifndef _ES_XML_H_
#define _ES_XML_H_

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct BlkXmlAttributeTag
	{
		const char* m_pszName;
		const char* m_pszValue;
	}BlkXmlAttribute;
	
	/**
	 *  @brief ���ļ��м���XML��
	 * 
	 *  @param[in] pszXmlDoc XML�ļ�·����
	 *  @param[out] phXmlNode XML����ľ��
	 * 
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlLoadFromFile(IN const char* pszXmlDoc, OUT ESHANDLE* phXmlNode);

	/**
	 *  @brief ���ڴ��м���XML��
	 * 
	 *  @param[in] pu1Data �ڴ����ݡ�
	 *  @param[out] u4Len ���ݳ���
	 *  @param[out] phXmlNode XML����ľ��
	 * 
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlLoadFromMemory(IN const u1* pu1Data, IN u4 u4Len, OUT ESHANDLE* phXmlNode);

	/**
	 *  @brief ���XML������ַ������롣
	 *  
	 *  @param[in] hXmlNode				XML����ľ����
	 *  @param[out] penCharSet			ָ�룬ָ�������ַ�������Ӧ��Ӧ�ı��룩
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetEncoding(IN ESHANDLE hXmlNode, OUT EnCharSet* penCharSet);

	/**
	 *  @brief ������Դ���ڵ��á�EsXmlLoadFromFile������EsXmlLoadFromMemory���ɹ�֮�����XML�����Ѿ�������ʹ�ã�
	 * 		   ������ô˽ӿ��ͷ���Ӧ���ڴ档
	 *  
	 *  @param[in] hXmlNode XML����ľ��
	 * 
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlFree(IN const ESHANDLE hXmlNode);

	/**
	 *  @brief ���ָ�����Ƶ��ӽڵ㣬���ж��ͬ���ڵ㣬�����ص�һ���ڵ㡣
	 * 
	 *  @param[in] hXmlNode XML����ľ��
	 *  @param[out] pszChildName �ӽڵ������
	 *  @param[out] phXmlChildNode �ӽڵ�ľ��ָ�룬ָ���ȡ�����ӽڵ�ľ��
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetSingleChild(IN ESHANDLE hXmlNode, IN const char* pszChildName, OUT ESHANDLE* phXmlChildNode);

	/**
	 *  @brief ����ӽڵ��б�
	 * 
	 *  @param[in] hXmlNode XML����ľ��
	 *  @param[out] phXmlNodeChildrenList �ӽڵ���б�����ָ��
	 *  @param[in,out] pu4ChildrenNum �ӽڵ����Ŀ
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetChildren(IN ESHANDLE hXmlNode, OUT ESHANDLE* phXmlNodeChildrenList, IN OUT u4* pu4ChildrenNum);

	/**
	 *  @brief ��ýڵ�����ơ�
	 * 
	 *  @param[in] hXmlNode XML����ľ��
	 *  @param[out] ppszName �ڵ�����ƣ�����ָ�룬�����������ͷŶ�Ӧ�ڴ棬�����ڵ���ExXmlFree֮�󣬲�����ʹ��
	 *
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetClassName(IN ESHANDLE hXmlNode, OUT const char** ppszName);

	/**
	 *  @brief ��ýڵ���ı���������������һ���ڵ㣺��<nodeName>Node Text</nodeName>����
	 * 		   ��Դ˽ڵ���á�EsXmlGetText����*ppszNameָ����ڴ�Ϊ�ַ�����Node Text����
	 * 
	 *  @param[in] hXmlNode XML����ľ��
	 *  @param[out] ppszName �ڵ���ı���Ϣ������ָ�룬�����������ͷŶ�Ӧ�ڴ棬�����ڵ���ExXmlFree֮�󣬲�����ʹ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetText(IN ESHANDLE hXmlNode, OUT const char** ppszName);

	/**
	 *  @brief ��������б�
	 * 
	 *  @param[in] hXmlNode XML����ľ��
	 *  @param[out] pblkAttrList �����б������ͷ���Ӧ�ڴ棬�����ڵ���ExXmlFree֮�󣬲�����ʹ��
	 * 	@param[in,out]pu4AttrCount ���Ե���Ŀ��������ʱ��ָʾ�������Ĵ�С������Ӧ�Ľṹ��ƣ��������ʱ����ʾ���Ե�ʵ����Ŀ��
	 * 
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetAttributes(IN ESHANDLE hXmlNode, OUT BlkXmlAttribute* pblkAttrList, INOUT u4* pu4AttrCount);

	/**
	 *  @brief �������ֵ��
	 * 
	 *  @param[in] hXmlNode XML����ľ��
	 *  @param[in] pszName ���Ե�����
	 *  @param[out] ppszValue ���Ե�ֵ������ָ�룬�����ͷ���Ӧ���ڴ棬�����ڵ���ExXmlFree֮�󣬲�����ʹ��
	 * 
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetAttribute(IN ESHANDLE hXmlNode, IN const char* pszName, OUT const char** ppszValue);

	/**
	 *  @brief ����xml����ʱ�Ƿ�ѹ���ո�Ӧ�ü���xmlǰ����
	 *  
	 *  @param[in] bCondense ����xmlʱ�Ƿ�ѹ���ո�(������/dll������Ч)
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlSetCondenseWhiteSpace(IN ESBOOL bCondense);

	/**
	 *  @brief ��ȡxml����ʱ�Ƿ�ѹ���ո�
	 *  
	 *  @param[out] pbCondense ����xmlʱ�Ƿ�ѹ���ո�
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetCondenseWhiteSpace(IN ESBOOL* pbCondense);

	/**
	 *  @brief ����xml�ĵ��ṹ
	 *  
	 *  @param[in] enCharSet �ĵ�����
	 *  @param[out] phXml xml�ĵ����
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlCreate(IN EnCharSet enCharSet, OUT ESHANDLE* phXml);

	/**
	 *  @brief ����һ���ӽڵ�
	 *  
	 *  @param[in] hXml ��ǰ�ڵ�
	 *  @param[in] pszClassName �ڵ�tag
	 *  @param[in] pszValue �ڵ�value
	 *  @param[out] phChild �����ӽڵ�ľ��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlAddChild(IN ESHANDLE hXml, IN const char* pszClassName, IN const char* pszValue, OUT ESHANDLE* phChild);

	/**
	 *  @brief ���½ڵ��ֵ
	 *  
	 *  @param[in] hXml ��ǰ�ڵ�
	 *  @param[in] pszText �ڵ�ֵ
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlSetText(IN ESHANDLE hXml, IN const char* pszText);

	/**
	 *  @brief ��������ֵ���������Ѵ��ڣ���������ֵ��������������
	 *  
	 *  @param[in] hXml ��ǰ�ڵ�
	 *  @param[in] pszAttrName ��������
	 *  @param[in] pszAttrValue ����ֵ
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlSetAttribute(IN ESHANDLE hXml, IN const char* pszAttrName, IN const char* pszAttrValue);

	/**
	 *  @brief ��xml�ĵ����浽�ļ�
	 *  
	 *  @param[in]hXml xml���
	 *  @param[in]pszPath �ļ�·��
	 *  
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlWriteToFile(IN ESHANDLE hXml, IN const char* pszPath);

	/**
	 *  @brief ��xml�ĵ����浽ָ���Ļ�����
	 * 
	 *  @param[in] hXml xml���
	 *  @param[out] pu1Data ������
	 *  @param[in,out] pu4Len ����������/���ݳ���
	 * 
	 *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlWriteToMemory(IN ESHANDLE hXml, OUT u1* pu1Data, INOUT u4* pu4Len);

#ifdef __cplusplus
}
#endif

#endif // _ES_XML_H_
