
#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <string>
#include <vector>

namespace Utility {

    std::string EncodeBase64(const std::string &txt);

    std::string DecodeBase64(const std::string &txt);

    std::vector<std::string> Split(const std::string &s, const std::string &delimiter);

    std::string GenerateUUID();

} // Utility

#endif //_UTILITY_HPP
