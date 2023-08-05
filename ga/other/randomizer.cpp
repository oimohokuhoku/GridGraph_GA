#include "randomizer.hpp"
#include "../individual.hpp"
#include "../mutate/two_opt.hpp"

void Randomizer::randomize(Individual& indiv) {
    int v1, v2;

	//connect adjast edges
	for (int column = 0; column + 1 < columnNum(); column += 2) {
		for (int row = 0; row < rowNum(); row++) {
			v1 = fromAxis(column, row);
			v2 = v1 + 1;
			for (int i = 0; i < degree(); ++i) {
				indiv.adjacent[v1][i] = v2;
				indiv.adjacent[v2][i] = v1;
			}
		}
	}

	if (columnNum() % 2 == 1) {
		int rightColumn = columnNum() - 1;
		for (int row = 0; row + 1 < rowNum(); row += 2) {
			v1 = fromAxis(rightColumn, row);
			v2 = v1 + columnNum();
			for (int i = 0; i < degree(); ++i) {
				indiv.adjacent[v1][i] = v2;
				indiv.adjacent[v2][i] = v1;
			}
		}

		if (rowNum() % 2 == 1) {
			v1 = fromAxis(rightColumn, rowNum() - 1);
			for (int i = 0; i < degree(); ++i) {
				indiv.adjacent[v1][i] = v1;
			}
		}
	}

	for (int i = 0; i < nodeNum(); ++i) indiv.degrees[i] = degree();

	/* 2-opt randomize */
	TwoOpt mutater;
	for (int i = 0; i < degree() * 4; ++i) {
		for (int j = 0; j < nodeNum(); j++) {
			mutater.twoOpt(indiv, j);
		}
	}
}
