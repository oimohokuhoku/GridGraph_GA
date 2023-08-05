#pragma once

#include "i_crosser.hpp"

class DevideExchangeCross : public ICrosser {
public:
    void cross(const Individual& parentA, const Individual& parentB, Individual& child) override;
private:
    int* decideDevideLine(int width, int numDevideLine);
    void obtainEdgeWithinRange(const Individual& parent, int top, int bottom, int left, int right, Individual &child);
    void obtainDuplicateEdge(const Individual& parentA, const Individual& parentB, Individual& child);
    void obtainOtherEdge(const Individual& parentA, const Individual& parentB, Individual& child);
};