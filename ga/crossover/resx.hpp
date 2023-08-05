#pragma once

#include "i_crosser.hpp"

class RESX : public ICrosser {
public:
    RESX() =default;
    ~RESX() = default;
    void cross(const Individual& parentA, const Individual& parentB, Individual& child) override;
private:
    void GetRandomEdge(const Individual& parent, int* nodeA, int* nodeB);
};