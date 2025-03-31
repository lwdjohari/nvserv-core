#pragma once

#include "nvserv/global.h"
#include <vector>

NVSERV_BEGIN_NAMESPACE(ssl)

std::vector<std::string> GetCipherSuites(const std::string& tls_version);


NVSERV_END_NAMESPACE