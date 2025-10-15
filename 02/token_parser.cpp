#include "token_parser.hpp"
#include <cctype>
#include <limits>

void TokenParser::SetStartCallback(StartCallback cb) {
    startCallback_ = std::move(cb);
}

void TokenParser::SetEndCallback(EndCallback cb) {
    endCallback_ = std::move(cb);
}

void TokenParser::SetDigitTokenCallback(DigitCallback cb) {
    digitCallback_ = std::move(cb);
}

void TokenParser::SetStringTokenCallback(StringCallback cb) {
    stringCallback_ = std::move(cb);
}

void TokenParser::Parse(const std::string &text) {
    if (startCallback_) 
        startCallback_();

    size_t i = 0;
    const size_t len = text.size();

    while (i < len) {
        for (; i < len && std::isspace(static_cast<unsigned char>(text[i])); ++i) ;

        if (i >= len) 
            break;

        size_t start = i;
        for (; i < len && !std::isspace(static_cast<unsigned char>(text[i])); ++i) ;

        std::string token = text.substr(start, i - start);
        if (token.empty()) 
            continue;

        if (IsNumber(token)) {
            uint64_t value;
            if (TryParseUint64(token, value)) {
                if (digitCallback_) 
                    digitCallback_(value); }
            else {
                if (stringCallback_) 
                    stringCallback_(token); }
        } 
        else 
            if (stringCallback_) 
                stringCallback_(token);
    }

    if (endCallback_) 
        endCallback_();
}

bool TokenParser::IsNumber(const std::string &s) {
    if (s.empty()) 
        return false;

    for (char c : s) 
        if (!std::isdigit(static_cast<unsigned char>(c)))
            return false;
    return true;
}

bool TokenParser::TryParseUint64(const std::string &s, uint64_t &result) {
    uint64_t val = 0;
    for (char c : s) {
        uint64_t digit = c - '0';
        if (val > (std::numeric_limits<uint64_t>::max() - digit) / 10) 
            return false; 
        val = val * 10 + digit;
    }
    result = val;
    return true;
}
