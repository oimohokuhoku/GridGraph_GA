#include <cstdlib>
#include <iostream>
#include "two_opt.hpp"
#include "../individual.hpp"
#include "../../other/common.hpp"

void TwoOpt::edit(Individual& indiv) {
    if(_INDIV_MUTATE_PROBABILITY < randDouble()) return;

    for(int i = 0; i < nodeNum(); ++i) {
        if(_GENE_MUTATE_PROBABILITY < randDouble()) continue;
        twoOpt(indiv, i);
    }
}

void TwoOpt::twoOpt(Individual& indiv) {
	int rand = randInt(0, nodeNum());
	twoOpt(indiv, rand);
}

void TwoOpt::twoOpt(Individual& indiv, int targetNode) {
	/****************************/
	/* nA1   nB1      nA1 - nB1 */
	/*  |     |   →             */
	/* nA2   nB2      nA2 - nB2 */
	/****************************/
	int nA1, nA2, nB1, nB2;

	nA1 = targetNode;
	nB1 = getNearNode(nA1);

	int loopCount = 0;
	int maxLoop = length() * length();
	while(!getReconnectableEdge(indiv, nA1, nB1, &nA2, &nB2)) {
		nB1 = getNearNode(nA1);

		//avoid infinite loop
		loopCount++;
		if(maxLoop < loopCount) return;
	}

	twoOpt(indiv, nA1, nA2, nB1, nB2);
}

void TwoOpt::twoOpt(Individual& indiv, int nodeA1, int nodeA2, int nodeB1, int nodeB2) {
	int iA1, iA2, iB1, iB2;

	getIndex(indiv, nodeA1, nodeA2, &iA1, &iA2);
	getIndex(indiv, nodeB1, nodeB2, &iB1, &iB2);

	indiv.adjacent[nodeA1][iA1] = nodeB1;
	indiv.adjacent[nodeA2][iA2] = nodeB2;
	indiv.adjacent[nodeB1][iB1] = nodeA1;
	indiv.adjacent[nodeB2][iB2] = nodeA2;
}

void TwoOpt::twoOpt_safe(Individual& indiv, int nodeA1, int nodeA2, int nodeB1, int nodeB2) {
	if(!indiv.haveEdge(nodeA1, nodeA2)) { std::cerr << "2-opt Error1: " << nodeA1 << "-" << nodeA2 << " isn't exists" << endl; indiv.showNodes(); exit(0); }
	if(!indiv.haveEdge(nodeB1, nodeB2)) { std::cerr << "2-opt Error2: " << nodeB1 << "-" << nodeB2 << " isn't exists" << endl; exit(0); }
	if(length() < getLength(nodeA1, nodeB1)) { std::cerr << "2-opt Error3: " << nodeA1 << " and " << nodeB1 << " is so far" << endl; exit(0); }
	if(length() < getLength(nodeA2, nodeB2)) { std::cerr << "2-opt Error4: " << nodeA2 << " and " << nodeB2 << " is so far" << endl; exit(0); }

	twoOpt(indiv, nodeA1, nodeA2, nodeB1, nodeB2);
}

void TwoOpt::getIndex(const Individual& indiv, int nodeA, int nodeB, int* indexA, int* indexB) {
	if(nodeA != nodeB) {
		for(int i = 0; i < degree(); ++i) {
			if(indiv.adjacent[nodeA][i] == nodeB) {
				*indexA = i;
				break;
			}
		}

		for(int i = 0; i < degree(); ++i) {
			if(indiv.adjacent[nodeB][i] == nodeA) {
				*indexB = i;
				break;
			}
		}

		return;
	}
	else if(nodeA == nodeB) {
		int i = 0;
		for(i = 0; i < degree(); ++i) {
			if(indiv.adjacent[nodeA][i] == nodeB) {
				*indexA = i;
				break;
			}
		}

		for(i++; i < degree(); ++i) {
			if(indiv.adjacent[nodeB][i] == nodeA) {
				*indexB = i;
				break;
			}
		}
		return;
	}
}

int TwoOpt::getNearNode(int srcNode) {
	int srcColumn = getColumn(srcNode);
	int srcRow = getRow(srcNode);
	int resultNode;

	int lowerColumnRange = srcColumn - length();
	int upperColumnRange = srcColumn + length();
	if(lowerColumnRange < 0) lowerColumnRange  = 0;
	if(columnNum() < upperColumnRange) upperColumnRange = columnNum();

	while(true) {
		int column, row;

		column = randInt(lowerColumnRange, upperColumnRange);

		int dColumn = srcColumn - column;
		if(dColumn < 0) dColumn *= -1;

		int remainDistance = length() - dColumn;
		
		if(remainDistance == 0) {
			row = srcRow;
		}
		else {
			int lowerRowRange = srcRow - remainDistance;
			int upperRowRange = srcRow + remainDistance;
			if(lowerRowRange < 0) lowerRowRange = 0;
			if(rowNum() < upperRowRange) upperRowRange = rowNum();

			row = randInt(lowerRowRange, upperRowRange);
		}

		resultNode = fromAxis(column, row);

		if(resultNode != srcNode) break;
	}

	return resultNode;
}

bool TwoOpt::getReconnectableEdge(const Individual& indiv, int startNodeA, int startNodeB, int* endNodeA, int* endNodeB) {
	int iA = randInt(0, degree());
	int iB = randInt(0, degree());

	for(int a = 0; a < degree(); ++a) {
		iA = (iA + 1) % degree();
		*endNodeA = indiv.adjacent[startNodeA][iA];

		if(*endNodeA == -1) continue;

		for(int b = 0; b < degree(); ++b) {
			iB = (iB + 1) % degree();
			*endNodeB = indiv.adjacent[startNodeB][iB];

			if(*endNodeB == -1) continue;

			if(getLength(*endNodeA, *endNodeB) <= length()) return true;
		}
	}

	return false;
}
