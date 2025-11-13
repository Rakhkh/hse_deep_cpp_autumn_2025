#include <gtest/gtest.h>
#include <sstream>
#include "serializer.hpp"

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
};

struct SimpleStruct
{
    bool flag;
    uint64_t number;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(flag, number);
    }
};

// Верная сериализация
TEST(SerializerTest, CorrectSerialization) {
    Data x {1, true, 2};
    std::stringstream stream;

    Serializer serializer(stream);
    serializer.save(x);

    EXPECT_EQ(stream.str(), "1 true 2");
}

// Верная десериализация
TEST(SerializerTest, CorrectDeserialization) {
    std::stringstream stream("1 true 2");
    Data y {0, false, 0};

    Deserializer deserializer(stream);
    Error err = deserializer.load(y);

    EXPECT_EQ(err, Error::NoError);
    EXPECT_EQ(y.a, 1u);
    EXPECT_TRUE(y.b);
    EXPECT_EQ(y.c, 2u);
}

// Сериализация -> десериализация
TEST(SerializerTest, RoundTrip) {
    Data original {42, false, 100500};
    std::stringstream stream;

    Serializer serializer(stream);
    serializer.save(original);

    Data restored {0, true, 0};
    Deserializer deserializer(stream);
    Error err = deserializer.load(restored);

    EXPECT_EQ(err, Error::NoError);
    EXPECT_EQ(original.a, restored.a);
    EXPECT_EQ(original.b, restored.b);
    EXPECT_EQ(original.c, restored.c);
}


// Ошибка десериализации
TEST(DeserializerTest, InvalidBool) {
    std::stringstream stream("invalid_value 123abc");
    SimpleStruct s {false, 0};

    Deserializer deserializer(stream);
    Error err = deserializer.load(s);

    EXPECT_EQ(err, Error::CorruptedArchive);
}

// Отрицательное число (тоже должна быть ошибка)
TEST(DeserializerTest, InvalidUint64) {
    std::stringstream stream("true -123"); 
    SimpleStruct s {false, 0};

    Deserializer deserializer(stream);
    Error err = deserializer.load(s);

    EXPECT_EQ(err, Error::CorruptedArchive);
}

// Только одно поле (тоже ошибка)
TEST(DeserializerTest, MissingFields) {
    std::stringstream stream("true");  
    SimpleStruct s {false, 0};

    Deserializer deserializer(stream);
    Error err = deserializer.load(s);

    EXPECT_EQ(err, Error::CorruptedArchive);
}

// Пустая строка
TEST(DeserializerTest, EmptyStream) {
    std::stringstream stream("");
    Data d {0, false, 0};

    Deserializer deserializer(stream);
    Error err = deserializer.load(d);

    EXPECT_EQ(err, Error::CorruptedArchive);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}