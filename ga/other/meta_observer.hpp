#pragma once

template<typename T>
class Collection;
class Individual;

class MetaObserver {
public:
    static void reset();
    static void calcInheritRate(double numEmptyAdjacent);
    static void calcRefineRate(Collection<Individual>& parents, Collection<Individual>& childs);
    static void calcChildVariation(Collection<Individual>& parents, Collection<Individual>& childs);

    static inline double inharitRate() { return _inharitRate / _inharitPopulation; }
    static inline double refineRate() { return (double)_refineCount / _refinePopulation; }
    static inline double childVariation() { return (double)_childVariation / _childPopulation; }
private:
    static double _inharitRate;
    static int _inharitPopulation;
    static int _refineCount;
    static int _refinePopulation;
    static int _childVariation;
    static int _childPopulation;
};