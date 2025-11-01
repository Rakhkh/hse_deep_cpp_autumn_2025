#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <stdexcept>
#include <cstdint>

class Matrix {
private:
    size_t rows_;
    size_t cols_;
    int32_t* data_;

    class Item {
        size_t rows_i {0};
        size_t cols_i {0};
        Matrix* data_i {nullptr};
    public:
        Item(Matrix* data_i, size_t rows_i);

        operator int32_t() const;
        int32_t operator=(int32_t a);
        Item& operator[](size_t cols);
    };

    class ConstItem {
        size_t rows_i {0};
        size_t cols_i {0};
        const Matrix* data_i {nullptr};
    public:
        ConstItem(const Matrix* data_i, size_t rows_i);

        operator int32_t() const;
        ConstItem& operator[](size_t cols);
    };

public:
    Matrix(int rows, int cols);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    ~Matrix();

    size_t getRows() const;
    size_t getColumns() const;

    Matrix& operator*=(int32_t value);
    Matrix& operator+=(const Matrix& other);
    Matrix operator+(const Matrix& other) const;

    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
    
    Item operator[](size_t rows);
    ConstItem operator[](size_t rows) const;
};

std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

#endif