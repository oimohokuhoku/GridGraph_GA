#include "ga.hpp"
#include <iostream>
#include <fstream>
#include <ios>
#include <iomanip>
#include "group.hpp"
#include "parameter.hpp"
#include "mutate/mutater_base.hpp"
#include "crossover/crossover.hpp"
#include "other/meta_observer.hpp"
#include "../io/edges_file_writer.hpp"
#include "../other/random.hpp"
using namespace std;

GeneticAlgorithm::GeneticAlgorithm(Parameter param) : _MAX_GENERATION(param.maxGeneration()) {
    GridGraph::setParameter(param.numColumn(), param.numRow(), param.degree(), param.maxLength());
    Mutater_Base::SetParameter(param.mutateIndiv(), param.mutateGene());

    _group = new Group(param.groupSize(), param.numChild());
    _group->crossover =  generateCrossover(param);
    _group->doLocalSearch = param.doLocalSearch();
    _group->createRandomIndivs();
}

GeneticAlgorithm::~GeneticAlgorithm() {
    if(_group != nullptr) {
        delete _group;
        _group = nullptr;
    }
}

void GeneticAlgorithm::step() {
    _group->changeGeneration();
    _generation++;

    if (MetaObserver::childVariation() < 0.1) {
        _convergeCount++;
        if (_convergeCount == 10) _converged = true;
    }
    else {
        _convergeCount = 0;
    }
}

bool GeneticAlgorithm::isFinished() const {
    if(_converged) return true;
    if(_generation >= _MAX_GENERATION) return true;
    return false;
}

void GeneticAlgorithm::showParameter() const {
    const int DIGIT = 15;

    cout << left;
    cout << setw(6) << _generation;
    cout << setw(6) << _group->bestDiameter();

    cout << fixed;
    cout << setw(19) << setprecision(DIGIT) << _group->bestASPL();
    cout << setw(19) << setprecision(DIGIT) << _group->averageASPL();
    cout << setw(19) << setprecision(DIGIT) << _group->worstASPL();

    cout << setw(14) << setprecision(5) << MetaObserver::inharitRate();
    cout << setw(14) << setprecision(3) << MetaObserver::refineRate();

    cout << defaultfloat;
    cout << MetaObserver::childVariation() << " ";
 
    cout << endl;
}

void GeneticAlgorithm::recordParameter(ofstream& ofs) const {
    ofs << _generation << ",";
    ofs << _group->bestDiameter() << ",";
    ofs << _group->bestASPL() << ",";
    ofs << _group->averageASPL() << ",";
    ofs << _group->worstASPL() << ",";
    ofs << MetaObserver::inharitRate() << ",";
    ofs << MetaObserver::refineRate() << ",";
    ofs << MetaObserver::childVariation() << endl;
}

double GeneticAlgorithm::bestAspl() const {
    return _group->bestASPL();
}

void GeneticAlgorithm::showHeader() {
    cout << "============================================================================================================================" << endl;
    cout << left;
    cout << setw(6) << "Fase";
    cout << setw(6) << "Diam";

    cout << setw(19) << "Best ASPL";
    cout << setw(19) << "Average ASPL";
    cout << setw(19) << "Worst ASPL";

    cout << setw(14) << "Inherit rate";
    cout << setw(14) << "Refine rate";
    cout << "child variation";

    cout << endl;
}

void GeneticAlgorithm::saveEdgesFile(string filepath) const {
    EdgeFileWriter::save(_group->bestIndividual(), filepath);
}

/* private */
ICrosser* GeneticAlgorithm::generateCrossover(Parameter param) {
    if(param.crossover() == "twx") 
        return new TourWeavingCrossover(param.twxNumLoop(), param.twxPerimeterSelectRate());
    if(param.crossover() == "gx")
        return new GraftingCrossover();

    cerr << "Crossover \"" << param.crossover() << "\" is not implemented" << endl;
    exit(0);
}