#include<iostream>
#include<cstdio>
#include<ios>
#include<iomanip>
#include<fstream>
#include<cstring>
#include "other/common.hpp"
#include "other/command_line_argument.hpp"
#include "ga/other/meta_observer.hpp"
#include "ga/other/local_search.hpp"
#include "ga/group.hpp"
#include "io/edges_file_reader.hpp"
#include "io/edges_file_writer.hpp"
#include "test/unit_test.hpp"

using namespace std;

/* Prameter Setting ------------------------------------------------ */
const int COLUMN_NUM = 10;
const int ROW_NUM = 10;
const int DEGREE = 4;
const int MAX_LENGTH = 2;

const int MAX_GENERATION = 300;
const int GROUP_SIZE = 100;
const int CREATED_CHILD_NUM = 100;
const double INDIV_MUTATE_PROBABILITY = 0.01;
const double GENE_MUTATE_PROBABILITY = 0.05;

int NUM_LOCAL_SEARCH = COLUMN_NUM * ROW_NUM * 10;
/* ------------------------------------------------------------------ */

double transition();
void unitTest();
void repeat(int numLoop);
void findNotSearchedEdges();
void coutHedder();
void coutGroup(int generationNum, const Group& group);
void ofstreamGroup(ofstream& ofs, int generationNum, const Group& group);
void showArguments();

int main(int argc, char* argv[]) {
    initRand();

    if(argc == 1) {
        transition();
    }
    else {
        CommandLineArgument args(argc, argv);
        if(args.existOption("-help")) showArguments();
        else if(args.existOption("-test")) unitTest();
        else if(args.existOption("-repeat")) {
            int numLoop = stoi(args.getValue("-repeat"));
            repeat(numLoop);
        }
        else if(args.existOption("-analyse")) findNotSearchedEdges();
        else  cout << "Invalid command line argument" << endl;
    }
}

double transition() {
    Group::setParameter(GROUP_SIZE, CREATED_CHILD_NUM);
    GridGraph::setParameter(COLUMN_NUM, ROW_NUM, DEGREE, MAX_LENGTH);
    Mutater_Base::SetParameter(INDIV_MUTATE_PROBABILITY, GENE_MUTATE_PROBABILITY);

    //ofstream ofs("test.csv");
    Group group;
    group.createRandomIndivs();
    group.doLocalSearch = false;

    coutHedder();
    coutGroup(0, group);

    int notRefineCount = 0;
    for (int i = 1; i <= MAX_GENERATION; ++i) {
        group.changeGeneration();
        coutGroup(i, group);

        //if(i % 10 == 0) ofstreamGroup(ofs, i, group);

        //group.createBestEdgesFile("test.edges");

        if(MetaObserver::childVariation() < 1) {
            notRefineCount++;
            if(notRefineCount == 10) break;
        }
        else notRefineCount = 0;
    }

    //ofstreamGroup(ofs, MAX_GENERATION, group);
    group.createBestEdgesFile(
        to_string(COLUMN_NUM) + "_" + to_string(ROW_NUM) +
        to_string(DEGREE) + "_" + 
        to_string(MAX_LENGTH) + "_" + 
        to_string(group.bestASPL()) + ".edges"
    );

    return group.bestASPL();
}

void repeat(int numLoop) {
    double* aspl = new double[numLoop];
    for(int i = 0; i < numLoop; ++i) {
        cout << "=== Case" << i + 1 << " ===" << endl;
        aspl[i] = transition();
        cout << endl;
    }

    double sum = 0;
    for(int i = 0; i < numLoop; ++i) sum += aspl[i];

    cout << fixed;
    for(int i = 0; i < numLoop; ++i) cout << setprecision(15) << aspl[i] << endl;
    cout << "average" << endl;
    cout << setprecision(15) << sum / numLoop << endl;

    delete[] aspl;
}

void unitTest() {
    UnitTest tester;
    tester.adjAsplTest();
}

void findNotSearchedEdges() {
    string fileName;
    const int NUM_NODE = COLUMN_NUM * ROW_NUM;

    cout << "Input best edges file : "; cin >> fileName;
    Individual bestIndiv = EdgeFileReader::GenerateIndividual(fileName);

    while(true) {
        cout << "Input obtained edges file : "; cin >> fileName;

        if(fileName == "quit") break;

        Individual indiv = EdgeFileReader::GenerateIndividual(fileName);
        for(int n = 0; n < NUM_NODE; ++n) {
            int numDegree1 = indiv.degrees[n];

            for(int d1 = 0; d1 < numDegree1; ++d1) {
                int numDegree2 = bestIndiv.degrees[n];

                for(int d2 = 0; d2 < numDegree2; ++d2) {
                    if(indiv.adjacent[n][d1] == bestIndiv.adjacent[n][d2]) {
                        bestIndiv.adjacent[n][d2] = -1;
                    }
                }
            }
        }
    }

    EdgeFileWriter::save(bestIndiv, "not_searched.edges");

}

void showParameters() {

}

void coutHedder() {
    cout << left;
    cout << setw(6) << "Fase";
    cout << setw(6) << "Diam";

    cout << setw(19) << "Best ASPL";
    cout << setw(19) << "Average ASPL";
    cout << setw(19) << "Worst ASPL";

    cout << setw(14) << "Inherit rate";
    cout << "child variation";

    cout << endl;
    
}

void coutGroup(int generationNum, const Group& group) {
    const int DIGIT = 15;

    cout << left;
    cout << setw(6) << generationNum;
    cout << setw(6) << group.bestDiameter();

    cout << fixed;
    cout << setw(19) << setprecision(DIGIT) << group.bestASPL();
    cout << setw(19) << setprecision(DIGIT) << group.averageASPL();
    cout << setw(19) << setprecision(DIGIT) << group.worstASPL();

    cout << setw(14) << setprecision(5) << MetaObserver::inharitRate();
    cout << setw(14) << setprecision(3) << MetaObserver::refineRate();

    cout << defaultfloat;
    cout << MetaObserver::childVariation() << endl;
}

void ofstreamGroup(ofstream& ofs, int generationNum, const Group& group) {
    ofs << generationNum << ",";
    ofs << group.bestDiameter() << ",";
    ofs << group.bestASPL() << ",";
    ofs << group.averageASPL() << ",";
    ofs << group.worstASPL() << endl;
}

void showArguments() {
    cout << "[Command Line Arguments]" << endl;
    cout << "-transition : Execute GA and show transition of GA parameter by generation" << endl;
    cout << "    -skipCSV       : skip output transition to CSV file" << endl;
    cout << "    -skipEdgesFile : skip output result to edges File" << endl;
    cout << "-test       : Unit test" << endl;
}
