#include "crossover.hpp"
#include "../individual.hpp"
#include "../grid_graph_base.hpp"
#include "../../other/random.hpp"

void SimpleEdgeSelectCrossover::operator() (const Individual& parentA, const Individual& parentB, Individual& child) {
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

void SimpleEdgeSelectCrossover::GetRandomEdge(const Individual& parent, int* nodeA, int* nodeB) {
    Random random;
    int nA = random.randomInt(0, numNode());
    int d = random.randomInt(0, parent.degrees[nA]);
    *nodeA = nA;
    *nodeB = parent.adjacent[nA][d];
}
