#pragma once

#include <climits>
#include "../grid_graph_base.hpp"
class Individual;

class LocalSearch: public GridGraph {
public:
    LocalSearch();
    ~LocalSearch();
    void operator()(Individual& indiv, int maxAsplCalc = INT_MAX);

private:
    int* _nearNodes;
    void collectNearNodes(int srcNode, int* out);
};
