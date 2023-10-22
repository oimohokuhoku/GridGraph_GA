#include <iostream>
#include <climits>
#include "adj_aspl.hpp"
#include "../individual.hpp"
#include "../../other/random.hpp"
#include "ga/other/meta_observer.hpp"

ADJ_ASPL::ADJ_ASPL(): _segmentNum((numNode() + (BIT_LENGTH - 1)) / BIT_LENGTH), _matrixSize(numNode() * numNode()) {
	_linkMatrix = new uint64_t*[numNode()];
	for(int i =0 ; i < numNode(); ++i) _linkMatrix[i] = new uint64_t[_segmentNum];

	_nextMatrix = new uint64_t*[numNode()];
	for(int i =0 ; i < numNode(); ++i) _nextMatrix[i] = new uint64_t[_segmentNum];

	this->_dislinkedNode = -1;
}

ADJ_ASPL::~ADJ_ASPL() {
	for(int i = 0; i < numNode(); ++i) delete[] _linkMatrix[i];
	for(int i = 0; i < numNode(); ++i) delete[] _nextMatrix[i];
	delete[] _linkMatrix;
	delete[] _nextMatrix;
}

void ADJ_ASPL::operator() (Individual& indiv) {
	MetaObserver::countNumAsplEvaluation();
    long distanceSum = _matrixSize - numNode();

	setIdentityMatrix(_linkMatrix);
	setIdentityMatrix(_nextMatrix);

	int hop = 1;
	while(hop < numNode()) {
		step(indiv, _linkMatrix, _nextMatrix);

		long noLinkNum = countZero(_nextMatrix);
		
		if(noLinkNum == 0) break;

		distanceSum += noLinkNum;
		hop++;

		uint64_t** temp = _linkMatrix;
		_linkMatrix = _nextMatrix;
		_nextMatrix = temp;
	}

	if(hop == numNode()) {
		indiv.diameter = INT_MAX; 
		indiv.aspl = INT_MAX;
		this->_dislinkedNode = findDislinkedNode(_linkMatrix);
		return;
	}

	indiv.diameter = hop;
	indiv.aspl = (double)distanceSum / (numPair() * 2);
	this->_dislinkedNode = -1;
}

int ADJ_ASPL::dislinkedNode() {
	return this->_dislinkedNode;
}


void ADJ_ASPL::setIdentityMatrix(uint64_t *__restrict *__restrict matrix) {
	for(int i = 0; i < numNode(); ++i) {
		for(int j = 0; j < _segmentNum; ++j) {
			matrix[i][j] = 0;
		}
	}

	int bitPos = BIT_LENGTH - 1;
	int segmentIndex = 0;
	for(int i = 0; i < numNode(); ++i) {
		matrix[i][segmentIndex] = (0x1ULL << bitPos);

		--bitPos;

		if(bitPos < 0) {
			segmentIndex++;
			bitPos = BIT_LENGTH - 1;
		}
	}
}

void ADJ_ASPL::step(const Individual& indiv, uint64_t *__restrict *__restrict currentMatrix, uint64_t *__restrict *__restrict nextMatrix){
	for(int n = 0; n < numNode(); ++n) {
			int deg = indiv.degrees[n];
			for(int d = 0; d < deg; ++d) {
				int targetNode = indiv.adjacent[n][d];
				for(int seg = 0; seg < _segmentNum; ++seg) {
					nextMatrix[n][seg] |= currentMatrix[targetNode][seg];
				}
			}
		}
}

int ADJ_ASPL::countZero(uint64_t *__restrict *__restrict matrix) {
	int popNum = 0;
	for(int row = 0; row < numNode(); ++row) {
		for(int seg = 0; seg < _segmentNum; ++seg) {
			popNum += __builtin_popcountl(_nextMatrix[row][seg]);
		}
	}

	return _matrixSize - popNum;
}

int ADJ_ASPL::findDislinkedNode(uint64_t *__restrict *__restrict matrix) {
	for(int seg = 0; seg < _segmentNum; ++seg) {
		for(int bit = BIT_LENGTH - 1; bit >= 0; --bit) {
			uint64_t mask = ((uint64_t)1 << bit);
			uint64_t n = mask & matrix[0][seg];
			if(n == 0) return (seg * BIT_LENGTH + (BIT_LENGTH - bit)) - 1;
		}
	}
	
	std::cerr << "Error Evaluater::findDislinkedNode()\n" << std::endl;
	std::cerr << "This graph is linked, but this jadge being dislinked" << std::endl;
	exit(1);
}
