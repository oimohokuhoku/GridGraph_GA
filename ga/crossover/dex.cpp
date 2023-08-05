#include "dex.hpp"
#include "../individual.hpp"
#include "../../other/common.hpp"

void DevideExchangeCross::cross(const Individual& parentA, const Individual& parentB, Individual& child) {
    child.clear();
    int numDevideRow;
    int numDevideColumn;

    if(1 + (rowNum() / 10) == 1) numDevideRow = 1;
    else numDevideRow = randInt(1, 1 + (rowNum() / 10));

    if(1 + (columnNum() / 10) == 1) numDevideColumn = 1;
    else numDevideColumn = randInt(1, 1 + (columnNum() / 10));

    int* devideRows = decideDevideLine(rowNum(), numDevideRow);
    int* devideColumns = decideDevideLine(columnNum(), numDevideColumn);

    numDevideRow += 2;
    numDevideColumn += 2;

    for(int row = 0; row + 1 < numDevideRow; row++) {
        int top = devideRows[row];
        int bottom = devideRows[row + 1];
        for(int column = 0; column + 1 < numDevideColumn; ++column) {
            int left = devideColumns[column];
            int right = devideColumns[column + 1];
            const Individual* parent = (randInt(0, 2) == 1)? &parentA : &parentB;
            obtainEdgeWithinRange(*parent, top, bottom, left, right, child);
        }
    }

    obtainDuplicateEdge(parentA, parentB, child);

    obtainOtherEdge(parentA, parentB, child);

    modifyGraph(child);

    delete[] devideRows;
    delete[] devideColumns;
}

int* DevideExchangeCross::decideDevideLine(int width, int numDevideLine) {
    int* devideLines;

    if(numDevideLine == 1) {
        devideLines = new int[3];
        devideLines[0] = 0;
        devideLines[1] = randInt(1, width - 1);
        devideLines[2] = width;
        return devideLines;
    }

    devideLines = new int[numDevideLine + 2];
    devideLines[0] = 0;
    for(int line = 1; line <= numDevideLine; ++line) {
        while(true) {
            int r = randInt(1, width - 1);

            bool dupli = false;
            for(int i = 1; i < line; ++i) {
                if(devideLines[i] != r) continue;
                dupli = true;
                break;
            }

            if(dupli) continue;

            devideLines[line] = r;
            break;
        }
    }
    devideLines[numDevideLine + 1] = width;

    //bubble sort
    for(int head = 1; head + 1 <= numDevideLine; ++head) {
        for(int i = head; i + 1 <= numDevideLine; i++) {
            if(devideLines[i] < devideLines[i + 1]) continue;
            int temp = devideLines[i];
            devideLines[i] = devideLines[i + 1];
            devideLines[i + 1] = temp;
        }
    }

    return devideLines;
}

void DevideExchangeCross::obtainEdgeWithinRange(const Individual& parent, int top, int bottom, int left, int right, Individual &child) {
    for(int row = top; row < bottom; ++row) {
        for(int column = left; column < right; ++column) {
            int nodeA = fromAxis(column, row);

            for(int d = 0; d < degree(); ++d) {
                int nodeB = parent.adjacent[nodeA][d];
                int rowB = getRow(nodeB);
                int columnB = getColumn(nodeB);
                
                if(rowB < top) continue;
                if(bottom <= rowB) continue;
                if(columnB < left) continue;
                if(right <= columnB) continue;

                child.addEdge(nodeA, nodeB);

            }
        }
    }
}

void DevideExchangeCross::obtainDuplicateEdge(const Individual& parentA, const Individual& parentB, Individual& child) {
    for(int node1 = 0; node1 < nodeNum(); ++node1) {
        for(int dA = 0; dA < degree(); ++dA) {
            int nodeA = parentA.adjacent[node1][dA];
            for(int dB = 0; dB < degree(); ++dB) {
                int nodeB = parentB.adjacent[node1][dB];
                if(nodeA != nodeB) continue;

                int node2 = nodeA;
                if(node1 == node2) continue;
                if(child.haveEdge(node1, node2)) continue;

                child.addEdge(node1, node2);
            }
        }
    }
}

void DevideExchangeCross::obtainOtherEdge(const Individual& parentA, const Individual& parentB, Individual& child) {
    for(int node1 = 0; node1 < nodeNum(); ++node1) {
        if(child.degrees[node1] == degree()) continue;

        const Individual* parent;
        int rand = randInt(0, 2);
        if(rand == 0) parent = &parentA;
        else          parent = &parentB;

        for(int d = 0; d < degree(); ++d) {
            int node2 = parent->adjacent[node1][d];

            if(node1 == node2) continue;
            if(child.degrees[node2] == degree()) continue;
            if(child.haveEdge(node1, node2)) continue;

            child.addEdge(node1, node2);
            if(child.degrees[node1] == degree()) break;
        }
    }
}