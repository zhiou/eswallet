#ifndef _ESP12_H_
#define _ESP12_H_

#ifdef __cplusplus
extern "C"
{
#endif
    
    /**
     *  @brief ֤��-PFX֤�����
     *
     *  @param[in] pszPfxFileName PFX֤���ļ�·��
     *  @param[in] pszPsw PFX֤�鱣������
     *  @param[out] pblkAsymKey �ǶԳ���Կ���ҵ��ĳ��˸�֤����ĵ�1��֤�����Կ��
     *  @param[out] pvCert ֤�飨�ҵ��ĳ��˸�֤����ĵ�1��֤�飩��֧�ִ���NULL�󳤶�
     *  @param[in,out] pu4CertLen ֤�黺��������/�������
     *
     *  @return 0-�ɹ� other-ʧ�ܣ����EsError.h
     *
     *  @details none
     */
    u4 CALL_TYPE EsPfxParse(IN const char* pszPfxFileName, IN const char* pszPsw, OUT BlkAsymKey* pblkAsymKey, OUT void* pvCert, INOUT u4* pu4CertLen);
    
#ifdef __cplusplus
};
#endif

#endif // !defined _ESP7_H_
