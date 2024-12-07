#pragma once

#include <nvserv/global_macro.h>

#include <string>
#include <vector>
#include <functional>

// cppcheck-suppress unknownMacro
NVSERV_BEGIN_NAMESPACE(utils::string)
std::vector<std::string> SplitString(const std::string& str,
                                            char delimiter) ;

void SplitString(const std::string& str, char delimiter,
                 const std::function<void(const std::string&)>& callback);

std::string RemoveSpaces(const std::string& str_to_clean);

void RemoveSpacesRef(std::string& str);
NVSERV_END_NAMESPACE