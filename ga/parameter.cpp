#include "parameter.hpp"
#include "../other/command_line_argument.hpp"
#include<iostream>
#include<fstream>
#include<algorithm>
#include <string>
using namespace std;

Parameter::Parameter(const string& srcFile) {
    _srcFile = srcFile;
    ifstream ifs(srcFile);
    if(!ifs) {
        cout << "source file can't be found" << endl;
        exit(0);
    }

    int doLS;
    ifs >> _numColumn >> _numRow >> _degree >> _maxLength;
    ifs >> _maxGeneration >> _groupSize >> _numChild >> _mutateIndiv >> _mutateGene;
    ifs >> _crossover >> _twx_numLoop >> _twx_PerimeterSelectRate;
    ifs >> doLS;

    if(doLS == 0) _doLocalSearch = false;
    else          _doLocalSearch = true;

    this->_fromArg = false;
}

Parameter::Parameter(const CommandLineArgument& arg) {
    setParam(arg, true);
    this->_fromArg = true;
}

Parameter::~Parameter() {
    if(_fromArg) return;

    int doLS;
    if(_doLocalSearch) doLS = 1;
    else              doLS = 0;

    ofstream ofs(_srcFile);
    ofs << _numColumn << " " << _numRow << " " << _degree << " " << _maxLength << endl;
    ofs << _maxGeneration << " " << _groupSize << " " << _numChild << " " << _mutateIndiv << " " << _mutateGene << endl;
    ofs << _crossover << " " << _twx_numLoop << " " << _twx_PerimeterSelectRate << endl;
    ofs << doLS << endl;
}

void Parameter::showParam() const {
    cout << "[Grid Graph]" << endl;
    cout << "Node   : " << _numColumn << "x" << _numRow << endl;
    cout << "Degree : " << _degree << endl; 
    cout << "Length : " << _maxLength << endl; 
    cout << endl;
    cout << "[GA]" << endl;
    cout << "Max Generation   : " << _maxGeneration << endl;
    cout << "Population       : " << _groupSize << endl;
    cout << "Number of childs : " << _numChild << endl;
    cout << "Mutate probably  : " << _mutateIndiv << "(Individual), " << _mutateGene << "(Gene)" << endl;
    cout << "Crossover        : " << _crossover << endl;
    if(_doLocalSearch)
        cout << "Local Search     : enabled" << endl;
    else
        cout << "Local Search     : disabled" << endl;

    if(_crossover.compare("twx") == 0) {
        cout << endl;
        cout << "[TWX]" << endl;
        cout << "Number of Inheritance : " << _twx_numLoop << endl;
        cout << "Rate of perimeter nodes selection : " << _twx_PerimeterSelectRate << endl;
    }
}

void Parameter::writeParam(ofstream& ofs) const {
    ofs << "column," << _numColumn << endl;
    ofs << "row," << _numRow << endl;
    ofs << "degree," << _degree << endl; 
    ofs << "length," << _maxLength << endl; 
    ofs << endl;
    ofs << "max generation," << _maxGeneration << endl;
    ofs << "population," << _groupSize << endl;
    ofs << "number of offspring," << _numChild << endl;
    ofs << "mutate probably(individual)," << _mutateIndiv << endl;
    ofs << "mutate probably(gene)," << _mutateGene << endl;
    ofs << "crossover," << _crossover << endl;
    if(_doLocalSearch)
        ofs << "local Search,enabled" << endl;
    else
        ofs << "local Search,disable" << endl;

    if(_crossover.compare("twx") == 0) {
        ofs << endl;
        ofs << "number of inheritance : " << _twx_numLoop << endl;
        ofs << "rate of perimeter nodes selection : " << _twx_PerimeterSelectRate << endl;
    }
}

string Parameter::getProblemName() const {
    string result("");
    result += to_string(_numColumn) + "_";
    result += to_string(_numRow) + "_";
    result += to_string(_degree) + "_";
    result += to_string(_maxLength);
    return result;
}


void Parameter::setParam(const CommandLineArgument& arg, bool allRequired) {
    if(arg.existOption("-width") || allRequired)
        setNumColumn(arg.getValueInt("-width"));

    if(arg.existOption("-height") || allRequired)
        setNumRow(arg.getValueInt("-height"));

    if(arg.existOption("-degree") || allRequired)
        setDegree(arg.getValueInt("-degree"));

    if(arg.existOption("-length") || allRequired)
        setMaxLength(arg.getValueInt("-length"));

    if(arg.existOption("-generation") || allRequired)
        setMaxGeneration(arg.getValueInt("-generation"));

    if(arg.existOption("-population") || allRequired)
        setGroupSize(arg.getValueInt("-population"));

    if(arg.existOption("-offspring") || allRequired) 
        setNumChild(arg.getValueInt("-offspring"));

    if(arg.existOption("-mutate_indiv") || allRequired)
        setMutateIndiv(arg.getValueDouble("-mutate_indiv"));

    if(arg.existOption("-mutate_gene") || allRequired)
        setMutateGene(arg.getValueDouble("-mutate_gene"));

    if(arg.existOption("-cross") || allRequired) {
        string value = arg.getValueString("-cross");
        setCrossover(value);
    }

    if(arg.existOption("-local_search") || allRequired) 
        setDoLocalSearch(arg.getValueBool("-local_search"));

    if(_crossover == "twx") {
        if(arg.existOption("-twx_loop") || allRequired)
            setTwxNumLoop(arg.getValueInt("-twx_loop"));

        if(arg.existOption("-twx_perimeter") || allRequired)
            setTwxPerimeterSelectRate(arg.getValueDouble("-twx_perimeter"));
    }
}

void Parameter::setNumColumn(int value) {
    if(value < 2) value = 2;
    _numColumn = value; 
}

void Parameter::setNumRow(int value) {
    if(value < 2) value = 2;
    _numRow = value; 
}

void Parameter::setDegree(int value) {
    if(value < 2) value = 2;
    _degree = value; 
}

void Parameter::setMaxLength(int value) {
    if(value < 2) value = 2;
    _maxLength = value; 
}

void Parameter::setGroupSize(int value) {
    if(value < 1) value = 1;
    _groupSize = value; 
}

void Parameter::setNumChild(int value) {
    if(value < 2) value = 2;
    _numChild = value;
}

void Parameter::setMutateIndiv(double value) {
    if(value < 0) value = 0;
    if(1 < value) value = 1;
    _mutateIndiv = value;
}

void Parameter::setMutateGene(double value) {
    if(value < 0) value = 0;
    if(1 < value) value = 1;
    _mutateGene = value;
}

void Parameter::setMaxGeneration(int value) {
    if(value < 2) value = 2;
    _maxGeneration = value;
}

void Parameter::setCrossover(string& value) {
    transform(value.begin(), value.end(), value.begin(), ::tolower);
    _crossover = value;
}

void Parameter::setTwxNumLoop(int value) {
    if(value < 1) value = 1;
    _twx_numLoop = value;
}

void Parameter::setTwxPerimeterSelectRate(double value) {
    if(value < 0) value = 0;
    if(1 < value) value = 1;
    _twx_PerimeterSelectRate = value;
}

void Parameter::setDoLocalSearch(bool value) {
    _doLocalSearch = value;
}
