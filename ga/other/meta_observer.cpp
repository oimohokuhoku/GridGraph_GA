#include "meta_observer.hpp"
#include "../individual.hpp"
#include "../../other/collection.hpp"

void MetaObserver::reset() {
	MetaObserver::_inharitRate = 0;
	MetaObserver::_inharitPopulation = 0;
	MetaObserver::_refineCount = 0;
	MetaObserver::_refinePopulation = 0;
	MetaObserver::_childVariation = 0;
	MetaObserver::_childPopulation = 0;
	MetaObserver::_edgeLengthPopulation = 0;
	MetaObserver::_sumNumAsplEvaluation = 0;
	MetaObserver::_numLocalSearch = 0;

	for(int i = 0; i < MetaObserver::_maxLength; ++i)
		MetaObserver::_edgeLengthDistribution[i] = 0;
}

void MetaObserver::calcInheritRate(double numEmptyAdjacent) {
	_inharitRate += 1.0 - numEmptyAdjacent;
	_inharitPopulation++;
}

void MetaObserver::calcRefineRate(Collection<Individual>& parents, Collection<Individual>& childs) {
	Individual* better;
	int refineCount = 0;

	if(parents[0] < parents[1]) better = &parents[0];
	else						better = &parents[1];

	for(int i = 0; i < childs.size(); ++i)
		if(childs[i] < *better) refineCount++;

	MetaObserver::_refineCount += refineCount;
	MetaObserver::_refinePopulation += childs.size();
}

void MetaObserver::calcChildVariation(Collection<Individual>& parents, Collection<Individual>& childs) {
    const int CREATED_CHILD_NUM = childs.size();

	int variation = 0;
	for(int i = 0; i < CREATED_CHILD_NUM; ++i) {
		if(childs[i].sameGraph(parents[0])) continue;
		if(childs[i].sameGraph(parents[1])) continue;

		bool same = false;
		for(int j = i + 1; j < CREATED_CHILD_NUM; ++j) {
			if(childs[i].sameGraph(childs[j])) {
				same = true;
				break;
			}
		}

		if(!same) variation++;
	}

	MetaObserver::_childVariation += variation;
	MetaObserver::_childPopulation++;
}

void MetaObserver::calcLengthDistribution(Individual& indiv) {
	for(int n1 = 0; n1 < numNode(); ++n1) {
		int deg = indiv.degrees[n1];

		for(int d = 0; d < deg; ++d) {
			int n2 = indiv.adjacent[n1][d];
			if(n1 <= n2) continue;

			int len = getLength(n1, n2);
			MetaObserver::_edgeLengthDistribution[len]++;
		}
	}
	MetaObserver::_edgeLengthPopulation++;
}

void MetaObserver::calcNumAsplEvaluation(int numAsplEvaluation) {
	_sumNumAsplEvaluation += numAsplEvaluation;
	_numLocalSearch++;
}

double MetaObserver::_inharitRate = 0;
int MetaObserver::_inharitPopulation = 0;
int MetaObserver::_refineCount = 0;
int MetaObserver::_refinePopulation = 0;
int MetaObserver::_childVariation = 0;
int MetaObserver::_childPopulation = 0;
const int MetaObserver::_maxLength = 10;
int MetaObserver::_edgeLengthDistribution[10];
int MetaObserver::_edgeLengthPopulation = 0;
int MetaObserver::_sumNumAsplEvaluation = 0;
int MetaObserver::_numLocalSearch = 0;