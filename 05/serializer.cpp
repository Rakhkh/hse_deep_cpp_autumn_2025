#include "serializer.hpp"
#include <cstdint>

Error Serializer::process(bool value)
{
    out_ << (value ? "true" : "false");
    return Error::NoError;
}

Error Serializer::process(uint64_t value)
{
    out_ << value;
    return Error::NoError;
}

Error Deserializer::process(bool& value)
{
    std::string text;
    in_ >> text;

    if (text == "true")
        value = true;
    else if (text == "false")
        value = false;
    else
        return Error::CorruptedArchive;

    return Error::NoError;
}

Error Deserializer::process(uint64_t& value)
{
    std::string text;
    in_ >> text;

    if (text[0] == '-') {
        return Error::CorruptedArchive;
    }

    try
    {
        size_t pos = 0;
        uint64_t num = std::stoull(text, &pos);
        if (pos != text.size())
            return Error::CorruptedArchive;
        value = num;
    }
    catch (...)
    {
        return Error::CorruptedArchive;
    }

    return Error::NoError;
}
