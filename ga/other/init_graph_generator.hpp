#pragma once

#include<climits>
#include<string>
class Parameter;
class Individual;
template<class T> class Collection;

class InitGraph {
public:
    static void generate(const Parameter& param, int numGeneratedGraph, int numMaxLocalSearch = INT_MAX);
    static void load(const Parameter& param, int numMaxLocalSearch, Collection<Individual>& storage);

private:
    static const std::string ROOT_DIRECTORY;
    static const int NUM_INIDIV_FILE;
    static inline std::string getDirectoryName(const Parameter& param, int numMaxLocalSearch);
};