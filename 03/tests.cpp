#include <gtest/gtest.h>
#include "matrix.hpp"
#include <stdexcept>
#include <sstream>

// Конструктор и базовые методы
TEST(MatrixTest, Constructor) {
    Matrix m(2, 3);
    EXPECT_EQ(m.getRows(), 2u);
    EXPECT_EQ(m.getColumns(), 3u);
    EXPECT_EQ(m[0][0], 0);
    EXPECT_EQ(m[1][2], 0);
}

// Конструктор с неположительными размерами
TEST(MatrixTest, ConstructorWithNegativeSize) {
    EXPECT_THROW(Matrix(-1, 5), std::out_of_range);
    EXPECT_THROW(Matrix(5, 0), std::out_of_range);
    EXPECT_THROW(Matrix(-1, -1), std::out_of_range);
}

// Присваивание по индексу
TEST(MatrixTest, IndexAssignment) {
    Matrix m(2, 2);
    m[0][0] = 5;
    m[0][1] = 7;
    m[1][0] = -3;
    m[1][1] = 9;

    EXPECT_EQ(m[0][0], 5);
    EXPECT_EQ(m[0][1], 7);
    EXPECT_EQ(m[1][0], -3);
    EXPECT_EQ(m[1][1], 9);
}

// Чтение по индексу
TEST(MatrixTest, IndexReading) {
    Matrix m(3, 3);
    m[0][0] = 10;
    m[1][1] = 15;
    int32_t a = m[0][0];
    int32_t b = m[1][1] * 2;

    EXPECT_EQ(a, 10);
    EXPECT_EQ(b, 30);
}

// Константный доступ
TEST(MatrixTest, ConstAccess) {
    Matrix m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    const Matrix& cm = m;
    EXPECT_EQ(cm[0][0], 1);
    EXPECT_EQ(cm[0][1], 2);
    EXPECT_EQ(cm[1][0], 3);
    EXPECT_EQ(cm[1][1], 4);
}

// Выброс исключений
TEST(MatrixTest, OutOfRangeThrows) {
    Matrix m(3, 3);
    EXPECT_THROW(m[3][0] = 1, std::out_of_range);
    EXPECT_THROW(m[0][3] = 1, std::out_of_range);
    EXPECT_THROW(m[5][2] = 1, std::out_of_range);  
    EXPECT_THROW(m[2][5] = 1, std::out_of_range);  
}

// Оператор *=
TEST(MatrixTest, Multiplication) {
    Matrix m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    m *= 3;

    EXPECT_EQ(m[0][0], 3);
    EXPECT_EQ(m[0][1], 6);
    EXPECT_EQ(m[1][0], 9);
    EXPECT_EQ(m[1][1], 12);
}

// Оператор +=
TEST(MatrixTest, CompoundAddition) {
    Matrix a(2, 2);
    Matrix b(2, 2);

    a[0][0] = 1; 
    a[0][1] = 2;
    a[1][0] = 3; 
    a[1][1] = 4;

    b[0][0] = 5; 
    b[0][1] = 6;
    b[1][0] = 7; 
    b[1][1] = 8;

    a += b;

    EXPECT_EQ(a[0][0], 6);
    EXPECT_EQ(a[0][1], 8);
    EXPECT_EQ(a[1][0], 10);
    EXPECT_EQ(a[1][1], 12);
}

// Оператор +
TEST(MatrixTest, Addition) {
    Matrix a(2, 2);
    Matrix b(2, 2);

    a[0][0] = 1; 
    a[0][1] = 2;
    a[1][0] = 3; 
    a[1][1] = 4;

    b[0][0] = 5; 
    b[0][1] = 6;
    b[1][0] = 7; 
    b[1][1] = 8;

    Matrix c = a + b;

    EXPECT_EQ(c.getRows(), 2u);
    EXPECT_EQ(c.getColumns(), 2u);
    EXPECT_EQ(c[0][0], 6);
    EXPECT_EQ(c[0][1], 8);
    EXPECT_EQ(c[1][0], 10);
    EXPECT_EQ(c[1][1], 12);
}

// Операторы сравнения
TEST(MatrixTest, EqualityAndNoteq) {
    Matrix a(2, 2);
    Matrix b(2, 2);

    a[0][0] = 1; 
    a[0][1] = 2;
    a[1][0] = 3; 
    a[1][1] = 4;

    b[0][0] = 1; 
    b[0][1] = 2;
    b[1][0] = 3; 
    b[1][1] = 4;

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    b[1][1] = 5;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// Сравнение матриц разных размеров
TEST(MatrixTest, DifferentSizesComparison) {
    Matrix a(2, 2);
    Matrix b(3, 3);
    Matrix c(2, 3);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

// Оператор вывода в поток
TEST(MatrixTest, OutputStream) {
    Matrix m(2, 3);
    m[0][0] = 1; m[0][1] = 2; m[0][2] = 3;
    m[1][0] = 4; m[1][1] = 5; m[1][2] = 6;

    std::stringstream ss;
    ss << m;

    EXPECT_EQ(ss.str(), "1 2 3\n4 5 6");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}