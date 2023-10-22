#pragma once

#include "../grid_graph_base.hpp"
class Individual;

class Randomizer: public GridGraph{
public:
    void operator()(Individual& indiv);
private:
    void setRandomEdge(Individual& indiv);
};