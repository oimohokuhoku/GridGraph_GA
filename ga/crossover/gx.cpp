#include "crossover.hpp"
#include <iostream>
#include "../individual.hpp"
#include "../mutate/two_opt.hpp"
#include "../evaluate/adj_aspl.hpp"
#include "../../other/random.hpp"
#include "ga/other/meta_observer.hpp"

GraftingCrossover::GraftingCrossover(int maxNumLS) : _MAX_NUM_LS(maxNumLS) {
}

void GraftingCrossover::operator() (const Individual& parentA, const Individual& parentB, Individual& child) {
    child.clear();

    int* nodeBelonging = new int[numNode()];
    decideNodeBelonging(nodeBelonging);

    obtainEdgeWithinRange(parentA, parentB, nodeBelonging, child);
    obtainDuplicateEdge(parentA, parentB, child);
    obtainOtherEdge(parentA, parentB, child);
    modifyGraph(child);

    //localSearch(nodeBelonging, child);

    delete[] nodeBelonging;
}

void GraftingCrossover::decideNodeBelonging(int* storage) {
    Random random;
    int numRowDivision, numColumnDivision;

    int maxRowDivision = 1 + (numRow() / 10);
    numRowDivision = random.randomInt(1, maxRowDivision + 1);

    int maxColumnDivision = 1 + (numColumn() / 10);
    numColumnDivision = random.randomInt(1, maxColumnDivision + 1);

    numRowDivision += 2;      //add upper end and lower end
    numColumnDivision += 2;   //add left end and right end

    int* divideRows = decideDivideLine(numRow(), numRowDivision);
    int* divideColumns = decideDivideLine(numColumn(), numColumnDivision);
    
    for(int row = 0; row + 1 < numRowDivision; row++) {
        int top = divideRows[row];
        int bottom = divideRows[row + 1];

        for(int column = 0; column + 1 < numColumnDivision; ++column) {
            int left = divideColumns[column];
            int right = divideColumns[column + 1];

            int parent = random.randomInt(2);
            fill(top, bottom, left, right, parent, storage);
        }
    }

    delete[] divideRows;
    delete[] divideColumns;
}

int* GraftingCrossover::decideDivideLine(int width, int numDivision) {
    Random random;
    int* divideLines = new int[numDivision];

    //set ends of graph
    divideLines[0] = 0;
    divideLines[numDivision - 1] = width;

    int* number = new int[width];
    for(int i = 0; i < width; ++i) number[i] = i;

    //select without replacement from 1~(width-1) as division position
    for(int i = 1; i <= numDivision - 2; ++i) {
        int r = random.randomInt(1, width - i);
        divideLines[i] = number[r];

        int temp = number[width - i];
        number[width - i] = number[r];
        number[r] = temp;
    }
    delete[] number;

    //bubble sort
    for(int head = 0; head + 1 < numDivision; ++head) {
        for(int i = numDivision - 1; i - 1 >= head; --i) {
            if(divideLines[i - 1] < divideLines[i]) continue;
            int temp = divideLines[i];
            divideLines[i] = divideLines[i - 1];
            divideLines[i - 1] = temp;
        }
    }

    return divideLines;
}

void GraftingCrossover::obtainEdgeWithinRange(const Individual& parentA, const Individual& parentB, int* nodeBelonging, Individual& child) {
    for(int row = 0; row < numRow(); ++row) {
        int left = row * numColumn();

        for(int column = 0; column < numColumn(); ++column) {
            int nodeA = left + column;
            const Individual* parent = (nodeBelonging[nodeA] == 0) ? &parentA : &parentB;

            for(int d = 0; d < parent->degrees[nodeA]; ++d) {
                int nodeB = parent->adjacent[nodeA][d];
                if(nodeBelonging[nodeA] == nodeBelonging[nodeB])
                    child.addEdge(nodeA, nodeB);
            }
        }
    }
}

void GraftingCrossover::obtainDuplicateEdge(const Individual& parentA, const Individual& parentB, Individual& child) {
    //If nodeA1-nodeA2 matches nodeB1-nodeB2, this edge is added to child
    for(int nodeA1 = 0; nodeA1 < numNode(); ++nodeA1) {
        for(int dA = 0; dA < parentA.degrees[nodeA1]; ++dA) {
            int nodeA2 = parentA.adjacent[nodeA1][dA];
            
            int nodeB1 = nodeA1;
            for(int dB = 0; dB < parentB.degrees[nodeB1]; ++dB) {
                int nodeB2 = parentB.adjacent[nodeB1][dB];
                if(nodeA2 == nodeB2) child.addEdge(nodeA1, nodeB2);
            }
        }
    }
}

void GraftingCrossover::obtainOtherEdge(const Individual& parentA, const Individual& parentB, Individual& child) {
    Random random;
    for(int node1 = 0; node1 < numNode(); ++node1) {
        if(child.degrees[node1] == degree()) continue;

        const Individual* parent;
        int rand = random.randomInt(0, 2);
        if(rand == 0) parent = &parentA;
        else          parent = &parentB;

        for(int d = 0; d < degree(); ++d) {
            int node2 = parent->adjacent[node1][d];

            child.addEdge(node1, node2);
            if(child.degrees[node1] == degree()) break;
        }
    }
}

void GraftingCrossover::localSearch(int const *const nodeBelonging, Individual& child) {
    int* aroundBorderNodes = new int[numNode() + 1];
    int* nearNodes = new int[length() * length() * 4];
    TwoOpt mutate;
    ADJ_ASPL evaluate;

    collectBorderNodes(nodeBelonging, aroundBorderNodes);

    if(aroundBorderNodes[0] == -1) {
        delete[] aroundBorderNodes;
        delete[] nearNodes;
        return;
    }

    evaluate(child);
    Individual beforeIndiv = child;

    bool refine = false;
    int numCalcAspl = 0;
    int i = 0;
    while(true) {
        int nodeA1 = aroundBorderNodes[i];
        collectNearNodes(nodeA1, nearNodes, beforeIndiv);

        int j = 0;
        while(nearNodes[j] != -1) {
            int nodeB1 = nearNodes[j];
            
            for(int dA = 0; dA < child.degrees[nodeA1]; ++dA) {
                for(int dB = 0; dB < child.degrees[nodeB1]; ++dB) {
                    int nodeA2 = child.adjacent[nodeA1][dA];
                    int nodeB2 = child.adjacent[nodeB1][dB];

                    if(getLength(nodeA2, nodeB2) >= length()) continue;

                    mutate.twoOpt_safe(child, nodeA1, nodeA2, nodeB1, nodeB2);
                    evaluate(child);
                    numCalcAspl++;

                    if(child.betterThan(beforeIndiv)) { 
                        beforeIndiv = child; 
                        refine = true;
                    }
                    else {             
                        child = beforeIndiv; 
                    }

                    if(numCalcAspl > _MAX_NUM_LS) {
                        delete[] aroundBorderNodes;
                        delete[] nearNodes;
                        return;
                    }
                }
            }
            j++;
        }
        i++;

        if(aroundBorderNodes[i] == -1)  {
            if(!refine) break;
            refine = false;
            i = 0;
        }
    }
    delete[] aroundBorderNodes;
    delete[] nearNodes;
}

void GraftingCrossover::fill(int top, int bottom, int left, int right, int value, int* nodes) {
    int width = right - left;
    for(int row = top; row < bottom; ++row) {
        int node = fromAxis(left, row);
        for(int i = 0; i < width; ++i) {
            nodes[node + i] = value;
        }
    }
}

void GraftingCrossover::collectBorderNodes(int const *const nodeBelonging, int* result) {
    int borderRange = length();
    int index = 0;

    for(int nodeA = 0; nodeA < numNode(); ++nodeA) {
        bool isBorder = false;
        int columnA = getColumn(nodeA);
        int rowA = getRow(nodeA);

        //left
        for(int i = 1; i <= borderRange; ++i) {
            int columnB = columnA - i;
            int rowB = rowA;
            if(columnB < 0) break;

            int nodeB = fromAxis(columnB, rowB);
            if(nodeBelonging[nodeA] != nodeBelonging[nodeB]) {
                result[index++] = nodeA;
                isBorder = true;
                break;
            }
        }
        if(isBorder) continue;

        //right
        for(int i = 1; i <= borderRange; ++i) {
            int columnB = columnA + i;
            int rowB = rowA;
            if(numColumn() <= columnB) break;

            int nodeB = fromAxis(columnB, rowB);
            if(nodeBelonging[nodeA] != nodeBelonging[nodeB]) {
                result[index++] = nodeA;
                isBorder = true;
                break;
            }
        }
        if(isBorder) continue;

        //upper
        for(int i = 1; i <= borderRange; ++i) {
            int columnB = columnA;
            int rowB = rowA - i;
            if(rowB < 0) break;

            int nodeB = fromAxis(columnB, rowB);
            if(nodeBelonging[nodeA] != nodeBelonging[nodeB]) {
                result[index++] = nodeA;
                isBorder = true;
                break;
            }
        }
        if(isBorder) continue;

        //lower
        for(int i = 1; i <= borderRange; ++i) {
            int columnB = columnA;
            int rowB = rowA + i;
            if(numRow() <= rowB) break;

            int nodeB = fromAxis(columnB, rowB);
            if(nodeBelonging[nodeA] != nodeBelonging[nodeB]) {
                result[index++] = nodeA;
                isBorder = true;
                break;
            }
        }
        if(isBorder) continue;
    }

    result[index] = -1; //sentinel
}

void GraftingCrossover::collectNearNodes(int srcNode, int* result, Individual& test) {
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
            result[index] = fromAxis(col, row);
            index++;
        }
    }

    result[index] = -1;
}
