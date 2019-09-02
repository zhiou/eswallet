#ifndef _ESP12_H_
#define _ESP12_H_

#ifdef __cplusplus
extern "C"
{
#endif
    
    /**
     *  @brief 证书-PFX证书解析
     *
     *  @param[in] pszPfxFileName PFX证书文件路径
     *  @param[in] pszPsw PFX证书保护密码
     *  @param[out] pblkAsymKey 非对称密钥（找到的除了根证书外的第1张证书的密钥）
     *  @param[out] pvCert 证书（找到的除了根证书外的第1张证书），支持传入NULL求长度
     *  @param[in,out] pu4CertLen 证书缓冲区长度/结果长度
     *
     *  @return 0-成功 other-失败，详见EsError.h
     *
     *  @details none
     */
    u4 CALL_TYPE EsPfxParse(IN const char* pszPfxFileName, IN const char* pszPsw, OUT BlkAsymKey* pblkAsymKey, OUT void* pvCert, INOUT u4* pu4CertLen);
    
#ifdef __cplusplus
};
#endif

#endif // !defined _ESP7_H_
