#pragma once
#include<string>
#include<vector>
using std::string, std::vector;

class CommandLineArgument{ 
public:
    struct ArgSet {
        string option;
        string value;
    };

    CommandLineArgument(int argc, char* argv[]);
    ~CommandLineArgument() = default;
    void showArguments();
    bool existOption(string option);
    string getValue(string option);

private:
    vector<ArgSet> _argSet;

private:
    CommandLineArgument() = delete;
};