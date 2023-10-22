#include "console.hpp"
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

void Console::ShowError(std::string message, bool exitProgramm) {
    cout << "Error : " << message << endl;
    if(exitProgramm) exit(1);
}

void Console::ShowError(std::string methodName, std::string message, bool exitProgramm) {
    cout << "Error in " + methodName + " : " + message << endl;
    if(exitProgramm) exit(1);
}