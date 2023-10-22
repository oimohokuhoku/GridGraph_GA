#include "crossover.hpp"
#include <math.h>
#include "ga/individual.hpp"
#include "ga/evaluate/adj_aspl.hpp"
#include "other/array_util.hpp"
#include "other/int_queue.hpp"

DMSX::DMSX(int unitWidth) : 
    _UNIT_WIDTH(unitWidth),
    _NUM_UNIT_COLUMN(ceill((double)numColumn() / unitWidth)), 
    _NUM_UNIT_ROW(ceill((double)numRow() / unitWidth)) 
{
    exchandegEdgeInStartParent = ArrayUtil::newArray2D<bool>(numNode(), degree(), false);
    exchandegEdgeInEndParent = ArrayUtil::newArray2D<bool>(numNode(), degree(), false);
}

DMSX::~DMSX() {
    ArrayUtil::deleteArray2D<bool>(exchandegEdgeInStartParent, numNode());
    ArrayUtil::deleteArray2D<bool>(exchandegEdgeInEndParent, numNode());
}

void DMSX::operator() (const Individual& startParent, const Individual& endParent, Individual& bestChild) {
    ADJ_ASPL evaluate;
    const int maxStep = 10;
    const int numUnit = 10;
    Individual baseIndiv = startParent;
    
    for(int step = 0; step < maxStep; ++step) {
        Individual bestNextIndiv;
        for(int unitIndex = 0; unitIndex < numUnit; ++unitIndex) {
            Individual nextIndiv = baseIndiv;

            generateNeighbor(startParent, endParent, unitIndex, nextIndiv);
            evaluate(nextIndiv);

            if(nextIndiv.betterThan(bestNextIndiv)) bestNextIndiv = nextIndiv;
        }
        if(bestNextIndiv.betterThan(bestChild)) bestChild = bestNextIndiv;
        baseIndiv = bestNextIndiv;
    }
}

void DMSX::generateNeighbor(const Individual& startParent, const Individual& endParent, int unitIndex, Individual& baseIndiv) {
    int unitColumnIndex = unitIndex % _NUM_UNIT_COLUMN;
    int unitRowIndex = unitIndex / _NUM_UNIT_ROW;
    
    int unitLeft = unitColumnIndex * _UNIT_WIDTH;
    int unitRight = unitLeft + _UNIT_WIDTH;
    int unitTop = unitRowIndex * _UNIT_WIDTH;
    int unitBottom = unitTop + _UNIT_WIDTH;
    if(numColumn() < unitRight) unitRight = numColumn();
    if(numRow() < unitBottom) unitBottom = numRow();

    int* nodesInUnit = new int[_UNIT_WIDTH * _UNIT_WIDTH];
    int numNodesInUnit = 0;
    for(int row = unitTop; row < unitBottom; ++row) {
        int node = row * numColumn();
        for(int column = unitLeft; column < unitRight; ++column) {
            nodesInUnit[numNodesInUnit++] = node + column;
        }
    }

    int* abCycle = new int[numNode()];
    for(int i = 0; i < numNodesInUnit; ++i) {
        int node1 = nodesInUnit[i];
        for(int d = 0; d < startParent.degrees[node1]; ++d) {
            int node2 = endParent.adjacent[node1][d];
            int cycleLength = findShortestABCycle(startParent, endParent, node1, node2, abCycle);
            if(cycleLength == -1) continue;
            embedABCycle(abCycle, cycleLength, baseIndiv);
            recordEmbedded(abCycle, cycleLength, startParent, endParent);
        }
    }

    delete[] abCycle;
    delete[] nodesInUnit;
}

int DMSX::findShortestABCycle(const Individual& startParent, const Individual& endParent, int startEdgeNode1, int startEdgeNode2, int* abCycle) {
    const int START_PARENT = 0;
    const int END_PARENT = 1;

    IntQueue queue(numNode());
    bool* visited = ArrayUtil::newArray<bool>(numNode(), false);
    int* preNodes = ArrayUtil::newArray<int>(numNode());

    queue.enqueue(startEdgeNode1);
    visited[startEdgeNode1] = true;
    preNodes[startEdgeNode1] = -1;

    bool found = false;
    int targetParent = 0;
    while (!queue.empty()) {
        int numCurrentNode = queue.count();

        for(int i = 0; i < numCurrentNode; ++i) {
            int current = queue.dequeue();

            for (int d = 0; d < degree(); ++d) {
                int nextNode;
                if(targetParent == START_PARENT) {
                    if(exchandegEdgeInStartParent[current][d]) continue;
                    nextNode = startParent.adjacent[current][d];
                }
                else {
                    if(exchandegEdgeInEndParent[current][d]) continue;
                    nextNode = endParent.adjacent[current][d];
                }

                if (nextNode == startEdgeNode2 && targetParent == START_PARENT) {
                    preNodes[nextNode] = current;
                    int cycleLength = toPath(preNodes, startEdgeNode1, startEdgeNode2, abCycle);
                    delete[] visited;
                    delete[] preNodes;
                    return cycleLength;
                }

                if (visited[nextNode]) continue;

                queue.enqueue(nextNode);
                visited[nextNode] = true;
                preNodes[nextNode] = current;
            }
        }
        targetParent = (targetParent + 1) % 2;
    }

    delete[] visited;
    delete[] preNodes;
    return -1;
}

int DMSX::toPath(int* preNodes, int startNode, int endNode, int* path) {
    int pathLength = 0;
    int at = endNode;
    while (at != -1) {
        path[pathLength++] = at;
        at = preNodes[at];
    }
    return pathLength;
}

void DMSX::embedABCycle(int* abCycle, int cycleLength, Individual& baseIndiv) {
    if(cycleLength == 2) return;

    for(int i = 0; i < cycleLength; i += 2) {
        int node1 = abCycle[i];
        int node2 = abCycle[(i + 1) % cycleLength];
        baseIndiv.removeEdge(node1, node2);

        int deg1 = baseIndiv.getDegreeIndex(node1, node2);
        int deg2 = baseIndiv.getDegreeIndex(node2, node1);
        exchandegEdgeInStartParent[node1][deg1] = true;
        exchandegEdgeInStartParent[node2][deg2] = true;
    }

    for(int i = 1; i < cycleLength; i += 2) {
        int node1 = abCycle[i];
        int node2 = abCycle[(i + 1) % cycleLength];
        baseIndiv.addEdge(node1, node2);

        int deg1 = baseIndiv.getDegreeIndex(node1, node2);
        int deg2 = baseIndiv.getDegreeIndex(node2, node1);
        exchandegEdgeInEndParent[node1][deg1] = true;
        exchandegEdgeInEndParent[node2][deg2] = true;
    }
}

void DMSX::recordEmbedded(int* abCycle, int cycleLength, const Individual& startParent, const Individual& endParent) {
    for(int i = 0; i < cycleLength; i += 2) {
        int node1 = abCycle[i];
        int node2 = abCycle[(i + 1) % cycleLength];

        int deg1 = startParent.getDegreeIndex(node1, node2);
        int deg2 = startParent.getDegreeIndex(node2, node1);
        exchandegEdgeInStartParent[node1][deg1] = true;
        exchandegEdgeInStartParent[node2][deg2] = true;
    }

    for(int i = 1; i < cycleLength; i += 2) {
        int node1 = abCycle[i];
        int node2 = abCycle[(i + 1) % cycleLength];

        int deg1 = endParent.getDegreeIndex(node1, node2);
        int deg2 = endParent.getDegreeIndex(node2, node1);
        exchandegEdgeInEndParent[node1][deg1] = true;
        exchandegEdgeInEndParent[node2][deg2] = true;
    }
}
