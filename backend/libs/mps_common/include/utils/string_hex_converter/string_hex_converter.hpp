#ifndef SYSTEM_CONVERTER_HPP
#define SYSTEM_CONVERTER_HPP

#include <iostream>
#include <cstring>

namespace mps {
namespace converter {
class converter_ {
public:
    explicit converter_();

    ~converter_() = default;

    std::string c_string_to_hex(const std::string& input);

    std::string c_hex_to_string(const std::string& input);
};
}       /// <--- converter
}   /// <--- mps

#endif
