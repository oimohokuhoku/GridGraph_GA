#include <iostream>
#include <cstdlib>
#include <memory>
#include "group.hpp"
#include "individual.hpp"
#include "evaluate/adj_aspl.hpp"
#include "select/i_selector.hpp"
#include "select/mgg.hpp"
#include "crossover/twx.hpp"
#include "crossover/dex.hpp"
#include "crossover/uniform_cross.hpp"
#include "mutate/rotate.hpp"
#include "other/randomizer.hpp"
#include "other/local_search.hpp"
#include "other/meta_observer.hpp"
#include "../io/edges_file_writer.hpp"
#include "../other/common.hpp"
#include "../other/collection.hpp"
using std::unique_ptr;

/* public */
Group::Group() {
	this->_indivs = Collection<Individual>(_GROUP_SIZE);

	_bestIndex = -1;
	_worstIndex = -1;
	_averageASPL = -1;
	_averageDiameter = -1;
}

void Group::createRandomIndivs() {
	ADJ_ASPL evaluater;
	TwoOpt twoOpt;
	Randomizer randomizer;

	for (int i = 0; i < _GROUP_SIZE; ++i) {
		randomizer.randomize(_indivs[i]);

		while (true) {
			_indivs[i].editGene(evaluater);
			
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
	TWX crossover1;
	TwoOpt mutater;
	Rotate rotate;
	ADJ_ASPL evaluate;
	LocalSearch ls;
	MetaObserver::reset();
	unique_ptr<ICopySelector> copySelector = selector.generateCopySelector(this->_indivs);
	unique_ptr<ISurviveSelector> surviveSelector = selector.generateSurviveSelector();

	int childNum = 0;
	this->_indivs = Collection<Individual>(_GROUP_SIZE);

	while (childNum < _GROUP_SIZE) {
		Collection<Individual> parents = copySelector->select();
		Collection<Individual> childs(_CREATED_CHILD_NUM);

		for(int i = 0; i < _CREATED_CHILD_NUM; ++i) crossover1.cross(parents[0], parents[1], childs[i]);

		for (int i = 0; i < _CREATED_CHILD_NUM; ++i) mutater.edit(childs[i]);
		
		for (int i = 0; i < _CREATED_CHILD_NUM; ++i) evaluate.edit(childs[i]);

		if(doLocalSearch)
			for (int i = 0; i < _CREATED_CHILD_NUM; ++i) ls.doLocalSearch(childs[i]);

		Collection<Individual> survivors = surviveSelector->select(parents, childs);

		//survivors[0].editGene(rotate);
		//survivors[1].editGene(rotate);

		this->addIndiv(survivors, &childNum);

		MetaObserver::calcChildVariation(parents, childs);
		MetaObserver::calcRefineRate(parents, childs);
	}

	this->tallyFitness();
}

void Group::createBestEdgesFile(string filePath) const {
	EdgeFileWriter::save(_indivs[_bestIndex], filePath);
}

Individual Group::getBestIndiv() {
	return _indivs[_bestIndex];
}

void Group::setParameter(int groupSize, int createdChildNum) {
	Group::_GROUP_SIZE = groupSize;
	Group::_CREATED_CHILD_NUM = createdChildNum;
}

/* private */
int Group::_GROUP_SIZE;
int Group::_CREATED_CHILD_NUM;

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
