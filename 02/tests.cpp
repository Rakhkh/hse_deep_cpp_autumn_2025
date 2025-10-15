#include <gtest/gtest.h>
#include "token_parser.hpp"
#include <vector>
#include <string>

class TokenParserTest : public ::testing::Test {
protected:
    TokenParser parser;
    std::vector<uint64_t> digitTokens;
    std::vector<std::string> stringTokens;
    bool startCalled = false;
    bool endCalled = false;

    void SetUp() override {
        digitTokens.clear();
        stringTokens.clear();
        startCalled = false;
        endCalled = false;

        parser.SetStartCallback([this]() { startCalled = true; });
        parser.SetEndCallback([this]() { endCalled = true; });
        parser.SetDigitTokenCallback([this](uint64_t token) {
            digitTokens.push_back(token);
        });
        parser.SetStringTokenCallback([this](const std::string& token) {
            stringTokens.push_back(token);
        });
    }
};

// Тест на пустую строку
TEST_F(TokenParserTest, EmptyString) {
    parser.Parse("");
    
    EXPECT_TRUE(startCalled);
    EXPECT_TRUE(endCalled);
    EXPECT_TRUE(digitTokens.empty());
    EXPECT_TRUE(stringTokens.empty());
}

// Тест на строку только с пробелами
TEST_F(TokenParserTest, OnlySpaces) {
    parser.Parse("   \t\n  \r\n  ");
    
    EXPECT_TRUE(startCalled);
    EXPECT_TRUE(endCalled);
    EXPECT_TRUE(digitTokens.empty());
    EXPECT_TRUE(stringTokens.empty());
}

// Тест на цифровые токены
TEST_F(TokenParserTest, DigitTokens) {
    parser.Parse("123 456 789");
    
    EXPECT_TRUE(startCalled);
    EXPECT_TRUE(endCalled);
    
    ASSERT_EQ(digitTokens.size(), 3u);
    EXPECT_EQ(digitTokens[0], 123u);
    EXPECT_EQ(digitTokens[1], 456u);
    EXPECT_EQ(digitTokens[2], 789u);
    
    EXPECT_TRUE(stringTokens.empty());
}

// Тест на строковые токены
TEST_F(TokenParserTest, StringTokens) {
    parser.Parse("hello world test");
    
    EXPECT_TRUE(startCalled);
    EXPECT_TRUE(endCalled);
    
    ASSERT_EQ(stringTokens.size(), 3u);
    EXPECT_EQ(stringTokens[0], "hello");
    EXPECT_EQ(stringTokens[1], "world");
    EXPECT_EQ(stringTokens[2], "test");
    
    EXPECT_TRUE(digitTokens.empty());
}

// Тест на смешанные токены
TEST_F(TokenParserTest, MixedTokens) {
    parser.Parse("hello 123 world 456 test");
    
    EXPECT_TRUE(startCalled);
    EXPECT_TRUE(endCalled);
    
    ASSERT_EQ(digitTokens.size(), 2u);
    EXPECT_EQ(digitTokens[0], 123u);
    EXPECT_EQ(digitTokens[1], 456u);
    
    ASSERT_EQ(stringTokens.size(), 3u);
    EXPECT_EQ(stringTokens[0], "hello");
    EXPECT_EQ(stringTokens[1], "world");
    EXPECT_EQ(stringTokens[2], "test");
}

// Тест с разными пробельными символами
TEST_F(TokenParserTest, DifferentSpaces) {
    parser.Parse("  token1\ttoken2\ntoken3\r\ntoken4  ");
    
    EXPECT_TRUE(startCalled);
    EXPECT_TRUE(endCalled);
    
    ASSERT_EQ(stringTokens.size(), 4u);
    EXPECT_EQ(stringTokens[0], "token1");
    EXPECT_EQ(stringTokens[1], "token2");
    EXPECT_EQ(stringTokens[2], "token3");
    EXPECT_EQ(stringTokens[3], "token4");
}

// Тест на больших числах
TEST_F(TokenParserTest, LargeNumbers) {
    parser.Parse("18446744073709551615 999999999999"); 
    
    EXPECT_TRUE(startCalled);
    EXPECT_TRUE(endCalled);
    
    ASSERT_EQ(digitTokens.size(), 2u);
    EXPECT_EQ(digitTokens[0], 18446744073709551615ULL);
    EXPECT_EQ(digitTokens[1], 999999999999ULL);
}

// Тест на числа, начинающиеся с нуля
TEST_F(TokenParserTest, LeadingZeros) {
    parser.Parse("00123 000456");
    
    EXPECT_TRUE(startCalled);
    EXPECT_TRUE(endCalled);
    
    ASSERT_EQ(digitTokens.size(), 2u);
    EXPECT_EQ(digitTokens[0], 123u);
    EXPECT_EQ(digitTokens[1], 456u);
    
    EXPECT_TRUE(stringTokens.empty());
}

// Тест специальных символов в строках
TEST_F(TokenParserTest, SpecialCharacters) {
    parser.Parse("hello-world test123 456abc @#$%");
    
    EXPECT_TRUE(startCalled);
    EXPECT_TRUE(endCalled);
    
    EXPECT_TRUE(digitTokens.empty()); 
    
    ASSERT_EQ(stringTokens.size(), 4u);
    EXPECT_EQ(stringTokens[0], "hello-world");
    EXPECT_EQ(stringTokens[1], "test123");
    EXPECT_EQ(stringTokens[2], "456abc");
    EXPECT_EQ(stringTokens[3], "@#$%");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}