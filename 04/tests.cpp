#include <gtest/gtest.h>
#include "big_numbers.hpp"
#include <string>
#include <limits>
#include <sstream>

class BigIntTest : public ::testing::Test {
protected:
    void SetUp() override {}
    
    // Вспомогательная функция для преобразования BigInt в string (также проверяет корректность вывода в поток)
    std::string to_string(const BigInt& num) {
        std::ostringstream oss;
        oss << num;
        return oss.str();
    }
};

// Тест основных конструкторов
TEST_F(BigIntTest, BaseConstructors) {
    BigInt a;
    EXPECT_EQ(to_string(a), "0");
    
    BigInt b(123);
    EXPECT_EQ(to_string(b), "123");
    
    BigInt c(-458);
    EXPECT_EQ(to_string(c), "-458");
    
    BigInt d(std::numeric_limits<int32_t>::max());
    EXPECT_EQ(to_string(d), "2147483647");
    
    BigInt e(std::numeric_limits<int32_t>::min());
    EXPECT_EQ(to_string(e), "-2147483648");
    
    BigInt f("123456789012345678901234567890");
    EXPECT_EQ(to_string(f), "123456789012345678901234567890");
    
    BigInt g("-987654321098765432109876543210");
    EXPECT_EQ(to_string(g), "-987654321098765432109876543210");
    
    BigInt h("0000123");
    EXPECT_EQ(to_string(h), "123");
}

// Тест конструктора копирования
TEST_F(BigIntTest, CopyConstructor) {
    BigInt a("123456789");
    BigInt b = a;
    EXPECT_EQ(to_string(a), "123456789");
    EXPECT_EQ(to_string(b), "123456789");
}

// Тест конструктора перемещения
TEST_F(BigIntTest, MoveConstructor) {
    BigInt a("123456789");
    BigInt b = std::move(a);
    EXPECT_EQ(to_string(b), "123456789");
}

// Тест оператора копирования
TEST_F(BigIntTest, CopyAssignment) {
    BigInt a("123456789");
    BigInt b;
    b = a;
    EXPECT_EQ(to_string(a), "123456789");
    EXPECT_EQ(to_string(b), "123456789");
}

// Тест оператора перемещения
TEST_F(BigIntTest, MoveAssignment) {
    BigInt a("123456789");
    BigInt b;
    b = std::move(a);
    EXPECT_EQ(to_string(b), "123456789");
}

// Тест сложения
TEST_F(BigIntTest, Addition) {
    BigInt a("123");
    BigInt b("456");
    EXPECT_EQ(to_string(a + b), "579");

    BigInt c("999");
    BigInt d("1");
    EXPECT_EQ(to_string(c + d), "1000");

    BigInt e("100");
    BigInt f("-50");
    EXPECT_EQ(to_string(e + f), "50");

    BigInt g("1000");
    EXPECT_EQ(to_string(g + 500), "1500");
}

// Тест вычитания
TEST_F(BigIntTest, Subtraction) {
    BigInt a("1000");
    BigInt b("500");
    EXPECT_EQ(to_string(a - b), "500");

    BigInt c("100");
    BigInt d("200");
    EXPECT_EQ(to_string(c - d), "-100");
    
    BigInt e("1000");
    BigInt f("1");
    EXPECT_EQ(to_string(e - f), "999");
    
    BigInt g("1000");
    EXPECT_EQ(to_string(g - 1500), "-500");
}

// Тест унарного минуса
TEST_F(BigIntTest, UnaryMinus) {
    BigInt a("123");
    BigInt b = -a;
    EXPECT_EQ(to_string(b), "-123");
    
    BigInt c("-456");
    BigInt d = -c;
    EXPECT_EQ(to_string(d), "456");
    
    BigInt e("0");
    BigInt f = -e;
    EXPECT_EQ(to_string(f), "0");
}

// Тест умножения
TEST_F(BigIntTest, Multiplication) {
    BigInt a("12");
    BigInt b("34");
    EXPECT_EQ(to_string(a * b), "408");

    BigInt c("123456789");
    BigInt d("0");
    EXPECT_EQ(to_string(c * d), "0");
    
    BigInt e("123");
    BigInt f("-456");
    EXPECT_EQ(to_string(e * f), "-56088");
    
    BigInt g("123456789");
    BigInt h("987654321");
    EXPECT_EQ(to_string(g * h), "121932631112635269");
    
    BigInt i("123");
    EXPECT_EQ(to_string(i * 2), "246");
}

// Тест сравнений
TEST_F(BigIntTest, Comparisons) {
    BigInt a("100");
    BigInt b("200");
    BigInt c("100");
    BigInt d("-50");
    
    EXPECT_TRUE(a == c);
    EXPECT_FALSE(a == b);
    
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != c);
    
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(d < a);
    EXPECT_FALSE(b < a);
    
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a > d);
    EXPECT_FALSE(a > b);
    
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b >= a);
    EXPECT_TRUE(a >= c);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}