#pragma once
#include "abstract_decimal.h"
#include <iostream>
#include <stdexcept>

class Fraction : public Decimal {
private:
    int* numer_;
    int* denom_;

    void check_null() const {
        if (!numer_ || !denom_) {
            throw std::runtime_error("can't dereference null pointers");
        }
    }

public:
    Fraction();
    Fraction(int n, int d);
    Fraction(const Fraction& other);
    Fraction(Fraction&& other) noexcept;

    ~Fraction();

    Fraction& operator=(const Fraction& other);
    Fraction& operator=(Fraction&& other) noexcept;

    double convertToDecimal() const override;

    friend Fraction operator+(const Fraction& lhs, const Fraction& rhs);
    friend Fraction operator*(const Fraction& lhs, const Fraction& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Fraction& frac);
};
