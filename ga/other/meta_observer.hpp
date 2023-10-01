#pragma once

#include "../grid_graph_base.hpp"
template<typename T>
class Collection;
class Individual;

class MetaObserver : public GridGraph {
public:
    static void reset();
    static void calcInheritRate(double numEmptyAdjacent);
    static void calcRefineRate(Collection<Individual>& parents, Collection<Individual>& childs);
    static void calcChildVariation(Collection<Individual>& parents, Collection<Individual>& childs);
    static void calcLengthDistribution(Individual& indiv);
    static void calcNumAsplEvaluation(int numAsplEvaluation);

    static inline double inharitRate() { return _inharitRate / _inharitPopulation; }
    static inline double refineRate() { return (double)_refineCount / _refinePopulation; }
    static inline double childVariation() { return (double)_childVariation / _childPopulation; }
    static inline double numLength(int length) {return (double)_edgeLengthDistribution[length] / _edgeLengthPopulation; }
    static inline double numAsplEvaluation() {return (double)_sumNumAsplEvaluation / _numLocalSearch; }
private:
    static double _inharitRate;
    static int _inharitPopulation;

    static int _refineCount;
    static int _refinePopulation;

    static int _childVariation;
    static int _childPopulation;

    static const int _maxLength;
    static int _edgeLengthDistribution[10];
    static int _edgeLengthPopulation;

    static int _sumNumAsplEvaluation;
    static int _numLocalSearch;
};