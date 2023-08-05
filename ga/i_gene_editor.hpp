#pragma once

#include "grid_graph_base.hpp"
class Individual;

class IGeneEditor: public GridGraph {
public:
    IGeneEditor() = default;
    virtual ~IGeneEditor() = default;
    virtual void edit(Individual& indiv) = 0;
};

