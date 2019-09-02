//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2013,深圳市文鼎创数据安全有限公司
// All rights reserved.
// 
// 文件名称：
//		EsXml.h
//
// 文件标识：
//
// 摘   要：
//		XML文件解析接口。本接口只提供最基本的解析函数，不提供类型转换、宏展开等高级功能。
//		定义本接口的意义在于，对XML解析功能进行了一层抽象，从而使得使用者无需关心本层接口的具体实现方式。
//		具体实现方式可以有如下方式：使用第三方库，使用平台SDK、自主编码等。
//		我们的选择是使用第三方开源库。
//		注意：
//		1）本接口所提供的函数并不对字符编码进行转换处理。
//		2）本接口的某些函数的输出参数是二级字符串指针，但调用者不需要对其指向的内存进行释放操作，
//		最终在“EsXmlFree”被调用之后，所有相关的内存都将合理释放。
// 
// 当前版本：
//		1.0
//
// 作   者：
//		伍友良
//
// 完成日期：
//		2013年06月28日
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
	 *  @brief 从文件中加载XML。
	 * 
	 *  @param[in] pszXmlDoc XML文件路径。
	 *  @param[out] phXmlNode XML对象的句柄
	 * 
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlLoadFromFile(IN const char* pszXmlDoc, OUT ESHANDLE* phXmlNode);

	/**
	 *  @brief 从内存中加载XML。
	 * 
	 *  @param[in] pu1Data 内存数据。
	 *  @param[out] u4Len 数据长度
	 *  @param[out] phXmlNode XML对象的句柄
	 * 
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlLoadFromMemory(IN const u1* pu1Data, IN u4 u4Len, OUT ESHANDLE* phXmlNode);

	/**
	 *  @brief 获得XML对象的字符集编码。
	 *  
	 *  @param[in] hXmlNode				XML对象的句柄。
	 *  @param[out] penCharSet			指针，指向具体的字符集（对应相应的编码）
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetEncoding(IN ESHANDLE hXmlNode, OUT EnCharSet* penCharSet);

	/**
	 *  @brief 回收资源。在调用“EsXmlLoadFromFile”、“EsXmlLoadFromMemory”成功之后，如果XML数据已经无需再使用，
	 * 		   必须调用此接口释放相应的内存。
	 *  
	 *  @param[in] hXmlNode XML对象的句柄
	 * 
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlFree(IN const ESHANDLE hXmlNode);

	/**
	 *  @brief 获得指定名称的子节点，若有多个同名节点，仅返回第一个节点。
	 * 
	 *  @param[in] hXmlNode XML对象的句柄
	 *  @param[out] pszChildName 子节点的名称
	 *  @param[out] phXmlChildNode 子节点的句柄指针，指向获取到的子节点的句柄
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetSingleChild(IN ESHANDLE hXmlNode, IN const char* pszChildName, OUT ESHANDLE* phXmlChildNode);

	/**
	 *  @brief 获得子节点列表。
	 * 
	 *  @param[in] hXmlNode XML对象的句柄
	 *  @param[out] phXmlNodeChildrenList 子节点的列表，二级指针
	 *  @param[in,out] pu4ChildrenNum 子节点的数目
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetChildren(IN ESHANDLE hXmlNode, OUT ESHANDLE* phXmlNodeChildrenList, IN OUT u4* pu4ChildrenNum);

	/**
	 *  @brief 获得节点的名称。
	 * 
	 *  @param[in] hXmlNode XML对象的句柄
	 *  @param[out] ppszName 节点的名称，二级指针，调用者无需释放对应内存，但是在调用ExXmlFree之后，不可再使用
	 *
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetClassName(IN ESHANDLE hXmlNode, OUT const char** ppszName);

	/**
	 *  @brief 获得节点的文本。例如有这样的一个节点：“<nodeName>Node Text</nodeName>”，
	 * 		   则对此节点调用“EsXmlGetText”，*ppszName指向的内存为字符串“Node Text”。
	 * 
	 *  @param[in] hXmlNode XML对象的句柄
	 *  @param[out] ppszName 节点的文本信息，二级指针，调用者无需释放对应内存，但是在调用ExXmlFree之后，不可再使用
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetText(IN ESHANDLE hXmlNode, OUT const char** ppszName);

	/**
	 *  @brief 获得属性列表。
	 * 
	 *  @param[in] hXmlNode XML对象的句柄
	 *  @param[out] pblkAttrList 属性列表，无需释放相应内存，但是在调用ExXmlFree之后，不可再使用
	 * 	@param[in,out]pu4AttrCount 属性的数目，作输入时，指示缓冲区的大小（以相应的结构体计），作输出时，表示属性的实际数目。
	 * 
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetAttributes(IN ESHANDLE hXmlNode, OUT BlkXmlAttribute* pblkAttrList, INOUT u4* pu4AttrCount);

	/**
	 *  @brief 获得属性值。
	 * 
	 *  @param[in] hXmlNode XML对象的句柄
	 *  @param[in] pszName 属性的名称
	 *  @param[out] ppszValue 属性的值，二级指针，无需释放相应的内存，但是在调用ExXmlFree之后，不可再使用
	 * 
	 *  @return 0-成功 other-失败，详见EsError.h
	 * 
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetAttribute(IN ESHANDLE hXmlNode, IN const char* pszName, OUT const char** ppszValue);

	/**
	 *  @brief 设置xml解释时是否压缩空格，应该加载xml前设置
	 *  
	 *  @param[in] bCondense 解析xml时是否压缩空格(本进程/dll库内有效)
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlSetCondenseWhiteSpace(IN ESBOOL bCondense);

	/**
	 *  @brief 获取xml解释时是否压缩空格
	 *  
	 *  @param[out] pbCondense 解析xml时是否压缩空格
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlGetCondenseWhiteSpace(IN ESBOOL* pbCondense);

	/**
	 *  @brief 建立xml文档结构
	 *  
	 *  @param[in] enCharSet 文档编码
	 *  @param[out] phXml xml文档句柄
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlCreate(IN EnCharSet enCharSet, OUT ESHANDLE* phXml);

	/**
	 *  @brief 增加一个子节点
	 *  
	 *  @param[in] hXml 当前节点
	 *  @param[in] pszClassName 节点tag
	 *  @param[in] pszValue 节点value
	 *  @param[out] phChild 新增子节点的句柄
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlAddChild(IN ESHANDLE hXml, IN const char* pszClassName, IN const char* pszValue, OUT ESHANDLE* phChild);

	/**
	 *  @brief 更新节点的值
	 *  
	 *  @param[in] hXml 当前节点
	 *  @param[in] pszText 节点值
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlSetText(IN ESHANDLE hXml, IN const char* pszText);

	/**
	 *  @brief 设置属性值，若属性已存在，更新属性值，否则增加属性
	 *  
	 *  @param[in] hXml 当前节点
	 *  @param[in] pszAttrName 属性名称
	 *  @param[in] pszAttrValue 属性值
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlSetAttribute(IN ESHANDLE hXml, IN const char* pszAttrName, IN const char* pszAttrValue);

	/**
	 *  @brief 将xml文档保存到文件
	 *  
	 *  @param[in]hXml xml句柄
	 *  @param[in]pszPath 文件路径
	 *  
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlWriteToFile(IN ESHANDLE hXml, IN const char* pszPath);

	/**
	 *  @brief 将xml文档保存到指定的缓冲区
	 * 
	 *  @param[in] hXml xml句柄
	 *  @param[out] pu1Data 缓冲区
	 *  @param[in,out] pu4Len 缓冲区长度/数据长度
	 * 
	 *  @return 0-成功 other-失败，详见EsError.h
	 *  
	 *  @details none
	 */
	u4 CALL_TYPE EsXmlWriteToMemory(IN ESHANDLE hXml, OUT u1* pu1Data, INOUT u4* pu4Len);

#ifdef __cplusplus
}
#endif

#endif // _ES_XML_H_
