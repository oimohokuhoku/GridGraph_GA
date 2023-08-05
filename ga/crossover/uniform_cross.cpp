#include "uniform_cross.hpp"
#include "../individual.hpp"
#include "../../other/common.hpp"

void UniformCross::cross(const Individual& parentA, const Individual& parentB, Individual& child) {
    bool* selectNodeA = new bool[nodeNum()];
    bool* selectNodeB = new bool[nodeNum()];
    child.clear();

    setSelectNode(selectNodeA, selectNodeB);

    obtainUniqueEdge(child, parentA, selectNodeA);
    obtainUniqueEdge(child, parentB, selectNodeB);

    delete[] selectNodeA;
    delete[] selectNodeB;

    obtainShareEdge(child, parentA, parentB);
    obtainOtherEdge(child, parentA, parentB);

    modifyGraph(child);
}

void UniformCross::setSelectNode(bool* selectNodeA, bool* selectNodeB) {
    for(int i = 0; i < nodeNum(); ++i){
        int rand = randInt(0, 2);
        if(rand == 0) selectNodeA[i] = true;
        else          selectNodeA[i] = false;
    }

    for(int i = 0; i < nodeNum(); ++i){
        selectNodeB[i] = (!selectNodeA[i]);
    }
}

void UniformCross::obtainUniqueEdge(Individual& child, const Individual& parent, bool const *const selectNode) {
    for(int nodeA = 0; nodeA < nodeNum(); ++nodeA) {
        if(!selectNode[nodeA]) continue;

        int degree = parent.degrees[nodeA];
        for(int d = 0; d < degree; ++d) {
            int nodeB = parent.adjacent[nodeA][d];
            if(!selectNode[nodeB]) continue;

            child.addEdge(nodeA, nodeB);
        }
    }
}

void UniformCross::obtainShareEdge(Individual& child, const Individual& parentA, const Individual& parentB) {
    for(int node1 = 0; node1 < nodeNum(); ++node1) {
        if(child.degrees[node1] == degree()) continue;

        int degree = parentA.degrees[node1];
        for(int d = 0; d < degree; ++d) {
            int node2 = parentA.adjacent[node1][d];
            
            if(!parentB.haveEdge(node1, node2)) continue;

            child.addEdge(node1, node2);
        }
    }
}

void UniformCross::obtainOtherEdge(Individual& child, const Individual& parentA, const Individual& parentB) {
    for(int node1 = 0; node1 < nodeNum(); ++node1) {
        if(child.degrees[node1] == degree()) continue;

        const Individual* parent;
        int rand = randInt(0, 2);
        if(rand == 0) parent = &parentA;
        else          parent = &parentB;

        for(int d = 0; d < degree(); ++d) {
            int node2 = parent->adjacent[node1][d];
            child.addEdge(node1, node2);
            if(child.degrees[node1] == degree()) break;
        }
    }
}
