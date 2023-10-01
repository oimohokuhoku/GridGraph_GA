#pragma once

#include "mutater_base.hpp"
#include "../grid_graph_base.hpp"

class TwoOpt: private Mutater_Base, public GridGraph {
public:
    TwoOpt() = default;
    ~TwoOpt() = default;
    void operator() (Individual& indiv) override;
    void twoOpt(Individual& indiv);
    void twoOpt(Individual& indiv, int targetNode);
    void twoOpt(Individual& indiv, int nodeA1, int nodeA2, int nodeB1, int nodeB2);
    void twoOpt_safe(Individual& indiv, int nodeA1, int nodeA2, int nodeB1, int nodeB2);
    void temp(int node);

private:
    void getIndex(const Individual& indiv, int nodeA, int nodeB, int* indexA, int* indexB);
    int getNearNode(int srcNode);
    bool getReconnectableEdge(const Individual& indiv, int startNodeA, int startNodeB, int* indexA, int* indexB);

private: /* prohibit */
    TwoOpt(TwoOpt& obj) = delete;
    TwoOpt(TwoOpt&& obj) = delete;
    TwoOpt& operator=(TwoOpt& obj) = delete;
    TwoOpt& operator=(TwoOpt&& obj) = delete;
};
