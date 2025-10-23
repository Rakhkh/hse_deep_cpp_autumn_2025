#include <gtest/gtest.h>
#include "matrix.hpp"
#include <stdexcept>

// Конструктор и базовые методы
TEST(MatrixTest, Constructor) {
    Matrix m(2, 3);
    EXPECT_EQ(m.rows(), 2u);
    EXPECT_EQ(m.columns(), 3u);
    EXPECT_EQ(m[0][0], 0);
    EXPECT_EQ(m[1][2], 0);
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

// Операторы сравнения
TEST(MatrixTest, EqualityAndNoteq) {
    Matrix a(2, 2);
    Matrix b(2, 2);

    a[0][0] = 1; a[0][1] = 2;
    a[1][0] = 3; a[1][1] = 4;

    b[0][0] = 1; b[0][1] = 2;
    b[1][0] = 3; b[1][1] = 4;

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    b[1][1] = 5;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}


// Пустая матрица
TEST(MatrixTest, EmptyMatrix) {
    Matrix m(0, 0);
    EXPECT_EQ(m.rows(), 0u);
    EXPECT_EQ(m.columns(), 0u);
    EXPECT_THROW(m[0][0], std::out_of_range);  
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}