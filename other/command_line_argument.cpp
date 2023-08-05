#include "command_line_argument.hpp"
#include<iostream>
#include<ios>
#include<iomanip>
using std::cout, std::endl, std::left, std::setw;

CommandLineArgument::CommandLineArgument(int argc, char* argv[]) {
    int state = 0;
    string option = "";
    string value = "";

    for(int i = 1; i < argc; ++i) {
        if(state == 0) {
            if(argv[i][0] == '-') {
                option = string(argv[i]);
                state = 1;
            }
            else {
                value = string(argv[i]);
                ArgSet argSet = {"", value};
                this->_argSet.push_back(argSet);

                option = "";
                value = "";
                state = 0;
            }
        }
        else if(state == 1) { //after reading option
            if(argv[i][0] == '-') {
                ArgSet argSet = {option, ""};
                this->_argSet.push_back(argSet);

                option = string(argv[i]);
                value = "";
                state = 1;
            }
            else {
                value = string(argv[i]);
                ArgSet argSet = {option, value};
                this->_argSet.push_back(argSet);

                option = "";
                value = "";
                state = 0;
            }
        }
    }

    if(state == 1) {
        ArgSet argSet = {option, ""};
        this->_argSet.push_back(argSet);
    }
}

void CommandLineArgument::showArguments() {
    const int NUM_ARG = _argSet.size();

    for(int i = 0; i < NUM_ARG; ++i) {
        if(_argSet[i].option == "") continue;
        cout << left << setw(10) << _argSet[i].option << " " << _argSet[i].value << endl;
    }

    for(int i = 0; i < NUM_ARG; ++i) {
        if(_argSet[i].option != "") continue;
        cout << _argSet[i].value << endl;
    }
}

bool CommandLineArgument::existOption(string option) {
    const int NUM_ARG = _argSet.size();
    for(int i = 0; i < NUM_ARG; ++i) {
        if(_argSet[i].option == option) return true;
    }
    return false;
}

string CommandLineArgument::getValue(string option) {
    const int NUM_ARG = _argSet.size();
    for(int i = 0; i < NUM_ARG; ++i) {
        if(_argSet[i].option == option) return _argSet[i].value;
    }
    return "";
}