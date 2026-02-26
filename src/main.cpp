#include <iostream>
#include "../include/integer_utils.h"
#include "../include/string_utils.h"
#include "../include/grade_utils.h"
#include "../include/while_count_utils.h"
#include "../include/do_while_count_utils.h"

using namespace std;

int main(){
    int choice;

    while (true){
        cout << "1. Integer operations\n2. Character arrays and C-style strings\n3. Grade evaluation\n4. While-loop counting\n5. Do-while and range-based for counting\n6. Quit" << endl;
        if (!(cin >> choice) || choice < 1 || choice > 6){
            cin.clear();
            cin.ignore();
            continue;
        }

        try {
            switch (choice) {
                case 1:
                    integer_utils::runIntegerOption();
                    break;
                case 2:
                    string_utils::runStringOption();
                    break;
                case 3:
                    grade_utils::calculateGrade();
                    break;
                case 4:
                    while_count_utils::runWhileCount();
                    break;
                case 5:
                    do_while_count_utils::runDoWhileCount();
                    break;
                case 6:
                    std::cout << "Goodbye!" << std::endl;
                    return 0;
                default:
                    break;
            }
        }
        catch(int e){
            return 1;
        }
    }


    return 0;
}
