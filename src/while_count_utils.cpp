#include <iostream>
#include "while_count_utils.h"

using namespace std;

namespace while_count_utils{
    void runWhileCount(){
        int num;
        cout << "Enter a number to count to: ";
        cin >> num;
        while (num > 10) {
            cout << "I'm programmed to only count up to 10!" << endl;
            cout << "Enter a number to count to: ";
            cin >> num;
        }
        int x = 1;
        cout << "" << endl; // for some reason the test doesn't read the first number without this
        while (x <= num){
            if (x != 5){
                cout << x << endl;
            }
            x++;
        }
    }
}

