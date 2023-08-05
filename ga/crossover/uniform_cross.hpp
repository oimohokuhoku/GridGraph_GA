#pragma once

#include "i_crosser.hpp"

class UniformCross: public ICrosser {
public:
    void cross(const Individual& parentA, const Individual& parentB, Individual& child) override;

private:
    void setSelectNode(bool* selectNodeA, bool* selectNodeB);
    void obtainUniqueEdge(Individual& child, const Individual& parent, bool const *const selectNode);
    void obtainShareEdge(Individual& child, const Individual& parentA, const Individual& parentB);
    void obtainOtherEdge(Individual& child, const Individual& parentA, const Individual& parentB);
};