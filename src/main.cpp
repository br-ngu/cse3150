#include <iostream>
#include <string>
#include "text_mode.h"
#include "add_mode.h"
#include "stats_mode.h"

using namespace std;

int main(int argc, char* argv[]){
    if (argc < 2) {
        cout << "Usage: ./analyzer <mode> [arguments]" << endl;
        return 1;
    }
    
    string arg = argv[1];

    int mode = 0;

    if (arg == "text"){
        mode = 1;
    } else if (arg == "add") {
        mode = 2;
    } else if (arg == "stats") {
        mode = 3;
    }

    switch(mode){
        case 1:
            text_mode::run(argc, argv);
            break;
        case 2:
            add_mode::run(argc, argv);
            break;
        case 3:
            stats_mode::run(argc, argv);
            break;
        default:
            cout << "Invalid mode" << endl;
            return 1;
    }

    return 0;
}
