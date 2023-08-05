#pragma once

#include "../grid_graph_base.hpp"
class Individual;

class Randomizer: public GridGraph{
public:
    Randomizer() = default;
    ~Randomizer() = default;
    void randomize(Individual& indiv);
};