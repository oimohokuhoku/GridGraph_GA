#pragma once

#include <climits>
#include "../grid_graph_base.hpp"
class Individual;

class LocalSearch: public GridGraph {
public:
    LocalSearch();
    ~LocalSearch();
    void doLocalSearch(Individual& indiv, int maxAsplCalc = INT_MAX);
    void collectNearNodes(int srcNode, int* out);

private:
    int* _nearNodes;
};
