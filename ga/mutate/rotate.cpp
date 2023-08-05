#include "rotate.hpp"
#include "../individual.hpp"
#include "../../other/common.hpp"
#include <iostream>
#include <memory>
using std::unique_ptr;

void Rotate::edit(Individual& indiv) {
    double r = randDouble();
    if(r >= _INDIV_MUTATE_PROBABILITY) return;

    if(rowNum() == columnNum()) {
        int rand = randInt(1, 6);

        if(rand == 1) {
            if(!_rotatedNode90) setRotatedNode90();
            convert(indiv, _rotatedNode90);
            return;
        }
        if(rand == 2) {
            if(!_rotatedNode180) setRotatedNode180();
            convert(indiv, _rotatedNode180);
            return;
        }
        if(rand == 3) {
            if(!_rotatedNode270) setRotatedNode270();
            convert(indiv, _rotatedNode270);
            return;
        }
        if(rand == 4) {
            if(!_turnedNodeVertical) setTurnedNodeVertical();
            convert(indiv, _turnedNodeVertical);
            return;
        }
        if(rand == 5) {
            if(!_turnedNodeHorizontal) setTurnedNodeHorizontal();
            convert(indiv, _turnedNodeHorizontal);
            return;
        }
    }
    else {
        int rand = randInt(1, 4);

        if(rand == 1) {
            if(!_rotatedNode180) setRotatedNode180();
            convert(indiv, _rotatedNode180);
            return;
        }
        if(rand == 2) {
            if(!_turnedNodeVertical) setTurnedNodeVertical();
            convert(indiv, _turnedNodeVertical);
            return;
        }
        if(rand == 3) {
            if(!_turnedNodeHorizontal) setTurnedNodeHorizontal();
            convert(indiv, _turnedNodeHorizontal);
            return;
        }
    }
}

void Rotate::convert(Individual& indiv, unique_ptr<int[]>& convertedNode) {
    int** oldEdges = indiv.adjacent;
    int* oldDegrees = indiv.degrees;
    indiv.adjacent = newArray_2D<int>(nodeNum(), degree());
    indiv.degrees = new int[nodeNum()];

    for(int n1 = 0; n1 < nodeNum(); ++n1) {
        int n2 = convertedNode[n1];

        int deg = oldDegrees[n1];
        indiv.degrees[n2] = deg;

        for(int d = 0; d < deg; ++d) {
            indiv.adjacent[n2][d] = convertedNode[oldEdges[n1][d]];
        }
        for(int d = deg; d < degree(); ++d) {
            indiv.adjacent[n2][d] = -1;
        }
    }

    deleteArray_2D(oldEdges, nodeNum());
    delete[] oldDegrees;
}

void Rotate::setRotatedNode90() {
    _rotatedNode90 = make_unique<int[]>(nodeNum()); 

    int n = 0;
    for(int row = 0; row < rowNum(); ++row) {
        int rotColumn = (columnNum() - 1) - row;
        for(int column = 0; column < columnNum(); ++column) {
            int rotRow = column;
            _rotatedNode90[n] = fromAxis(rotColumn, rotRow);
            n++;
        }
    }
}

void Rotate::setRotatedNode180() {
    _rotatedNode180 = make_unique<int[]>(nodeNum()); 

    for(int n = 0; n < nodeNum(); ++n) {
        _rotatedNode180[n] = (nodeNum() - 1) - n;
    }
}

void Rotate::setRotatedNode270() {
    _rotatedNode270 = make_unique<int[]>(nodeNum()); 

    int n = 0;
    for(int row = 0; row < rowNum(); ++row) {
        int rotColumn = row;
        for(int column = 0; column < columnNum(); ++column) {
            int rotRow = (rowNum() - 1) - column;
            _rotatedNode270[n] = fromAxis(rotColumn, rotRow);
            n++;
        }
    }
}

void Rotate::setTurnedNodeVertical() {
    _turnedNodeVertical = make_unique<int[]>(nodeNum());

    int n = 0;
    for(int row = 0; row < rowNum(); ++row) {
        int turnRow = (rowNum() - 1) - row;
        for(int column = 0; column < columnNum(); ++column) {
            _turnedNodeVertical[n] = fromAxis(column, turnRow);
            n++;
        }
    }
}

void Rotate::setTurnedNodeHorizontal() {
    _turnedNodeHorizontal = make_unique<int[]>(nodeNum());

    int n = 0;
    for(int row = 0; row < rowNum(); ++row) {
        for(int column = 0; column < columnNum(); ++column) {
            int turnColumn = (columnNum() - 1) - column;
            _turnedNodeHorizontal[n] = fromAxis(turnColumn, row);
            n++;
        }
    }
}

unique_ptr<int[]> Rotate::_rotatedNode90;
unique_ptr<int[]> Rotate::_rotatedNode180;
unique_ptr<int[]> Rotate::_rotatedNode270;
unique_ptr<int[]> Rotate::_turnedNodeVertical;
unique_ptr<int[]> Rotate::_turnedNodeHorizontal;
