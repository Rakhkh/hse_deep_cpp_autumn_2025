#include "matrix.hpp"

Matrix::Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows * cols, 0) 
    { }

Matrix::Item::Item(Matrix* data_i, size_t rows_i) : data_i(data_i), rows_i(rows_i)
    {}

Matrix::Item::operator int32_t() const{
    return data_i->data_[rows_i * data_i->cols_ + cols_i];
}

int32_t Matrix::Item::operator=(int32_t a) {
    data_i->data_[rows_i * data_i->cols_+ cols_i] = a;
    return a;
}

Matrix::Item& Matrix::Item::operator[](size_t cols) {
    if (cols >= data_i->cols_)
        throw std::out_of_range("");
    cols_i = cols;
    return *this;
}

Matrix::Item Matrix::operator[](size_t rows) {
    if (rows >= rows_) 
        throw std::out_of_range("");
    return Item(this, rows);
}

size_t Matrix::rows() const {
    return rows_;
}

size_t Matrix::columns() const {
    return cols_;
}

Matrix& Matrix::operator*=(int32_t value) {
    for (auto& elem : data_) {
        elem *= value;
    }
    return *this;
}

bool Matrix::operator==(const Matrix& other) const {
    return rows_ == other.rows_ && cols_ == other.cols_ && data_ == other.data_;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}