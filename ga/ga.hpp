#pragma once

#include <fstream>
class Parameter;
class Group;
class ICrosser;

class GeneticAlgorithm {
public:
    GeneticAlgorithm(Parameter param);
    ~GeneticAlgorithm();

    void step();
    bool isFinished() const;
    void showParameter() const;
    void recordParameter(std::ofstream& ofs) const;
    void saveEdgesFile(std::string filename) const;
    double bestAspl() const;
    static void showHeader();

private:
    Group* _group = nullptr;
    const int _MAX_GENERATION;
    int _generation = 0;
    int _convergeCount = 0;
    bool _converged = false;

    ICrosser* generateCrossover(Parameter param);
};