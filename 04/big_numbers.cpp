#include "big_numbers.hpp"
#include <cstring>
#include <algorithm>
#include <stdexcept>

// Вспомогательные функции
static int char_to_digit(char c) {
    return c - '0';
}

static char digit_to_char(int d) {
    return d + '0';
}

BigInt::BigInt() : digits(nullptr), size(0), is_negative(false) {
    resize(1);
    digits[0] = 0;
}

BigInt::BigInt(int32_t value) : digits(nullptr), size(0), is_negative(value < 0) {
    uint32_t abs_value = std::abs(value);
    uint32_t temp = abs_value;
    size_t num_digits = 0;
    do {
        num_digits++;
        temp /= 10;
    } while (temp > 0);
    
    resize(num_digits);
    
    temp = abs_value;
    for (size_t i = 0; i < size; ++i) {
        digits[i] = temp % 10;
        temp /= 10;
    }
}

BigInt::BigInt(const std::string& str) : digits(nullptr), size(0), is_negative(false) {
    from_string(str.c_str());
}

BigInt::BigInt(const BigInt& other) 
    : digits(nullptr), size(0), is_negative(other.is_negative) {
    resize(other.size);
    std::memcpy(digits, other.digits, size);
}

BigInt::BigInt(BigInt&& other) noexcept 
    : digits(other.digits), size(other.size), is_negative(other.is_negative) {
    other.digits = nullptr;
    other.size = 0;
    other.is_negative = false;
}

BigInt::~BigInt() {
    delete[] digits;
}

// Изменение размера массива цифр
void BigInt::resize(size_t new_size) {
    char* new_digits = new char[new_size]();
    
    if (digits) {
        size_t copy_size = std::min(size, new_size);
        std::memcpy(new_digits, digits, copy_size);
        delete[] digits;
    }
    
    digits = new_digits;
    size = new_size;
}

// Удаление ведущих нулей
void BigInt::remove_leading_zeros() {
    size_t new_size = size;
    while (new_size > 1 && digits[new_size - 1] == 0) {
        new_size--;
    }
    
    if (new_size != size) {
        resize(new_size);
    }

    if (size == 1 && digits[0] == 0) {
        is_negative = false;
    }
}

// Парсинг строки
void BigInt::from_string(const char* str) {
    if (str == nullptr || str[0] == '\0') {
        resize(1);
        digits[0] = 0;
        return;
    }
    
    size_t start = 0;
    is_negative = false;

    if (str[0] == '-') {
        is_negative = true;
        start = 1;
    } 
    else if (str[0] == '+') {
        start = 1;
    }

    while (str[start] == '0' && str[start + 1] != '\0') {
        start++;
    }

    size_t len = std::strlen(str);
    if (start >= len) {
        resize(1);
        digits[0] = 0;
        is_negative = false;
        return;
    }
    
    size_t num_digits = len - start;
    resize(num_digits);
    
    for (size_t i = 0; i < num_digits; ++i) {
        char c = str[len - 1 - i];
        if (c < '0' || c > '9') {
            throw std::invalid_argument("Invalid character in BigInt string");
        }
        digits[i] = char_to_digit(c);
    }
    
    remove_leading_zeros();
}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this != &other) {
        is_negative = other.is_negative;
        resize(other.size);
        std::memcpy(digits, other.digits, size);
    }
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this != &other) {
        delete[] digits;
        
        digits = other.digits;
        size = other.size;
        is_negative = other.is_negative;
        
        other.digits = nullptr;
        other.size = 0;
        other.is_negative = false;
    }
    return *this;
}

BigInt& BigInt::operator=(int32_t value) {
    *this = BigInt(value);
    return *this;
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (is_negative != other.is_negative) {
        BigInt positive = is_negative ? other : *this;
        BigInt negative = is_negative ? *this : other;
        positive.is_negative = false;
        negative.is_negative = false;
        return positive - negative;
    }
    
    BigInt result;
    size_t max_size = std::max(size, other.size) + 1;
    result.resize(max_size);
    result.is_negative = is_negative;
    
    int carry = 0;
    for (size_t i = 0; i < max_size; ++i) {
        int sum = carry;
        if (i < size) sum += digits[i];
        if (i < other.size) sum += other.digits[i];
        
        result.digits[i] = sum % 10;
        carry = sum / 10;
    }
    
    result.remove_leading_zeros();
    return result;
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (is_negative != other.is_negative) {
        BigInt positive = is_negative ? other : *this;
        BigInt negative = is_negative ? *this : other;
        positive.is_negative = false;
        negative.is_negative = false;
        BigInt result = positive + negative;
        result.is_negative = is_negative;
        return result;
    }
    
    bool this_is_larger = true;
    if (size != other.size) {
        this_is_larger = (size > other.size);
    } else {
        for (size_t i = size; i > 0; --i) {
            if (digits[i - 1] != other.digits[i - 1]) {
                this_is_larger = (digits[i - 1] > other.digits[i - 1]);
                break;
            }
        }
    }
    
    const BigInt& larger = this_is_larger ? *this : other;
    const BigInt& smaller = this_is_larger ? other : *this;
    
    BigInt result;
    result.resize(larger.size);
    result.is_negative = (is_negative && this_is_larger) || (!is_negative && !this_is_larger);
    
    int borrow = 0;
    for (size_t i = 0; i < larger.size; ++i) {
        int diff = larger.digits[i] - borrow;
        if (i < smaller.size) {
            diff -= smaller.digits[i];
        }
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.digits[i] = diff;
    }
    
    result.remove_leading_zeros();
    return result;
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt result;
    result.resize(size + other.size);
    result.is_negative = (is_negative != other.is_negative);
    
    for (size_t i = 0; i < size; ++i) {
        int carry = 0;
        for (size_t j = 0; j < other.size; ++j) {
            int remainder = digits[i] * other.digits[j] + result.digits[i + j] + carry;
            result.digits[i + j] = remainder % 10;
            carry = remainder / 10;
        }
        
        if (carry > 0) {
            result.digits[i + other.size] += carry;
        }
    }
    
    result.remove_leading_zeros();
    return result;
}

BigInt BigInt::operator-() const {
    BigInt result = *this;
    if (result.size > 1 || result.digits[0] != 0) {
        result.is_negative = !result.is_negative;
    }
    return result;
}

BigInt BigInt::operator+(int32_t other) const {
    return *this + BigInt(other);
}

BigInt BigInt::operator-(int32_t other) const {
    return *this - BigInt(other);
}

BigInt BigInt::operator*(int32_t other) const {
    return *this * BigInt(other);
}

bool BigInt::operator==(const BigInt& other) const {
    if (is_negative != other.is_negative || size != other.size) {
        return false;
    }
    
    for (size_t i = 0; i < size; ++i) {
        if (digits[i] != other.digits[i]) {
            return false;
        }
    }
    
    return true;
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
    if (is_negative != other.is_negative) {
        return is_negative;
    }
    
    if (size != other.size) {
        return (size < other.size) != is_negative;
    }
    
    for (size_t i = size; i > 0; --i) {
        if (digits[i - 1] != other.digits[i - 1]) {
            return (digits[i - 1] < other.digits[i - 1]) != is_negative;
        }
    }
    
    return false;
}

bool BigInt::operator<=(const BigInt& other) const {
    return *this < other || *this == other;
}

bool BigInt::operator>(const BigInt& other) const {
    return !(*this <= other);
}

bool BigInt::operator>=(const BigInt& other) const {
    return !(*this < other);
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    if (num.is_negative && (num.size > 1 || num.digits[0] != 0)) {
        os << '-';
    }
    
    for (size_t i = num.size; i > 0; --i) {
        os << digit_to_char(num.digits[i - 1]);
    }
    return os;
}