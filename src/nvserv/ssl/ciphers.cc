#include "nvserv/ssl/ciphers.h"

NVSERV_BEGIN_NAMESPACE(ssl)
std::vector<std::string> GetCipherSuites(const std::string& tls_version) {
    std::vector<std::string> cipher_suites;
    
    if (tls_version == "TLSv1.1") {
        cipher_suites = {
            "TLS_RSA_WITH_AES_128_CBC_SHA",
            "TLS_RSA_WITH_AES_256_CBC_SHA",
            "TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA",
            "TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA",
            "TLS_RSA_WITH_3DES_EDE_CBC_SHA",
            "TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA",
            "TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA"
        };
    } else if (tls_version == "TLSv1.2") {
        cipher_suites = {
            "TLS_RSA_WITH_AES_128_CBC_SHA",
            "TLS_RSA_WITH_AES_256_CBC_SHA",
            "TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA",
            "TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA",
            "TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256",
            "TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA256",
            "TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256",
            "TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384",
            "TLS_RSA_WITH_3DES_EDE_CBC_SHA",
            "TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA",
            "TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA",
            "TLS_RSA_WITH_AES_128_GCM_SHA256",
            "TLS_RSA_WITH_AES_256_GCM_SHA384",
            "TLS_ECDHE_RSA_WITH_AES_128_CCM",
            "TLS_ECDHE_RSA_WITH_AES_256_CCM"
        };
    } else if (tls_version == "TLSv1.3") {
        cipher_suites = {
            "TLS_AES_128_GCM_SHA256",    // Default cipher suite for TLS 1.3
            "TLS_AES_256_GCM_SHA384",
            "TLS_CHACHA20_POLY1305_SHA256",
            "TLS_AES_128_CCM_SHA256",
            "TLS_AES_256_CCM_SHA384"
        };
    }
    
    return cipher_suites;
}

NVSERV_END_NAMESPACE