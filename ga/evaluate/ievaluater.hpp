#pragma once

#include "../grid_graph_base.hpp"
class Individual;

class IEvaluater: public GridGraph {
public:
    virtual ~IEvaluater() = default;
    virtual void operator() (Individual& indiv) = 0;
    virtual int dislinkedNode() = 0;
};