#include "init_graph_generator.hpp"
#include <string>
#include <iostream>
#include "ga/individual.hpp"
#include "ga/grid_graph_base.hpp"
#include "ga/parameter.hpp"
#include "ga/other/randomizer.hpp"
#include "ga/other/local_search.hpp"
#include "io/edges_file_reader.hpp"
#include "io/edges_file_writer.hpp"
#include "other/directory.hpp"
#include "other/array_util.hpp"
using std::cout, std::endl;

void InitGraph::generate(const Parameter& param, int numGeneratedGraph, int numMaxLocalSearch) {
    std::cout << "Node   : " << param.numColumn() << "x" << param.numRow() << std::endl;
    std::cout << "Degree : " << param.degree() << std::endl;
    std::cout << "Length : " << param.maxLength() << std::endl;

    GridGraph::setParameter(param.numColumn(), param.numRow(), param.degree(), param.maxLength());
    Randomizer randomize;
    LocalSearch ls;

    string saveDir = getDirectoryName(param, numMaxLocalSearch);
    if(!Directory::exist(ROOT_DIRECTORY)) Directory::create(ROOT_DIRECTORY);
    if(!Directory::exist(saveDir)) Directory::create(saveDir);
    
    for(int i = 0; i < numGeneratedGraph; ++i) {
        Individual indiv;
        randomize(indiv);
        ls(indiv, numMaxLocalSearch);
        
        EdgeFileWriter::save(indiv, saveDir + "/" + std::to_string(i) + ".edges");

        std::cout << i << " : " << indiv.diameter << " " << indiv.aspl << std::endl;
    }
}

void InitGraph::load(const Parameter& param, int numMaxLocalSearch, Collection<Individual>& storage) {
    string saveDir = getDirectoryName(param, numMaxLocalSearch);

    if(!Directory::exist(saveDir)) {
        std::cout << "Initial individuals isn't generated with this parameter. " << std::endl;
        std::cout << "Would you like to generate initial individuals? (yes:y, no:n)" << std::endl;
        char c; std::cout << ">>"; std::cin >> c;
        if(c == 'n') exit(0);
        generate(param, NUM_INIDIV_FILE, numMaxLocalSearch);
    }

    int* seq = new int[NUM_INIDIV_FILE];
    ArrayUtil::fillSequence(seq, NUM_INIDIV_FILE);
    ArrayUtil::shaffle<int>(seq, NUM_INIDIV_FILE);
    for(int i = 0; i < storage.size(); ++i) {
        string fileName = saveDir + "/" + std::to_string(seq[i]) + ".edges";
        storage[i] = EdgeFileReader::GenerateIndividual(fileName);
    }

    delete[] seq;
}

std::string InitGraph::getDirectoryName(const Parameter& param, int numMaxLocalSearch) {
    if(numMaxLocalSearch == INT_MAX)
        return ROOT_DIRECTORY + "/" + param.getProblemName() + "_MAX";
    else
        return ROOT_DIRECTORY + "/" + param.getProblemName() + "_" + std::to_string(numMaxLocalSearch);
}

const std::string InitGraph::ROOT_DIRECTORY = "initial_graphs"; 
const int InitGraph::NUM_INIDIV_FILE = 1000;