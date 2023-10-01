#pragma once
#include <string>

class Directory {
public:
    static bool exist(std::string dirName);
    static void create(std::string dirName);
};