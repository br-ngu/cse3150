#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "../include/audit_mode.h"
#include "../include/validation.h"

using namespace std;

void process_file(string input, string output){
    ifstream in_file(input);
    if (!in_file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    ofstream out_file(output, ios::app);
    if (!out_file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }
    
    char i_delim;
    char o_delim;

    if (input.substr(input.size() - 4) == ".csv"){
        i_delim = ',';
    } else {
        i_delim = '\t';
    }

    if (output.substr(output.size() - 4) == ".csv"){
        o_delim = ',';
    } else{
        o_delim = '\t';
    }


    string line, username, email, password;

    while (getline(in_file, line)) {
        if (line.empty()) 
            continue;

        stringstream ss(line);
        
        getline(ss, username, i_delim);
        getline(ss, email, i_delim);
        getline(ss, password);

        if (!is_valid_password(password)) {
            out_file << username << o_delim << email << o_delim << password << endl;
        }
    }
}



void run_menu(){
    int choice;

    do{
        cout << "1. Check a single password" << endl;
        cout << "2. Process a TSV/CSV file" << endl;
        cout << "3. Quit" << endl;

        cin >> choice;
        cin.ignore();
        if (choice == 1){
            string password;
            getline(cin, password);
            if (is_valid_password(password)){
                cout << "Valid" << endl;
            } else {
                cout << "Invalid" << endl;
            }
        } if (choice == 2){
            string i;
            string o;
            getline(cin, i);
            getline(cin, o);
            process_file(i, o);

        }

    } while (choice != 3);
}
