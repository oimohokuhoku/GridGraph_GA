#pragma once

#include "../grid_graph_base.hpp"
class Individual;

class IEvaluater: public GridGraph {
public:
    virtual ~IEvaluater() = default;
    virtual int dislinkedNode() = 0;
};