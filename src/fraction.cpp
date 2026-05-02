#include "fraction.h"
#include <iostream>

Fraction::Fraction() {
    std::cout << "I am in the default constructor" << std::endl;
    numer_ = new int(0);
    denom_ = new int(1);
}

Fraction::Fraction(int n, int d) {
    std::cout << "I am in the custom constructor" << std::endl;
    if (d == 0) {
        throw std::runtime_error("Divide by zero error");
    }
    numer_ = new int(n);
    denom_ = new int(d);
}

Fraction::Fraction(const Fraction& other) {
    std::cout << "I am in the copy constructor" << std::endl;
    other.check_null();
    numer_ = new int(*(other.numer_));
    denom_ = new int(*(other.denom_));
}

Fraction::Fraction(Fraction&& other) noexcept {
    std::cout << "I am in the move constructor" << std::endl;
    numer_ = other.numer_;
    denom_ = other.denom_;
    
    other.numer_ = nullptr;
    other.denom_ = nullptr;
}

Fraction::~Fraction() {
    std::cout << "I am in the destructor" << std::endl;
    delete numer_;
    delete denom_;
}

Fraction& Fraction::operator=(const Fraction& other) {
    std::cout << "I am in the assignment operator (lvalue)" << std::endl;
    if (this != &other) {
        other.check_null();
        
        delete numer_;
        delete denom_;
        
        numer_ = new int(*(other.numer_));
        denom_ = new int(*(other.denom_));
    }
    return *this;
}

Fraction& Fraction::operator=(Fraction&& other) noexcept {
    std::cout << "I am in the assignment operator (rvalue)" << std::endl;
    if (this != &other) {
        delete numer_;
        delete denom_;
        
        numer_ = other.numer_;
        denom_ = other.denom_;
        
        other.numer_ = nullptr;
        other.denom_ = nullptr;
    }
    return *this;
}

double Fraction::convertToDecimal() const {
    std::cout << "I am in the convertToDecimal" << std::endl;
    check_null();
    return static_cast<double>(*numer_) / static_cast<double>(*denom_);
}

Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
    std::cout << "I am in the plus operator" << std::endl;
    lhs.check_null();
    rhs.check_null();
    
    int n = (*(lhs.numer_) * *(rhs.denom_)) + (*(rhs.numer_) * *(lhs.denom_));
    int d = *(lhs.denom_) * *(rhs.denom_);
    
    return Fraction(n, d);
}

Fraction operator*(const Fraction& lhs, const Fraction& rhs) {
    std::cout << "I am in the multiplication operator" << std::endl;
    lhs.check_null();
    rhs.check_null();
    
    int n = *(lhs.numer_) * *(rhs.numer_);
    int d = *(lhs.denom_) * *(rhs.denom_);
    
    return Fraction(n, d);
}

std::ostream& operator<<(std::ostream& os, const Fraction& frac) {
    std::cout << "I am in the << operator" << std::endl;
    frac.check_null();
    os << *(frac.numer_) << "/" << *(frac.denom_);
    return os;
}
