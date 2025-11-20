#include <gtest/gtest.h>
#include "formatting.hpp"
#include <string>

// Тест на базовое форматирование
TEST(FormatTest, BasicFormatting) {
    EXPECT_EQ(format("{0} + {1} = {2}", 2, 3, 5), "2 + 3 = 5");
    EXPECT_EQ(format("{1}+{1} = {0}", 2, "one"), "one+one = 2");
    EXPECT_EQ(format("Hello, {0}!", "World"), "Hello, World!");
}

// Тест на повторное использование аргументов
TEST(FormatTest, DuplicateArguments) {
    EXPECT_EQ(format("{0} {1} {0}", "A", "B"), "A B A");
    EXPECT_EQ(format("{2} {1} {0} {1} {2}", 1, 2, 3), "3 2 1 2 3");
}

// Тест на разные типы данных
TEST(FormatTest, DifferentTypes) {
    EXPECT_EQ(format("int: {0}, double: {1}, string: {2}", 42, 3.14, "test"), "int: 42, double: 3.14, string: test");
    EXPECT_EQ(format("bool: {0} {1}", true, false), "bool: 1 0");
    
}

// Тест на пустые строки и отсутствие подстановок
TEST(FormatTest, NoSubstitutions) {
    EXPECT_EQ(format("Just a string"), "Just a string");
    EXPECT_EQ(format(""), "");
    EXPECT_EQ(format("No brackets here!"), "No brackets here!");
}

// Тест на исключения 
TEST(FormatTest, Exceptions) {
    EXPECT_THROW(format("{5}", 1, 2, 3), FormatError);
    EXPECT_THROW(format("{1}", 1), FormatError);
    EXPECT_THROW(format("{10}", 1, 2), FormatError);


    EXPECT_THROW(format("{0", 1), FormatError);
    EXPECT_THROW(format("{", 1), FormatError);
    

    EXPECT_THROW(format("{}", 1), FormatError);
    

    EXPECT_THROW(format("{a}", 1), FormatError);
    EXPECT_THROW(format("{ }", 1), FormatError);
    EXPECT_THROW(format("{+}", 1), FormatError);
    

    EXPECT_THROW(format("}", 1), FormatError);
    EXPECT_THROW(format("text } text", 1), FormatError);
    EXPECT_THROW(format("{0}}", 1), FormatError);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}