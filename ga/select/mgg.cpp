#include <iostream>
#include<cstdlib>
#include<memory>
#include"mgg.hpp"
#include "i_selector.hpp"
#include"../individual.hpp"
#include"../../other/collection.hpp"
using std::unique_ptr;

unique_ptr<ICopySelector> MGG::generateCopySelector(Collection<Individual>& parents) {
    return unique_ptr<ICopySelector>(new MGG::CopySelector(parents));
}

unique_ptr<ISurviveSelector> MGG::generateSurviveSelector() {
    return unique_ptr<ISurviveSelector>(new MGG::SurviveSelector());
}



MGG::CopySelector::CopySelector(Collection<Individual>& parents) {
    this->_parents = parents;
    this->_selectSeq = new int[parents.size()];
    this->_selectIndex = 0;
    setRandomSequence(_selectSeq);
}

MGG::CopySelector::~CopySelector() {
    delete[] this->_selectSeq;
}

Collection<Individual> MGG::CopySelector::select() {
	Collection<Individual> result(2);
    int index;

    index = _selectSeq[_selectIndex];
	result[0] = _parents[index];
    _selectIndex++;

	index = _selectSeq[_selectIndex];
	result[1] = _parents[index];
    _selectIndex++;

	return result;
}

void MGG::CopySelector::setRandomSequence(int* seq) {
    const int NUM_PARENT = _parents.size();

    for(int i = 0; i < NUM_PARENT; ++i) seq[i] = i;

    for(int i = 0; i < NUM_PARENT; ++i) {
        int j = randInt(0, NUM_PARENT);
        int temp = seq[i];
        seq[i] = seq[j];
        seq[j] = temp;
    }
}



Collection<Individual> MGG::SurviveSelector::select(Collection<Individual>& parents, Collection<Individual>& childs) {
    this->_parents = parents;
    this->_childs = childs;
    this->_selectedParent.reset(new bool[parents.size()]);
    this->_selectedChild.reset(new bool[childs.size()]);

    for(int i = 0; i < parents.size(); ++i) _selectedParent[i] = false;
    for(int i = 0; i < childs.size(); ++i) _selectedChild[i] = false;

    Collection<Individual> result(2);
	result[0] = selectElite();
	result[1] = selectRoulette();

	return result;
}

Individual MGG::SurviveSelector::selectElite() {
    const int PARENT_SIZE = _parents.size();
    const int CHILD_SIZE = _childs.size();

    Individual bestParent = _parents[0];
    int bestParentIndex = 0;
    for(int i = 1; i < PARENT_SIZE; ++i) {
        if(_parents[i] < bestParent) {
            bestParent = _parents[i];
            bestParentIndex = i;
        }
    }

    Individual bestChild = _childs[0];
    int bestChildIndex = 0;
    for(int i = 1; i < CHILD_SIZE; ++i) {
        if(_childs[i] < bestChild) {
            bestChild = _childs[i];
            bestChildIndex = i;
        }
    }

    if(bestParent < bestChild) {
        _selectedParent[bestParentIndex] = true;
        return bestParent;
    }
    else {
        _selectedChild[bestChildIndex] = true;
        return bestChild;
    }
}

Individual& MGG::SurviveSelector::selectRoulette() {
    const int PARENT_SIZE = _parents.size();
    const int CHILD_SIZE = _childs.size();

    double sumASPL = 0;
    for(int i = 0; i < PARENT_SIZE; ++i) {
        if(_selectedParent[i]) continue;
        sumASPL += (1.0 / _parents[i].aspl);
    }
    for(int i = 0; i < CHILD_SIZE; ++i) {
        if(_selectedChild[i]) continue;
        sumASPL += (1.0 / _childs[i].aspl);
    }

    double roulette = randDouble() * sumASPL;
    for(int i = 0; i < PARENT_SIZE; ++i) {
        if(_selectedParent[i]) continue;
        roulette -= (1.0 / _parents[i].aspl);
        if(roulette <= 0) return _parents[i];
    }
    for(int i = 0; i < CHILD_SIZE; ++i) {
        if(_selectedChild[i]) continue;
        roulette -= (1.0 / _childs[i].aspl);
        if(roulette <= 0) return _childs[i];
    }

    std::cerr << "Exception in SurvivorSelector::selectRoulette()" << std::endl;
    exit(-1);
}
