#pragma once

#include <cstdint>
#include "ievaluater.hpp"
#include "../i_gene_editor.hpp"

class ADJ_ASPL: public IEvaluater, public IGeneEditor { 
public:
    ADJ_ASPL();
    ~ADJ_ASPL();
    void edit(Individual& indiv);
    int dislinkedNode() override { return this->_dislinkedNode; }

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