#ifndef _ESP10_H_
#define _ESP10_H_

#ifdef __cplusplus
extern "C"
{
#endif
    
	typedef enum EnP10AttrTypeTag
	{
		ESP10_ATTR_TYPE_DEFAULT = 0, // 默认
		ESP10_ATTR_TYPE_CFCA1 = 1,	// cfca格式，发现在CGB项目中使用
	}EnP10AttrType;

    /**
     *  @brief 生成P10请求信息
     *
	 *  @param[in] pblkAsymKey 请求信息对应的公钥
     *  @param[in] pblkAsymKeyTemp 临时密钥，双证下载时有效
     *  @param[in] pszDn 证书DN，格式为"CN=XX,E=XXX,OU=XXX"
     *  @param[out] pvRequestInfo 输出P10格式的请求信息，支持传入NULL求长度
     *  @param[in,out] pu4RequestInfoLen 缓冲区长度/请求信息长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
    u4 CALL_TYPE EsP10GenRequestInfo(IN const BlkAsymKey* pblkAsymKey,
                                     IN const BlkAsymKey* pblkAsymKeyTemp,
                                     IN const char* pszDn,
                                     OUT void* pvRequestInfo,
                                     INOUT u4* pu4RequestInfoLen);
    
    /**
     *  @brief 生成P10请求信息
     *
     *  @param[in] u4P10AttrType P10的类型（主要是属性信息不一样），详见EnP10AttrType
     *  @param[in] pblkAsymKey 请求信息对应的公钥
     *  @param[in] pblkAsymKeyTemp 临时密钥，双证下载时有效
     *  @param[in] pszDn 证书DN，格式为"CN=XX,E=XXX,OU=XXX"
     *  @param[out] pvRequestInfo 输出P10格式的请求信息，支持传入NULL求长度
     *  @param[in,out] pu4RequestInfoLen 缓冲区长度/请求信息长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
    u4 CALL_TYPE EsP10GenRequestInfoWithType(IN u4 u4P10AttrType,
											 IN const BlkAsymKey* pblkAsymKey,
											 IN const BlkAsymKey* pblkAsymKeyTemp, 
											 IN const char* pszDn,
											 OUT void* pvRequestInfo,
											 INOUT u4* pu4RequestInfoLen);

    /**
     *  @brief 生成P10请求
     *
     *  @param[in] pvRequstInfo P10请求信息，由EsP10GenRequestInfo生成
     *  @param[in] u4RequestInfoLen 请求信息长度
     *  @param[in] pvSignature 私钥对请求信息的签名值
     *  @param[in] u4HashAlgId 签名使用的Hash算法Id
     *  @param[out] pvRequest P10请求信息，支持传入NULL求长度
     *  @param[in,out] pu4RequestLen 缓冲区长度/P10请求长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
    u4 CALL_TYPE EsP10GenRequest(IN const void* pvRequstInfo,
                                 IN u4 u4RequestInfoLen,
                                 IN const void* pvSignature,
                                 IN u4 u4HashAlgId,
                                 OUT void* pvRequest,
                                 INOUT u4* pu4RequestLen);
    /**
     *  @brief 解析P10请求(属性类型默认为ESP10_ATTR_TYPE_DEFAULT)
     *
     *  @param[in] pvRequest P10请求，由EsP10GenRequest生成
     *  @param[in] u4RequestLen P10请求长度
     *  @param[out] pblkAsymKeySign P10的签名公钥
     *  @param[out] pblkAsymTemp 临时密钥
     *  @param[out] pszDn 证书DN，支持传入NULL求长度
     *  @param[in,out] pu4DnLen 缓冲区长度/证书DN长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details TODO: 如果需要解析其他格式的P10（属性不同），则还需要添加新的方法!!
     */
    u4 CALL_TYPE EsP10ParseRequest(IN const void* pvRequest, 
                                   IN u4 u4RequestLen, 
                                   OUT BlkAsymKey* pblkAsymKeySign,
                                   OUT BlkAsymKey* pblkAsymTemp,
                                   OUT char* pszDn, 
                                   INOUT u4* pu4DnLen);
#ifdef __cplusplus
};
#endif

#endif // !defined_ESP10_H_
