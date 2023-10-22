#pragma once

#include<string>
using std::string;
class CommandLineArgument;

class Parameter {
public:
    Parameter(const string& srcFile);
    Parameter(const CommandLineArgument& arg);
    ~Parameter();
    void showParam() const;
    void writeParam(std::ofstream& ofs) const;
    string getProblemName() const;

    void setParam(const CommandLineArgument& arg, bool allRequired = false);
    void setNumColumn(int value);
    void setNumRow(int value);
    void setDegree(int value);
    void setMaxLength(int value);
    void setGroupSize(int value);
    void setNumChild(int value);
    void setMutateIndiv(double value);
    void setMutateGene(double value);
    void setMaxGeneration(int value);
    void setCrossover(string& value);
    void setTwxNumLoop(int value);
    void setTwxPerimeterSelectRate(double value);
    void setGxMaxNumLS(string value);
    void setNumInitLocalSearch(string value);
    

    inline int numColumn() const { return _numColumn; }
    inline int numRow() const { return _numRow; }
    inline int degree() const { return _degree; }
    inline int maxLength() const { return _maxLength; }
    inline int groupSize() const { return _groupSize; }
    inline int numChild() const { return _numChild; }
    inline double mutateIndiv() const { return _mutateIndiv; }
    inline double mutateGene() const { return _mutateGene; }
    inline int maxGeneration() const { return _maxGeneration; }
    inline const string& crossover() const { return _crossover; }
    inline int twxNumLoop() const { return _twx_numLoop; }
    inline double twxPerimeterSelectRate() const { return _twx_PerimeterSelectRate; }
    inline double gxMaxNumLS() const { return _gx_MaxNumLS; }
    inline int numInitLocalSearch() const { return _numInitLS; }

private:
    int _numColumn;
    int _numRow;
    int _degree;
    int _maxLength;
    int _groupSize;
    int _numChild;
    double _mutateIndiv;
    double _mutateGene;
    int _maxGeneration;
    string _crossover;
    int _twx_numLoop;
    double _twx_PerimeterSelectRate;
    int _gx_MaxNumLS;
    int _numInitLS;

    bool _fromArg;
    string _srcFile;
};