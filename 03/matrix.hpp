// На GitHub не было примера того, как должен выглядеть сам класс. ДЗ делалось по слайду из претентации :p

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <stdexcept>
#include <cstdint>

class Matrix {
private:
    size_t rows_;
    size_t cols_;
    std::vector<int32_t> data_;

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

public:
    Matrix(size_t rows, size_t cols);


    size_t rows() const;
    size_t columns() const;

    Matrix& operator*=(int32_t value);

    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
    Item operator[](size_t rows);
};

#endif