#include <iostream>
#include <string>
#include <cctype>
#include "../include/validation.h"

bool is_valid_password(std::string password){
    bool valid = true;
    bool has_upper = false;
    bool has_non_alnum = false;

    if (password.size() < 8)
        return false;

    for (char c : password){
        if (std::isupper(c))
            has_upper = true;
        if (!std::isalnum(c))
            has_non_alnum = true;
    }

    valid = valid && has_upper && has_non_alnum;
    return valid;
}
