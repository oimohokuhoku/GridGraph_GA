#include "randomizer.hpp"
#include "../individual.hpp"
#include "ga/mutate/two_opt.hpp"
#include "ga/evaluate/adj_aspl.hpp"


void Randomizer::operator()(Individual& indiv) {
    setRandomEdge(indiv);

	ADJ_ASPL evaluater;
	TwoOpt twoOpt;
	while (true) {
		evaluater(indiv);
		
		int dislink = evaluater.dislinkedNode();
		if (dislink == -1) break;
		twoOpt.twoOpt(indiv, dislink);
	}
}

void Randomizer::setRandomEdge(Individual& indiv) {
	int v1, v2;

	//connect adjast edges
	for (int column = 0; column + 1 < numColumn(); column += 2) {
		for (int row = 0; row < numRow(); row++) {
			v1 = fromAxis(column, row);
			v2 = v1 + 1;
			for (int i = 0; i < degree(); ++i) {
				indiv.adjacent[v1][i] = v2;
				indiv.adjacent[v2][i] = v1;
			}
		}
	}

	if (numColumn() % 2 == 1) {
		int rightColumn = numColumn() - 1;
		for (int row = 0; row + 1 < numRow(); row += 2) {
			v1 = fromAxis(rightColumn, row);
			v2 = v1 + numColumn();
			for (int i = 0; i < degree(); ++i) {
				indiv.adjacent[v1][i] = v2;
				indiv.adjacent[v2][i] = v1;
			}
		}

		if (numRow() % 2 == 1) {
			v1 = fromAxis(rightColumn, numRow() - 1);
			for (int i = 0; i < degree(); ++i) {
				indiv.adjacent[v1][i] = v1;
			}
		}
	}

	for (int i = 0; i < numNode(); ++i) indiv.degrees[i] = degree();

	/* randomize edges */
	TwoOpt mutater;
	for (int i = 0; i < degree() * 4; ++i) {
		for (int j = 0; j < numNode(); j++) {
			mutater.twoOpt(indiv, j);
		}
	}
}
