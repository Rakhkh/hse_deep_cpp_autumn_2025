#pragma once
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

template<typename U>
std::string stringify(const U& v) {
    std::ostringstream os;
    os << v;
    return os.str();
}

class FormatError : public std::runtime_error {
public:
    explicit FormatError(const std::string& msg) : std::runtime_error(msg) {}
};

std::string format(const std::string& fmt);
std::string format(const std::string& fmt, const std::vector<std::string>& values);

template<typename... Args>
std::string format(const std::string& fmt, const Args&... args) {
    std::vector<std::string> list = { stringify(args)... };
    return format(fmt, list);
}


