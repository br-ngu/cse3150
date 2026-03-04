#include <iostream>
#include "../include/add_mode.h"
#include <string>
#include <cctype>


namespace add_mode{
    
    using namespace std;

    int add(int a, int b){
        return a + b;
    }

    double add(double a, double b){
        return a + b;
    }

    std::string add(std::string a, std::string b){
        return a + b;
    }


    void run(int argc, char* argv[]){
        if (argc < 4)
            return;

        string arg1 = argv[2];
        string arg2 = argv[3];

        bool is_digit = true;
        bool use_double = false;

        size_t decimal1 = arg1.find('.');
        size_t decimal2 = arg2.find('.');
        if (decimal1 != string::npos || decimal2 != string::npos)
            use_double = true;

        for (char c : arg1){
            if (!(isdigit(c) || c == '.'))
                is_digit = false;
        }
    
        for (char c : arg2){
            if (!(isdigit(c) || c == '.'))
                is_digit = false;
        }

        string result;

        if (is_digit){
            if (use_double){
                double val1 = stod(arg1);
                double val2 = stod(arg2);
                double temp_result = add(val1, val2);
                result = to_string(temp_result);
            } else {
                int val1 = stoi(arg1);
                int val2 = stoi(arg2);
                int temp_result = add(val1, val2);
                result = to_string(temp_result);
            }
        } else {
            result = add(arg1, arg2);
        }

        cout << "Result: " << result << endl;
    }
}
