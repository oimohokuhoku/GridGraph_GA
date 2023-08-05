#include "resx.hpp"
#include "../individual.hpp"
#include "../grid_graph_base.hpp"

void RESX::cross(const Individual& parentA, const Individual& parentB, Individual& child) {
    child.clear();

    int loop = 0;
    for(int loop = 0; loop < 1000; loop++) {
        int nodeA, nodeB;

        if(loop % 2 == 0)
            GetRandomEdge(parentA, &nodeA, &nodeB);
        else 
            GetRandomEdge(parentB, &nodeA, &nodeB);

        if(nodeA == nodeB) continue;
        if(child.degrees[nodeA] == degree()) continue;
        if(child.degrees[nodeB] == degree()) continue;
        if(child.haveEdge(nodeA, nodeB)) continue;

        child.adjacent[nodeA][child.degrees[nodeA]] = nodeB;
        child.degrees[nodeA]++;
        child.adjacent[nodeB][child.degrees[nodeB]] = nodeA;
        child.degrees[nodeB]++;
    }

    modifyGraph(child);
}

void RESX::GetRandomEdge(const Individual& parent, int* nodeA, int* nodeB) {
    int nA = randInt(0, nodeNum());
    int d = randInt(0, parent.degrees[nA]);
    *nodeA = nA;
    *nodeB = parent.adjacent[nA][d];
}
