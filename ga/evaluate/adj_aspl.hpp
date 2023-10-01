#pragma once

#include <cstdint>
#include "ievaluater.hpp"

class ADJ_ASPL: public IEvaluater { 
public:
    ADJ_ASPL();
    ~ADJ_ASPL();
    void operator() (Individual& indiv) override;
    int dislinkedNode() override;

private:
    static const int BIT_LENGTH = 64;
    const int _segmentNum;
	const int _matrixSize;
	uint64_t** _linkMatrix;
	uint64_t** _nextMatrix;
	
	int _dislinkedNode = 0;

    void setIdentityMatrix(uint64_t *__restrict *__restrict matrix);
    void step(const Individual& indiv, uint64_t *__restrict *__restrict currentMatrix, uint64_t *__restrict *__restrict nextMatrix);
    int countZero(uint64_t *__restrict *__restrict matrix);
    int findDislinkedNode(uint64_t *__restrict *__restrict matrix);
};