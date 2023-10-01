#pragma once
#include<string>
#include<vector>
using std::string, std::vector;

class CommandLineArgument{ 
public:
    struct ArgSet {
        string option;
        string value;
        bool hasValue;
    };

    CommandLineArgument(int argc, char* argv[]);
    void showArguments() const;
    bool existOption(string option) const;
    int getValueInt(string optionName) const;
    double getValueDouble(string optionName) const;
    string getValueString(string optionName) const;
    bool getValueBool(string optionName) const;

private:
    vector<ArgSet> _argSet;

private:
    CommandLineArgument() = delete;
};