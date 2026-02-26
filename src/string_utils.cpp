#include <iostream>
#include <string>
#include "string_utils.h"

using namespace std;

namespace string_utils{
    void runStringOption(){
        /*
        Asks for an integer input to get the length of the string to make sure it is less than 20 characters
        Reads the string and copies it into the array
        Prints the string
        */
        int length;
        string str;
        cout << "Enter string length: ";
        cin >> length;
        if (length >= 20){
            cout << "Error: length must be less than 20" << endl;
            throw 1;
        }
        cin.ignore();
        cout << "Enter string: ";
        getline(cin, str);
        char arr[20];
        for(int i = 0; i < length; i++){
            arr[i] = str[i];
        }
        arr[length] = '\0';
        cout << "C-style string: " << arr << endl;
    }
}

