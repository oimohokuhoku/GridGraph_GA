#include "elitist_recombination.hpp"
#include "copy_selector.hpp"
#include "survive_selector.hpp"

#include "ga/individual.hpp"
#include "other/collection.hpp"
#include "other/array_util.hpp"
#include <iostream>

Elitist_Recombination::Elitist_Recombination(int numParent) : _NUM_PAERNT(numParent){
    _sequence = new int[numParent];
    ArrayUtil::fillSequence(_sequence, numParent);
    ArrayUtil::shaffle(_sequence, numParent);
    _index = 0;
}

Elitist_Recombination::~Elitist_Recombination() {
    delete[] _sequence;
}

void Elitist_Recombination::copySelect(int* index1, int* index2) {
    *index1 = _sequence[_index++];
    *index2 = _sequence[_index++];
}

void Elitist_Recombination::surviveSelect(const Collection<Individual>& parents, const Collection<Individual>& childs, Individual& survivorA, Individual& survivorB) {
    Collection<Individual> indivs = parents.concat(childs);
    bool* selected = ArrayUtil::newArray<bool>(indivs.size(), false);

    ElitistSelect selectA;
    RankingSelect selectB;
    survivorA = selectA.select(indivs, selected);
    survivorB = selectB.select(indivs, selected);
}