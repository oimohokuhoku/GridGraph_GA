#include<iostream>
#include<sys/stat.h>
#include<cstdio>
#include<iomanip>
#include<fstream>
#include<climits>
#include "ga/ga.hpp"
#include "ga/parameter.hpp"
#include "other/random.hpp"
#include "other/command_line_argument.hpp"
#include "other/directory.hpp"
#include "ga/other/meta_observer.hpp"
#include "ga/other/init_graph_generator.hpp"
using namespace std;

static const string RESULT_DIR = "result";

void showHelp() {
    cout << "# When there is no argument, genetic algorithm is executed" << endl;

    cout << endl;
    cout << "# -repeat [int] : Repeat executing GA for the specified number of times" << endl;

    cout << endl;
    cout << "# -show_param : Show current parameter setting" << endl;

    cout << endl;
    cout << "# -set_param : Switch to setting parameter mode. ([e.g.]./grid -set_param -width 10 -mutate_indiv 0.01)" << endl;
    cout << "#    -width [int]  : Number of column in a graph" << endl;
    cout << "#    -height [int] : Number of row in a graph" << endl;
    cout << "#    -degree [int] : Degree of a graph" << endl;
    cout << "#    -length [int] : Max edge length in a graph" << endl;
    cout << "#" << endl;
    cout << "#    -generation [int] : Max generation in GA" << endl;
    cout << "#    -population [int] : Individual population of a group" << endl;
    cout << "#    -offspring [int]  : Number of offsprings created with each crossover" << endl;
    cout << "#    -mutate_indiv [double] : Mutate probably for each individual" << endl;
    cout << "#    -mutate_gene [double]  : Mutate probably for each gene" << endl;
    cout << "#    -cross [string] : Crossover method (\"twx\" or \"gx\")" << endl;
    cout << "#    -init_ls [0 or 1] : Whether to do local search (0:disable, 1:enable)" << endl;
    cout << "#" << endl;
    cout << "#    -twx_loop [int] : Number of inheritance in TWX" << endl;
    cout << "#    -twx_perimeter [double] : Rate of perimeter nodes selection in TWX" << endl;
    cout << "#" << endl;
    cout << "#    -gx_ls [int] : Rate of perimeter nodes selection in TWX" << endl;

    cout << endl;
    cout << "# -from_arg : Execute GA with parameter specified in command line argument" << endl;
    cout << "#    -width [int]  : Number of column in a graph" << endl;
    cout << "#    -height [int] : Number of row in a graph" << endl;
    cout << "#    -degree [int] : Degree of a graph" << endl;
    cout << "#    -length [int] : Max edge length in a graph" << endl;
    cout << "#" << endl;
    cout << "#    -generation [int] : Max generation in GA" << endl;
    cout << "#    -population [int] : Individual population of a group" << endl;
    cout << "#    -offspring [int]  : Number of offsprings created with each crossover" << endl;
    cout << "#    -mutate_indiv [double] : Mutate probably for each individual" << endl;
    cout << "#    -mutate_gene [double]  : Mutate probably for each gene" << endl;
    cout << "#    -cross [string] : Crossover method (\"twx\" or \"gx\")" << endl;
    cout << "#    -init_ls [0 or 1] : Whether to do local search (0:disable, 1:enable)" << endl;
    cout << "#" << endl;
    cout << "#    -twx_loop [int] : Number of inheritance in TWX" << endl;
    cout << "#    -twx_perimeter [double] : Rate of perimeter nodes selection in TWX" << endl;
    cout << "#" << endl;
    cout << "#    -gx_ls [int] : Rate of perimeter nodes selection in TWX" << endl;
    
}

string getTimeString() {
    time_t t = time(NULL);
    tm *local = localtime(&t);

    string year = to_string(local->tm_year + 2000);
    string month = to_string(local->tm_mon + 1);
    string day = to_string(local->tm_mday);
    string hour = to_string(local->tm_hour);
    string min = to_string(local->tm_min);
    string sec = to_string(local->tm_sec);
    if(month.length() == 1) month = "0" + month;
    if(day.length() == 1) day = "0" + day;
    if(hour.length() == 1) hour = "0" + hour;
    if(min.length() == 1) min = "0" + min;
    if(sec.length() == 1) sec = "0" + sec;

    return year + month + day + hour + min + sec;
}

double transition(Parameter& param, int seed, string message = "") {
    cout << "seed : " << seed << endl << endl;
    cout << "Initializing..." << endl;
    Random::init(seed);
    GeneticAlgorithm ga(param);

    string dirName;
    if(message == "")
        dirName = RESULT_DIR + "/" + getTimeString();
    else    
        dirName = RESULT_DIR + "/" + message;
    Directory::create(dirName);

    ofstream ofsParam(dirName + "/parameter.csv");
    ofstream ofsTrans(dirName + "/transition.csv");
    param.writeParam(ofsParam);
    ofsParam << "seed," << seed << endl;
    ofsParam.close();
    
    GeneticAlgorithm::showHeader();
    ga.showParameter();
    ga.recordParameter(ofsTrans);
    while(true) {
        ga.step();
        ga.showParameter();
        ga.recordParameter(ofsTrans);
        if(ga.isFinished()) break;
    }
    
    ga.saveEdgesFile(dirName + "/" + to_string(ga.bestAspl()) + ".edges");
    return ga.bestAspl();
}

double transition(Parameter& param, string message = "") {
    Random::init();
    Random random;
    int seed = random.randomInt(INT_MAX);
    return transition(param, seed, message);
}

void repeat(int numLoop,  Parameter& param, int seed, string message = "") {
    if(message == "") message = getTimeString();
    string dirName = RESULT_DIR + "/" + message;
    Directory::create(dirName);

    double* aspl = new double[numLoop];
    double* processTimes = new double[numLoop];

    for(int i = 0; i < numLoop; ++i) {
        string fileName = message + "/" + to_string(i);

        cout << "=== Case" << i + 1 << " ===" << endl;
        
        aspl[i] = transition(param, seed + i, fileName);
        cout << endl;
    }

    ofstream ofs(param.getProblemName() + ".csv");
    cout << fixed;
    for(int i = 0; i < numLoop; ++i)  {
        cout << setprecision(15) << aspl[i] << " " << processTimes[i] << endl;
        ofs << aspl[i] << "," << processTimes[i] << endl;
    }

    delete[] aspl;
    delete[] processTimes;
}

void repeat(int numLoop,  Parameter& param, string message = "") {
    Random::init();
    Random random;
    int seed = random.randomInt(INT_MAX);
    repeat(numLoop, param, seed, message);
}

int main(int argc, char* argv[]) {
    CommandLineArgument arg(argc, argv);

    string message;
    if(arg.existOption("-m")) message = arg.getValueString("-m");
    else message = "";

    if(!Directory::exist(RESULT_DIR)) Directory::create(RESULT_DIR);

    if(arg.existOption("-help")) {
        showHelp();
    }
    else if(arg.existOption("-set_param")) {
        Parameter param("setting.dat");
        param.setParam(arg);
        param.showParam();
    }
    else if(arg.existOption("-show_param")) {
        Parameter param("setting.dat");
        param.showParam();
    }
    else if(arg.existOption("-generate_init_graph")) {
        Parameter param = arg.existOption("-from_arg") ? Parameter(arg) : Parameter("setting.dat");
        InitGraph::generate(param, 1000);
    }
    else if(arg.existOption("-repeat")) {
        Parameter param = arg.existOption("-from_arg") ? Parameter(arg) : Parameter("setting.dat");
        param.showParam(); cout << endl;
        int numLoop = arg.getValueInt("-repeat");
        if(arg.existOption("-seed")) repeat(numLoop, param, arg.getValueInt("-seed"), message);
        else                         repeat(numLoop, param, message);
    }
    else {
        Parameter param = arg.existOption("-from_arg") ? Parameter(arg) : Parameter("setting.dat");
        param.showParam(); cout << endl;
        if(arg.existOption("-seed")) transition(param, arg.getValueInt("-seed"), message);
        else                         transition(param, message);
    }
}
