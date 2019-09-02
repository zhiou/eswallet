#ifndef _ESDER_H_
#define _ESDER_H_

_BEGIN_C_DECL_

#include "EsBer.h"

#ifndef szOID_SM2
#	define szOID_SM2				"1.2.156.10197.1.301"
#	define szOID_SM3				"1.2.156.10197.1.401"
#	define szOID_SM2_SIGN			"1.2.156.10197.1.301.1"
#	define szOID_SM2_KEYEXCHANGE	"1.2.156.10197.1.301.2"
#	define szOID_SM2_ENCRYPT		"1.2.156.10197.1.301.3"
#	define szOID_SM2_SM3			"1.2.156.10197.1.501"
#	define szOID_RSA_SM3			"1.2.156.10197.1.504"
#endif

#ifndef	szOID_ECC_PUBLIC_KEY
#	define szOID_ECC_PUBLIC_KEY "1.2.840.10045.2.1"
#endif
#ifndef	szOID_RSA_SHA256RSA
#	define szOID_RSA_SHA256RSA  "1.2.840.113549.1.1.11"
#endif
#ifndef	szOID_RSA_SHA384RSA
#	define szOID_RSA_SHA384RSA  "1.2.840.113549.1.1.12"
#endif
#ifndef	szOID_RSA_SHA512RSA
#	define szOID_RSA_SHA512RSA  "1.2.840.113549.1.1.13"
#endif
#ifndef	szOID_3DES168
#	define szOID_3DES168        "1.3.6.1.4.1.4929.1.7"
#endif
#ifndef szOID_RSA_RSA 
#   define szOID_RSA_RSA        "1.2.840.113549.1.1.1"
#endif
#ifndef szOID_RSA_SHA1RSA
#   define szOID_RSA_SHA1RSA    "1.2.840.113549.1.1.5"
#endif
#ifndef szOID_RSA_MD5RSA
#   define szOID_RSA_MD5RSA     "1.2.840.113549.1.1.4"
#endif
#ifndef szOID_RSA_MD5
#define szOID_RSA_MD5           "1.2.840.113549.2.5"
#endif

#ifndef szOID_NIST_sha256
#define szOID_NIST_sha256		"2.16.840.1.101.3.4.2.1"
#endif

#ifndef szOID_NIST_sha384
#define szOID_NIST_sha384		"2.16.840.1.101.3.4.2.2"
#endif

#ifndef szOID_NIST_sha512
#define szOID_NIST_sha512		"2.16.840.1.101.3.4.2.3"
#endif

typedef BlkBer		BlkDer;
typedef BlkBerTree	BlkDerTree;

u4 CALL_TYPE EsDerOidStr2Hex(const char* pszOidStr, u1* pu1OidDer, u4* pu4OidDerLen);
u4 CALL_TYPE EsDerEncodeAlg(const char* pszAlg, const u1* pu1DerParam, u4 u4DerParamLen, u1* pu1DerOid, u4* pu4DerOidLen);
u4 CALL_TYPE EsDerOidHex2Str(const u1* pu1HexOid, char* pszStrOid, u4 u4StrOidLen);
u4 CALL_TYPE EsDerOid2Str(const BlkBer* pblkBerOid, char* pszOid, u4 u4StrOidLen);
u4 CALL_TYPE EsDerOid2EsAlgId(const BlkBer* pblkBerOid, u4* pu4SignAlgId, u4* pu4HashAlgId);
u4 CALL_TYPE EsDerOid2PBEEsAlgId(const BlkBer* pblkBerOid, u4* pu4HashAlgId, u4* pu4SymmAlgId, u4* pu4SymmMode);

/**
 *  @brief 从sequence里面获取对应的非对称算法Id
 *
 *  @param [in] pblkBerTree tlv数据.格式如下
 * 							RSA:  30 xx
 * 										06 xx oid // oid
 * 										05 00	  // param
 * 
 * 						SM2/ecc:  30 xx
 * 										06 xx oid // oid
 * 										06 xx oid // param is curve
 * 
 *  @param [in] pu4SignAlgId 非对称算法ES_ASYM_ALG_XXXX--如果是ecc/sm2，可能会出现返回0的情况，调用者需要根据上下文自行确定算法id
 *  @param [out] pu4HashAlgId hash算法Id，ES_ASYM_ALG_XXXX
 *
 *  @return 0-成功 other-失败，详见EsError.h
 */
u4 CALL_TYPE EsDerGetAlgIdFromAlgSequence(const BlkBerTree* pblkBerTree, u4* pu4SignAlgId, u4* pu4HashAlgId); // todo

/**
 *  @brief 从sequence里面获取对应的非对称算法Id
 *
 *  @param [in] pu1Sequence tlv数据.格式如下
 * 							RSA:  30 xx
 * 										06 xx oid // oid
 * 										05 00	  // param
 * 
 * 						SM2/ecc:  30 xx
 * 										06 xx oid // oid
 * 										06 xx oid // param is curve
 * 
 *  @param [in] u4SequenceLen pu1Sequence数据长度
 *  @param [out] pu4AgId 算法Id，ES_ASYM_ALG_XXXX
 *
 *  @return 0-成功 other-失败，详见EsError.h
 */
u4 CALL_TYPE EsDerGetAsymAlgIdFromSequence(const u1* pu1Sequence, u4 u4SequenceLen, u4* pu4AgId);
u4 CALL_TYPE EsDerGetAsymAlgIdFromSequence2(const BlkBerTree* pblkBerTree, u4* pu4AgId);

u4 CALL_TYPE EsDerTreeGetDnFromSubject(const BlkBerTree* pblkBerTree, char* pszDn, u4* pu4DnLen);
u4 CALL_TYPE EsDerTreeGetPublicKey(const BlkBerTree* pblkBerTree, BlkAsymKey* pblkAsymKey);
u4 CALL_TYPE EsDerTreeGetTempPublicKey(const BlkBerTree* pblkBerTree, BlkAsymKey* pblkAsymKey);

u4 CALL_TYPE EsDerTreeGetKeyUsage(const BlkBerTree* pblkBerTree, u4* pu4KeyUsage);

u4 CALL_TYPE EsDerGetPubKeyViaSequence(const u1* pu1Sequence, u4 u4SequenceLen, BlkAsymKey* pblkAsymKey);

// 对于ecc/sm2算法，如果不指定u4SignAlgId，那么不能保证pu1Signature的长度为2*模长，调用者需要自行分别补足R和S至模长
// PS: 可以保证pu1Signature中R和S长度一样
u4 CALL_TYPE EsDerTreeGetSignature(const BlkBerTree* pblkBerTree, u4 u4SignAlgId, u1* pu1Signature, u4* pu4SignatureLen);

void CALL_TYPE EsDerTreeFree(BlkDerTree** ppblkDerTree);

u4 CALL_TYPE EsDerTreeNew(u2 u2Tag, const void* pvData, u4 u4DataLen, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewBigInt(const u1* pu1Value, u4 u4ValueLen, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewInteger(u4 u4Integer, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewBitString(const u1* pu1HexData, u4 u4HexDataLen, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewOctString(const u1* pu1HexData, u4 u4HexDataLen, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewNull(BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewOid(const char* pszOid, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewUtf8(const u1* pu1Utf8, u4 u4Utf8Len, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewSequence(BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewSet(BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewPrintableString(const char* pszString, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewUtc(const char* pszTime, BlkDerTree** ppblkDerTree);

u4 CALL_TYPE EsDerTreeNewSequenceOid(const char* pszOid, BlkDerTree* pblkDerTreeOidParam, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewSignOid(u4 u4AsymAlgId, u4 u4HashAlgId, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewPubKey(const BlkAsymKey* pblkAsymKey, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewParent(u2 u2Tag, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewParent2(u2 u2Tag, BlkDerTree** pblkDerTree1, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewSubject(const char* pszSubjectName, u4 u4SubjectNameLen, const char* pszSubjectValue, BlkDerTree** ppblkDerTree);
u4 CALL_TYPE EsDerTreeNewSubjectSequence(const char* pszDn, u4 u4DnLen, BlkBerTree** ppblkDerTree);

#define EsDerCalcTagLenSize(pblkDerTree)								EsBerCalcTagLenSize((BlkBerTree*)(pblkDerTree))
#define EsDerTreeDeflat(pu1DerData, u4DerDataLen, ppblkDerTree)			EsBerTreeDeflat(pu1DerData, u4DerDataLen, (BlkBerTree**)(ppblkDerTree))
#define EsDerTreeFlat(pblkDerTree, pu1DerData, pu4DerDataLen)			EsBerTreeFlat((BlkBerTree*)(pblkDerTree), pu1DerData, pu4DerDataLen)
#define EsDerTreeAddChild(pblkBerTreeParent, pblkBerTreeChild)			EsBerTreeAddChild((BlkBerTree*)(pblkBerTreeParent), (BlkBerTree*)(pblkBerTreeChild));
#define EsDerTreeMerge(u2Tag, pblkDerTree1, pblkDerTree2, ppblkDerTree)	EsBerTreeMerge(u2Tag, (BlkBerTree*)(pblkDerTree1), (BlkBerTree*)(pblkDerTree2), (BlkBerTree**)(ppblkDerTree))

_END_C_DECL_

#endif
