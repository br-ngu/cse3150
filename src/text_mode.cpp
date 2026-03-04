#include <iostream>
#include <string>
#include <cctype>
#include "../include/text_mode.h"

using namespace std;

namespace text_mode{
    void run(int argc, char* argv[]){
        if (argc < 3)
            return;

        string str = argv[2];
    
        cout << "Original " << str << endl;

        size_t len = str.size();
        cout << "Length " << len << endl;

        int letters = 0;
        int digits = 0;
        int spaces = 0;
        int punctuation = 0;
        size_t first_space_idx = 0;

        for (char c : str){
            if (isalpha(c))
                letters++;
            if (isdigit(c))
                digits++;
            if (isspace(c))
                spaces++;
            if (spaces < 1){
                first_space_idx++;
            }
            if (ispunct(c))
                punctuation++;
        }

        cout << "Letters " << letters << endl;
        cout << "Digits " << digits << endl;
        cout << "Spaces " << spaces << endl;
        cout << "Punctuation " << punctuation << endl;

        string copy = str;

        for (size_t i = 0; i < len; i++){
            copy[i] = toupper(str[i]);
            str[i] = tolower(str[i]);
        }

        cout << "Uppercase " << copy << endl;
        cout << "Lowercase " << str << endl;

        size_t test = str.find("test");
        string find_result = "Yes";
        if (test == string::npos)
            find_result = "No";
        cout << "Contains 'test' " << find_result << endl;

        if (first_space_idx != len){
            string sub = str.substr(0,first_space_idx);
            cout << "First word " << sub << endl;
        }
    }
}

