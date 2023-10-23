#include "ga/group.hpp"

#include <iostream>
#include <cstdlib>
#include <memory>
#include "ga/individual.hpp"
#include "ga/evaluate/adj_aspl.hpp"
#include "ga/select/elitist_recombination.hpp"
#include "ga/select/mgg.hpp"
#include "ga/crossover/crossover.hpp"
#include "ga/mutate/two_opt.hpp"
#include "ga/other/randomizer.hpp"
#include "ga/other/local_search.hpp"
#include "ga/other/meta_observer.hpp"
#include "ga/other/init_graph_generator.hpp"
#include "io/edges_file_writer.hpp"
#include "other/collection.hpp"
using std::unique_ptr;

#include <string>

/* public */
Group::Group(int groupSize, int numCreatedChild): _GROUP_SIZE(groupSize), _NUM_CREATED_CHILD(numCreatedChild) {
	this->_indivs = Collection<Individual>(_GROUP_SIZE);
	_bestIndex = -1;
	_worstIndex = -1;
	_averageASPL = -1;
	_averageDiameter = -1;
	_generation = 0;
}

Group::~Group() {
	delete crossover;
}

void Group::createRandomIndivs(const Parameter& param) {
	if(numInitLocalSearch > 0) {
		InitGraph::load(param, numInitLocalSearch, _indivs);
		ADJ_ASPL evaluate;
		for(int i = 0; i < _GROUP_SIZE; ++i)
			evaluate(_indivs[i]);
	}
	else {
		Randomizer randomizer;
		for (int i = 0; i < _GROUP_SIZE; ++i) {
			randomizer(_indivs[i]);
		}
	}

	tallyFitness();
}

void Group::changeGeneration_MGG() {
	MetaObserver::reset();
	MGG selector;
	TwoOpt mutater;
	ADJ_ASPL evaluate;
	LocalSearch localSearch;

	int parentIndexA, parentIndexB;
	Collection<Individual> parents(2);
	Collection<Individual> childs(_NUM_CREATED_CHILD);

	selector.copySelect(&parentIndexA, &parentIndexB, _GROUP_SIZE);
	parents[0] = _indivs[parentIndexA];
	parents[1] = _indivs[parentIndexB];

	for(int i = 0; i < _NUM_CREATED_CHILD; ++i) {
		(*crossover)(parents[0], parents[1], childs[i]);
		mutater(childs[i]);
		evaluate(childs[i]);
	}

	Individual survivorsA, survivorsB;
	selector.surviveSelect(parents, childs, survivorsA, survivorsB);

	_indivs[parentIndexA] = survivorsA;
	_indivs[parentIndexB] = survivorsB;

	MetaObserver::calcChildVariation(parents, childs);
	MetaObserver::calcRefineRate(parents, childs);

	this->tallyFitness();
	_generation++;
}

void Group::changeGeneration_ER() {
	Elitist_Recombination selector(_GROUP_SIZE);
	TwoOpt mutater;
	ADJ_ASPL evaluate;
	LocalSearch localSearch;

	int parentIndexA, parentIndexB;
	Collection<Individual> parents(2);
	Collection<Individual> childs(_NUM_CREATED_CHILD);

	for(int i = 0; i < _GROUP_SIZE / 2; ++i) {
		selector.copySelect(&parentIndexA, &parentIndexB);
		parents[0] = _indivs[parentIndexA];
		parents[1] = _indivs[parentIndexB];

		for(int i = 0; i < _NUM_CREATED_CHILD; ++i) {
			(*crossover)(parents[0], parents[1], childs[i]);
			mutater(childs[i]);
			evaluate(childs[i]);
		}

		Individual survivorsA, survivorsB;
		selector.surviveSelect(parents, childs, survivorsA, survivorsB);

		_indivs[parentIndexA] = survivorsA;
		_indivs[parentIndexB] = survivorsB;

		MetaObserver::calcChildVariation(parents, childs);
		MetaObserver::calcRefineRate(parents, childs);
	}

	this->tallyFitness();
	_generation++;
}

Individual Group::getBestIndiv() {
	return _indivs[_bestIndex];
}

/* private */
void Group::tallyFitness() {
	_bestIndex = 0;
	_worstIndex = 0;
	for(int i = 1; i < _GROUP_SIZE; ++i) {
		if(_indivs[i].betterThan(_indivs[_bestIndex])) _bestIndex = i;
		if(_indivs[_worstIndex].betterThan(_indivs[i])) _worstIndex = i;
	}

	double sumASPL = 0;
	int sumDiameter = 0;
	for(int i = 0; i < _GROUP_SIZE; ++i) {
		sumASPL += _indivs[i].aspl;
	}

	this->_averageASPL = sumASPL / _GROUP_SIZE;
	this->_averageDiameter = (double)sumDiameter / _GROUP_SIZE;
}

void Group::addIndiv(Collection<Individual>& survivors, int* childNum) {
	int count = survivors.size();

	for(int i = 0; i < count; ++i) {
		this->_indivs[*childNum] = survivors[i];
		(*childNum)++;

		if(*childNum == _GROUP_SIZE) return;
	}
}
