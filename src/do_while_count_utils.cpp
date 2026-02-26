#include <iostream>
#include "do_while_count_utils.h"

using namespace std;

namespace do_while_count_utils{
    void runDoWhileCount(){
        int x;
        int arr[5] = {1,2,3,4,5};
        do{
            cout << "Enter a number between 1 and 5: ";
            cin >> x;
        } while (x < 1 || x > 5);
        for (int i : arr){
            cout << "Value: " << i << endl;
            if (i == x){
                break;
            }
        }
    }
}

