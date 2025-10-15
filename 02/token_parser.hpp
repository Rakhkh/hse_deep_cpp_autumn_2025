#ifndef TOKEN_PARSER_H
#define TOKEN_PARSER_H

#include <string>
#include <functional>
#include <cstdint>

class TokenParser {
public:
    using StartCallback = std::function<void()>;
    using EndCallback = std::function<void()>;
    using DigitCallback = std::function<void(uint64_t)>;
    using StringCallback = std::function<void(const std::string&)>;

    TokenParser() = default;

    void SetStartCallback(StartCallback cb);
    void SetEndCallback(EndCallback cb);
    void SetDigitTokenCallback(DigitCallback cb);
    void SetStringTokenCallback(StringCallback cb);

    void Parse(const std::string &text);

private:
    StartCallback startCallback_;
    EndCallback endCallback_;
    DigitCallback digitCallback_;
    StringCallback stringCallback_;

    static bool IsNumber(const std::string &s);
    static bool TryParseUint64(const std::string &s, uint64_t &result);
};

#endif 
