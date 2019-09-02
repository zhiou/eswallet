#include "sm2.hpp"
#include "EsCommon.h"
#include "transmit_def.hpp"

namespace excelsecu {
    namespace sm2 {
        
        std::pair<std::string, std::string> gen_keypair()
        {
            
            BlkAsymKey blkAsymKey;
            blkAsymKey.m_u4AlgId = ES_ASYM_ALG_SM2;
            blkAsymKey.m_u4ModLen = 0x20;
            
            EsAsymGenKeyPair(&blkAsymKey);
            
            bytestream prikey(blkAsymKey.m_unionAsymKey.m_blkSm2Key.m_pu1PriKeyD, 0x20);
            bytestream pubkeyX(blkAsymKey.m_unionAsymKey.m_blkSm2Key.m_pu1PubKeyX, 0x20);
            bytestream pubkeyY(blkAsymKey.m_unionAsymKey.m_blkSm2Key.m_pu1PubKeyY, 0x20);
            return std::pair<std::string, std::string>(prikey.hex_str(), pubkeyX.hex_str() + pubkeyY.hex_str());
        }
        
        bytestream encrypt(const bytestream& plain, const bytestream& pubkey)
        {
            u4 cipher_len = (u4)plain.length() + 0x60;
            std::unique_ptr<ebyte> cipher(new ebyte[cipher_len]);
            
            u4 ret = EsSm2Enc(pubkey.bytes(), pubkey.bytes() + 0x20, plain.bytes(), (u4)plain.length(), cipher.get(), &cipher_len);
            if (ret)
            {
                throw tsm_err("sm2 encrypt failed", ret);
            }
            
            return bytestream(cipher.get(), cipher_len);
        }
        
        bytestream decrypt(const bytestream& cipher, const bytestream& prikey)
        {
            u4 plain_len = (u4)cipher.length() - 0x60;
            std::unique_ptr<ebyte> plain(new ebyte[plain_len]);
            
            u4 ret = EsSm2Dec(prikey.bytes(), cipher.bytes(), (u4)cipher.length(), plain.get(), &plain_len);
            if (ret)
            {
                throw tsm_err("sm2 decrypt failed", ret);
            }
            
            return bytestream(plain.get(), plain_len);
        }
        
        bytestream sign(const bytestream& digest, const bytestream& prikey)
        {
            u1 signature[0x40];
            
            u4 ret = EsSm2Sign(prikey.bytes(), digest.bytes(), signature, signature + 0x20);
            if (ret)
            {
                throw tsm_err("sm2 sign failed", ret);
            }
            
            return bytestream(signature, 0x40);
        }
        
        bool verify(const bytestream& digest, const bytestream& sign, const bytestream& pubkey)
        {
            return EsSm2Verify(pubkey.bytes(), pubkey.bytes() + 0x20, digest.bytes(), sign.bytes(), sign.bytes() + 0x20) == 0;
        }
    }
}
