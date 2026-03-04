#include <iostream>
#include "../include/stats_mode.h"
#include <string>
#include <cctype>

namespace stats_mode{
    using namespace std;

    void run(int argc, char* argv[]){
        if (argc < 3)
            return;

        string str = argv[2];
        
        size_t len = str.size();
        cout << "Length " << len << endl;

        char unique[len];
        size_t unique_count = len;
        int unique_index = 0;

        for (char c : str){
            for (char d : unique){
                if (c == d)                    
                    unique_count--;
            }
            unique[unique_index] = c;
            unique_index++;
        }

        cout << "Unique " << unique_count << endl;

        bool alnum = true;

        for (char c : str){
            if (!(isalnum(c))){
                alnum = false;
            }
        }

        string alnum_text = "No";
        if (alnum)
            alnum_text = "Yes";

        cout << "Alphanumeric " << alnum_text << endl;

        string reversed = str;
        for (size_t i = 0; i < len / 2; ++i) {
            char temp = reversed[i];
            reversed[i] = reversed[len - 1 - i];
            reversed[len - 1 - i] = temp;
        }

        cout << "Reversed " << reversed << endl;
        
        string no_space = "";
        string no_space_r = "";
        for (char c : str){
            if (!(isspace(c)))
                no_space += tolower(c);
        }
        for (char c : reversed){
            if (!(isspace(c)))
                no_space_r += tolower(c);
        }

        cout << "Without spaces " << no_space << endl;
        cout << "withoutspaces" << no_space << endl;

        bool pal = false;
        if (no_space == no_space_r)
            pal = true;
        string pal_text = "No";
        if (pal)
            pal_text = "Yes";

        cout << "Palindrome " << pal_text << endl;

   }
}
