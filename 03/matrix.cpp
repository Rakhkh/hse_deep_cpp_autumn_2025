#include "matrix.hpp"

Matrix::Matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) 
        throw std::out_of_range("");
    
    rows_ = static_cast<size_t>(rows);
    cols_ = static_cast<size_t>(cols);
    
    data_ = new int32_t[rows_ * cols_];
    for (size_t i = 0; i < rows_ * cols_; ++i) 
        data_[i] = 0;
}

Matrix::Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_) {
    data_ = new int32_t[rows_ * cols_];
    for (size_t i = 0; i < rows_ * cols_; ++i) 
        data_[i] = other.data_[i];
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        delete[] data_;
        
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = new int32_t[rows_ * cols_];
        for (size_t i = 0; i < rows_ * cols_; ++i) 
            data_[i] = other.data_[i];
    }
    return *this;
}

Matrix::~Matrix() {
    delete[] data_;
}

Matrix::Item::Item(Matrix* data_i, size_t rows_i) : rows_i(rows_i), data_i(data_i)
    {}

Matrix::Item::operator int32_t() const {
    return data_i->data_[rows_i * data_i->cols_ + cols_i];
}

int32_t Matrix::Item::operator=(int32_t a) {
    data_i->data_[rows_i * data_i->cols_ + cols_i] = a;
    return a;
}

Matrix::Item& Matrix::Item::operator[](size_t cols) {
    if (cols >= data_i->cols_)
        throw std::out_of_range("");
    cols_i = cols;
    return *this;
}

Matrix::ConstItem::ConstItem(const Matrix* data_i, size_t rows_i) : rows_i(rows_i), data_i(data_i)
    {}

Matrix::ConstItem::operator int32_t() const{
    return data_i->data_[rows_i * data_i->cols_ + cols_i];
}

Matrix::ConstItem& Matrix::ConstItem::operator[](size_t cols) {
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

Matrix::ConstItem Matrix::operator[](size_t rows) const {
    if (rows >= rows_) 
        throw std::out_of_range("");
    return ConstItem(this, rows);
}

size_t Matrix::getRows() const {
    return rows_;
}

size_t Matrix::getColumns() const {
    return cols_;
}

Matrix& Matrix::operator*=(int32_t value) {
    for (size_t i = 0; i < rows_ * cols_; ++i) 
        data_[i] *= value;
    return *this;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) 
        throw std::invalid_argument("Matrix dimensions must match for addition");
    
    Matrix result(rows_, cols_);
    for (size_t i = 0; i < rows_ * cols_; ++i) 
        result.data_[i] = data_[i] + other.data_[i];
    return result;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) 
        throw std::invalid_argument("Matrix dimensions must match for addition");

    for (size_t i = 0; i < rows_ * cols_; ++i) 
        data_[i] += other.data_[i];
    return *this;
}

bool Matrix::operator==(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) 
        return false;
    
    for (size_t i = 0; i < rows_ * cols_; ++i) 
        if (data_[i] != other.data_[i]) 
            return false;

    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getColumns(); ++j) {
            os << matrix[i][j];
            if (j < matrix.getColumns() - 1) 
                os << " ";
        }
        if (i < matrix.getRows() - 1) 
            os << std::endl;
    }
    return os;
}