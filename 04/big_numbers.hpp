#ifndef BIG_NUMBERS_HPP
#define BIG_NUMBERS_HPP

#include <iostream>
#include <cstdint>

class BigInt {
private:
    char* digits;      
    size_t size;       
    bool is_negative;  

    void resize(size_t new_size);
    void remove_leading_zeros();
    void from_string(const char* str);
    
public:
    BigInt();
    BigInt(int32_t value);
    BigInt(const std::string& str);
    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;
    
    ~BigInt();
    
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;
    BigInt& operator=(int32_t value);
    
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator-() const;
    
    BigInt operator+(int32_t other) const;
    BigInt operator-(int32_t other) const;
    BigInt operator*(int32_t other) const;
    
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
};

#endif 