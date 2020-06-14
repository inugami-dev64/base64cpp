#ifndef BASE64_H
#define BASE64_H
#include <string>
#include <vector>

namespace base64 {
    
    std::string encode(std::vector<unsigned char> decArr);
    std::vector<unsigned char> decode(std::string encodedMes);
}

#endif