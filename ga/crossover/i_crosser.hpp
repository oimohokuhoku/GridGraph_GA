#pragma once

#include "../grid_graph_base.hpp"
class Individual;

class ICrosser: public GridGraph{
public:
    ICrosser() = default;
    virtual ~ICrosser() = default;
    virtual void cross(const Individual& parentA, const Individual& parentB, Individual& child) = 0;

protected:
    void virtual modifyGraph(Individual& indiv);

private: /* prohibit */
    ICrosser(ICrosser& obj) = delete;
    ICrosser(ICrosser&& obj) = delete;
};