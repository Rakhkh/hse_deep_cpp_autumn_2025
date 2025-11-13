#pragma once
#include <iostream>
#include <string>
#include <cstdint>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    std::ostream& out_;
    static constexpr char Separator = ' ';

public:
    explicit Serializer(std::ostream& out) : out_(out)
    { }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    Error process(bool value);
    Error process(uint64_t value);

    template <class T, class... Args>
    Error process(T&& value, Args&&... args)
    {
        if (process(std::forward<T>(value)) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        out_ << Separator;
        return process(std::forward<Args>(args)...);
    }

    Error process() { return Error::NoError; }
};

class Deserializer
{
    std::istream& in_;

public:
    explicit Deserializer(std::istream& in) : in_(in)
    { }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    Error process(bool& value);
    Error process(uint64_t& value);

    template <class T, class... Args>
    Error process(T& value, Args&... args)
    {
        if (process(value) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(args...);
    }

    Error process() { return Error::NoError; }
};
