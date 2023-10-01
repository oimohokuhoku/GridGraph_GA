#include "command_line_argument.hpp"
#include<iostream>
#include<ios>
#include<iomanip>
#include<cstring>
#include<algorithm>
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
                ArgSet argSet = {"", value, true};
                this->_argSet.push_back(argSet);

                option = "";
                value = "";
                state = 0;
            }
        }
        else if(state == 1) { //after reading option
            if(argv[i][0] == '-') {
                ArgSet argSet = {option, "", false};
                this->_argSet.push_back(argSet);

                option = string(argv[i]);
                value = "";
                state = 1;
            }
            else {
                value = string(argv[i]);
                ArgSet argSet = {option, value, true};
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

void CommandLineArgument::showArguments() const {
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

bool CommandLineArgument::existOption(string option) const {
    const int NUM_ARG = _argSet.size();
    for(int i = 0; i < NUM_ARG; ++i) {
        if(_argSet[i].option == option) return true;
    }
    return false;
}

int CommandLineArgument::getValueInt(string optionName) const {
    return stoi(this->getValueString(optionName));
}

double CommandLineArgument::getValueDouble(string optionName) const {
    return stod(this->getValueString(optionName));
}

string CommandLineArgument::getValueString(string optionName) const {
    const int NUM_ARG = _argSet.size();
    for(int i = 0; i < NUM_ARG; ++i) {
        if(_argSet[i].option == optionName) {
            if(_argSet[i].hasValue) return _argSet[i].value;

            cout << "Option \"" << optionName << "\" doesn't be designated argument" << endl;
            exit(1);
        }
    }

    cout << "Option \"" << optionName << "\" doesn't exist" << endl;
    exit(1); return "";
}

bool CommandLineArgument::getValueBool(string optionName) const {
    string value = getValueString(optionName);

    bool isNumber = std::all_of(value.begin(), value.end(), [](char c) { return std::isdigit(c); });
    if(isNumber) return (value != "0");

    transform(value.begin(), value.end(), value.begin(), ::tolower);
    if(value == "true") return true;
    if(value == "false") return false;

    cout << optionName << " " << value << " can't regard as bool" << endl;
    exit(0); return false;
}
