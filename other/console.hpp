#pragma once

#include<string>

class Console {
public:
    static void ShowError(std::string message, bool exit = true);
    static void ShowError(std::string methodName, std::string message, bool exit = true);
private:
    Console() = delete;
};