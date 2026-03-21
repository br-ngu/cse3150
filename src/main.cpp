#include <iostream>
#include "../include/audit_mode.h"

using namespace std;

int main(int argc, char* argv[]){
    if (argc == 3){
        process_file(argv[1], argv[2]);    
    } else {
        run_menu();
    }

    return 0;
}
