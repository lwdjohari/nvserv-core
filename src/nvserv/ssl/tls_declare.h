#pragma once
#include "nvserv/global.h"
#include <nvm/macro.h>

NVSERV_BEGIN_NAMESPACE(ssl)

enum class TLSVersion{
    None = 0,
    TLSv1_1 = 1,
    TLSv1_2 = 2,
    TLSv1_3 = 3
};

enum class CertificateType {
    PEM = 0,
    ASN1 = 1
};


struct TLSConfig{
    bool use_ssl = false;
    bool use_custom_cipher_suites = false;
    TLSVersion tls_version = TLSVersion::TLSv1_2;
    CertificateType certificate_type = CertificateType::PEM;
    std::vector<std::string> cipher_suites;  
    std::string certificate_path;
    std::string private_key_path;
    std::string dh_params_path;
};


NVSERV_END_NAMESPACE