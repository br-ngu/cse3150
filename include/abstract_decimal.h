#pragma once
class Decimal {
public:
    virtual ~Decimal() = default;
    virtual double convertToDecimal() const = 0;
};
