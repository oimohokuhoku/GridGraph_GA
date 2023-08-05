#pragma once

#include "grid_graph_base.hpp"

class Evaluater: public GridGraph {
public:
	Evaluater();
	~Evaluater();
	void evaluate(int *__restrict *__restrict edges, int *__restrict degrees, int* diameterOut, double* asplOut);
	int getDislinkedNode();

private:
	static const int BIT_LENGTH = 64;
	uint64_t** _linkMatrix;
	uint64_t** _nextMatrix;
	const int _segmentNum;
	const int _matrixSize;
	int _dislinkedNode = 0;

	void calcDistanceSum(int *__restrict *__restrict edges, int startNode, int* maxDistance, long* distSum);

	void ADJ_ASPL(int *__restrict *__restrict edges, int *__restrict degrees, int* diameterOut, double* asplOut);
	void setIdentityMatrix(uint64_t *__restrict *__restrict matrix);
	void step(int *__restrict *__restrict edges, int *__restrict degrees, uint64_t *__restrict *__restrict currentMatrix, uint64_t *__restrict *__restrict nextMatrix);
	int countZero(uint64_t *__restrict *__restrict matrix);
	int findDislinkedNode(uint64_t *__restrict *__restrict matrix);

private:
	Evaluater(Evaluater& obj) = delete;
	Evaluater(Evaluater&& obj) = delete;
	Evaluater& operator=(Evaluater& obj) = delete;
	Evaluater& operator=(Evaluater&& obj) = delete;
};