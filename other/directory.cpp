#include "directory.hpp"
#include<string>
#include<sys/stat.h>
using std::string;

bool Directory::exist(string dirName) {
    struct stat statBuf;
    if(stat(dirName.c_str(), &statBuf) == 0) 
        return true;
    else 
        return false;
}

void Directory::create(string dirName) {
    mkdir(dirName.c_str(), 0777);
}