#include "ga/group.hpp"

#include <iostream>
#include <cstdlib>
#include <memory>
#include "ga/individual.hpp"
#include "ga/evaluate/adj_aspl.hpp"
#include "ga/select/i_selector.hpp"
#include "ga/select/mgg.hpp"
#include "ga/crossover/crossover.hpp"
#include "ga/mutate/two_opt.hpp"
#include "ga/other/randomizer.hpp"
#include "ga/other/local_search.hpp"
#include "ga/other/meta_observer.hpp"
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

void Group::createRandomIndivs() {
	ADJ_ASPL evaluater;
	TwoOpt twoOpt;
	Randomizer randomizer;

	for (int i = 0; i < _GROUP_SIZE; ++i) {
		randomizer.randomize(_indivs[i]);

		while (true) {
			evaluater(_indivs[i]);
			
			//modify graph until graph becomes linked
			int dislink = evaluater.dislinkedNode();
			if (dislink == -1) break;
			twoOpt.twoOpt(_indivs[i], dislink);
		}

		if(doLocalSearch) {
			LocalSearch ls;
			ls.doLocalSearch(_indivs[i]);
		}
	}

	tallyFitness();
}

void Group::changeGeneration() {
	MGG selector;
	TwoOpt mutater;
	ADJ_ASPL evaluate;
	LocalSearch ls;
	MetaObserver::reset();
	unique_ptr<ICopySelector> copySelector = selector.generateCopySelector(this->_indivs);
	unique_ptr<ISurviveSelector> surviveSelector = selector.generateSurviveSelector();

	int childNum = 0;
	this->_indivs = Collection<Individual>(_GROUP_SIZE);

	while (childNum < _GROUP_SIZE) {
		Collection<Individual> parents = copySelector->select();
		Collection<Individual> childs(_NUM_CREATED_CHILD);

		for(int i = 0; i < _NUM_CREATED_CHILD; ++i) (*crossover)(parents[0], parents[1], childs[i]);

		for (int i = 0; i < _NUM_CREATED_CHILD; ++i) mutater(childs[i]);
		
		for (int i = 0; i < _NUM_CREATED_CHILD; ++i) evaluate(childs[i]);

		if(doLocalSearch)
			for (int i = 0; i < _NUM_CREATED_CHILD; ++i) ls.doLocalSearch(childs[i]);

		Collection<Individual> survivors = surviveSelector->select(parents, childs);

		this->addIndiv(survivors, &childNum);

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
		if(_indivs[i] < _indivs[_bestIndex]) _bestIndex = i;
		if(_indivs[_worstIndex] < _indivs[i]) _worstIndex = i;
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
