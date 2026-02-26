#include <iostream>
#include "integer_utils.h"

using namespace std;

namespace integer_utils{

    void runIntegerOption(){
        int a;
        int b;
        cout << "Enter first integer: "; 
        cin >> a;
        cout << "Enter second integer: ";
        cin >> b;
        if (b == 0){
            cout << "Error: division by zero" << endl;
            return;
        } else {
            cout << "Result: " << a/b << endl;
        }
        // single line comment for step 7: basically divides two inputs then increments the first input by 2
        cout << "After post-increment: " << a++ << endl;
        cout << "After pre-increment: " << ++a << endl;
    }
}

