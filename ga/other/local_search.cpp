#include "local_search.hpp"
#include "../individual.hpp"
#include "../mutate/two_opt.hpp"
#include "../evaluate/adj_aspl.hpp"
#include "meta_observer.hpp"
#include "../../other/random.hpp"
#include <iostream>

LocalSearch::LocalSearch() {
    _nearNodes = new int[length() * length() * 4];
}

LocalSearch::~LocalSearch() {
    delete[] this->_nearNodes;
}

void LocalSearch::doLocalSearch(Individual& indiv, int maxAsplCalc) {
    TwoOpt twoOpter;
    ADJ_ASPL evaluater;
    Random random;
    int start = random.randomInt(0, numNode());
    int nodeA1 = start;
    Individual beforeIndiv = indiv;

    int numAsplCalc = 0;

    do {
        collectNearNodes(nodeA1, _nearNodes);

        for(int i = 0; _nearNodes[i] != -1; ++i) {
            int nodeB1 = _nearNodes[i];

            for(int dA = 0; dA < indiv.degrees[nodeA1]; ++dA) {
                int nodeA2 = indiv.adjacent[nodeA1][dA];

                for(int dB = 0; dB < indiv.degrees[nodeB1]; ++dB) {
                    int nodeB2 = indiv.adjacent[nodeB1][dB];

                    if(length() < getLength(nodeA2, nodeB2)) continue;

                    twoOpter.twoOpt(indiv, nodeA1, nodeA2, nodeB1, nodeB2);
                    evaluater(indiv);
                    numAsplCalc++;

                    if(beforeIndiv <= indiv) {
                        indiv = beforeIndiv;
                        if(maxAsplCalc < numAsplCalc) return;
                        continue;
                    }
                    
                    beforeIndiv = indiv;
                    start = nodeA1;
                    break;
                }
            }
        }

        nodeA1 = (nodeA1 + 1) % numNode();
    } while(start != nodeA1);

    MetaObserver::calcNumAsplEvaluation(numAsplCalc);
}

void LocalSearch::collectNearNodes(int srcNode, int* output) {
    int srcRow = getRow(srcNode);
    int srcColumn = getColumn(srcNode);

    int top = srcRow - length();
    int bottom = srcRow + length() + 1;

    if(top < 0) top = 0;
    if(numRow() < bottom) bottom = numRow();

    int index = 0;
    for(int row = top; row < bottom; ++row) {
        int rowDiff = srcRow - row;
        if(rowDiff < 0) rowDiff *= -1;

        int remainLength = length() - rowDiff;
        int left = srcColumn - remainLength;
        int right = srcColumn + remainLength + 1;
        if(left < 0) left = 0;
        if(numColumn() < right) right = numColumn();

        for(int col = left; col < right; ++col) {
            output[index] = fromAxis(col, row);
            index++;
        }
    }

    output[index] = -1;
}