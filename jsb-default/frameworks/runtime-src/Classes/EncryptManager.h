//
// Copyright (c) 2014-2019 x-studio365 - All Rights Reserved
//
#ifndef  _ENCRYPTMANAGER_H_
#define  _ENCRYPTMANAGER_H_
#include <string>
#include <stdint.h>
#include <unordered_map>
#include "cryptk/string_view.hpp"

class FileUtilsEncrypt;

class  EncryptManager final {
    friend class FileUtilsEncrypt;
public:
    enum ENCRYPT_FLAG
    {
        ENCF_NOFLAGS,
        ENCF_COMPRESS,
        ENCF_SIGNATURE = 2,
    };

    struct SignInfo
    {
        uint32_t mask = 0;
        uint32_t sigval = 0;
        union {
            uint8_t  reserved : 6;
            uint8_t  compressed : 2;
            uint8_t flags = 0;
        };
        int32_t original_size = -1;
    };

    static EncryptManager *getInstance();

    /*
    The default ivec is: nsc::hex2bin("00234b89aa96fecdaf80fbf178a25621")
    @Remark:
    1.If the key/ivec generated by batch encrypt tool, you need use nsc::hex2bin
    to translate the key/ivec hexstring format to binary format.
    And you must check compress option.
    2.If you use x-studio365 to publish assets with specific encrypt key, you do not need translate
    it with nsc::hex2bin.

    Why we are need to compress data before encrypt(picture or script, any other blob data)?
    Because when encrypt with AES-CBC mode, the result data will be few of redundancy data, any compress
    algorithm could not compress the AES-CBC encrypted data again, so we need compress data before encrypt it.
    Then final package(.apk,.ipa,.appx) will small, both of platform app pacakge use zip algorithm.
    */
    void setEncryptEnabled(bool bVal, 
        cxx17::string_view key = "",
        cxx17::string_view ivec = "",
        int flags = ENCF_NOFLAGS);
    bool isEncryptEnabled(void) const { return _encryptEnabled; }

    std::string decryptData(std::string data);

private:
    bool parseSignInfo(const char* data, size_t len, SignInfo* info) const;
protected:
    void setupHookFuncs();

private:
    bool _encryptEnabled = false;
    int _encryptFlags = ENCF_NOFLAGS;
    std::string _encryptKey;
    std::string _encryptIvec; // required by CBC mode.
    std::string _encryptSignKey;
};

#endif
