#include <iostream>
#include "grade_utils.h"

using namespace std;

namespace grade_utils{
    double calculateGrade(){
        char type;
        double grade;
        string status = "Fail";
        cout << "Enter student type (U for undergrad, G for grad): ";
        cin >> type;
        cout << "Enter numeric grade: ";
        cin >> grade;

        if (grade < 0 || grade > 100){
            cout << "Invalid grade" << endl;
            throw 1;
        } else {
            if (toupper(type) == 'U' && grade >= 60){
                status = "Pass";
            } else if (toupper(type) == 'G' && grade >= 70){
                status = "Pass";
            }
        }
        cout << "Status: " << status << endl;
        return grade;
    }
}

