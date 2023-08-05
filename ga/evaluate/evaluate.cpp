#include<cstdlib>
#include<iostream>
#include<ios>
#include<iomanip>
#include<intrin.h>
using namespace std;

#include "evaluate.hpp"
#include "../../other/int_queue.hpp"
#include "../../other/common.hpp"

static std::string to_binString(uint64_t val)
{
	std::string str;

    if(val == 0) {
		for(int i = 0; i < 64; ++i) {
			str.insert(str.begin(), '0');
		}
		return str;
	}

		while( val > 0 ) {
        	if( (val & 1) == 0 )  // val は偶数か？
            	str.insert(str.begin(), '0');  //  偶数の場合
        	else
            	str.insert(str.begin(), '1');  //  奇数の場合
        	val >>= 1;
    	}
		while(str.length() < 64) str.insert(str.begin(), '0');

    return str;
}

static void show(uint64_t** matrix) {
	cout << endl;
	for(int i = 0; i < 100; ++i) {
		for(int j = 0; j < 2; ++j) {
			cout << to_binString(matrix[i][j]) << " ";
		}
		cout << endl;
	}
}


Evaluater::Evaluater(): _segmentNum((nodeNum() + (BIT_LENGTH - 1)) / BIT_LENGTH), _matrixSize(nodeNum() * nodeNum()) {
	this->_linkMatrix = newArray_2D<uint64_t>(nodeNum(), _segmentNum);
	this->_nextMatrix = newArray_2D<uint64_t>(nodeNum(), _segmentNum);
	this->_dislinkedNode = -1;
}

Evaluater::~Evaluater() {
	deleteArray_2D(this->_linkMatrix, nodeNum());
	deleteArray_2D(this->_nextMatrix, nodeNum());
}

void Evaluater::evaluate(int *__restrict *__restrict edges, int *__restrict degrees, int* diameterOut, double* asplOut) {
	ADJ_ASPL(edges, degrees, diameterOut, asplOut);
	//return;

	/*---------------------------------------------------------*/
	// this->_dislinkedNode = -1;

	// long distanceSum = 0;
	// int diameter = 0;

	// for (int node = 0; node < nodeNum() - 1; ++node) {
	// 	int tempDiam;
	// 	long tempDist;
	// 	calcDistanceSum(edges, node, &tempDiam, &tempDist);

	// 	if(_dislinkedNode != -1) {
	// 		*asplOut = INF;
	// 		*diameterOut = INF;
	// 		return;
	// 	}

	// 	distanceSum += tempDist;
	// 	if(diameter < tempDiam) diameter = tempDiam;
	// }

	// *asplOut = (double)distanceSum / pairNum();
	// *diameterOut = diameter;
}

int Evaluater::getDislinkedNode() {
	return _dislinkedNode;
}


void Evaluater::ADJ_ASPL(int *__restrict *__restrict edges, int *__restrict degrees, int* diameterOut, double* asplOut) {
	long distanceSum = _matrixSize - nodeNum();

	setIdentityMatrix(_linkMatrix);
	setIdentityMatrix(_nextMatrix);

	int hop = 1;
	while(hop < nodeNum()) {
		step(edges, degrees, _linkMatrix, _nextMatrix);

		long noLinkNum = countZero(_nextMatrix);
		
		if(noLinkNum == 0) break;

		distanceSum += noLinkNum;
		hop++;

		uint64_t** temp = _linkMatrix;
		_linkMatrix = _nextMatrix;
		_nextMatrix = temp;
	}

	if(hop == nodeNum()) {
		*diameterOut = INF;
		*asplOut = INF;
		this->_dislinkedNode = findDislinkedNode(_linkMatrix);
		return;
	}

	*diameterOut = hop;
	*asplOut = (double)distanceSum / (pairNum() * 2);
	this->_dislinkedNode = -1;
}

void Evaluater::setIdentityMatrix(uint64_t *__restrict *__restrict matrix) {
	for(int i = 0; i < nodeNum(); ++i) {
		for(int j = 0; j < _segmentNum; ++j) {
			matrix[i][j] = 0;
		}
	}

	int bitPos = BIT_LENGTH - 1;
	int segmentIndex = 0;
	for(int i = 0; i < nodeNum(); ++i) {
		matrix[i][segmentIndex] = (0x1ULL << bitPos);

		--bitPos;

		if(bitPos < 0) {
			segmentIndex++;
			bitPos = BIT_LENGTH - 1;
		}
	}
}

void Evaluater::step(int *__restrict *__restrict edges, int *__restrict degrees, uint64_t *__restrict *__restrict currentMatrix, uint64_t *__restrict *__restrict nextMatrix){
	for(int n = 0; n < nodeNum(); ++n) {
			int deg = degrees[n];
			for(int d = 0; d < deg; ++d) {
				int targetNode = edges[n][d];
				for(int seg = 0; seg < _segmentNum; ++seg) {
					nextMatrix[n][seg] |= currentMatrix[targetNode][seg];
				}
			}
		}
}

int Evaluater::countZero(uint64_t *__restrict *__restrict matrix) {
	int popNum = 0;
	for(int row = 0; row < nodeNum(); ++row) {
		for(int seg = 0; seg < _segmentNum; ++seg) {
			popNum += __builtin_popcountl(_nextMatrix[row][seg]);
		}
	}
	return _matrixSize - popNum;
}

int Evaluater::findDislinkedNode(uint64_t *__restrict *__restrict matrix) {
	for(int seg = 0; seg < _segmentNum; ++seg) {
		for(int bit = BIT_LENGTH - 1; bit >= 0; --bit) {
			uint64_t mask = ((uint64_t)1 << bit);
			uint64_t n = mask & matrix[0][seg];
			if(n == 0) return (seg * BIT_LENGTH + (BIT_LENGTH - bit)) - 1;
		}
	}
	
	std::cerr << "Error Evaluater::findDislinkedNode()\n" << endl;
	std::cerr << "This graph is linked, but this jadge being dislinked" << endl;
	exit(1);
}

void Evaluater::calcDistanceSum(int *__restrict *__restrict edges, int startNode, int* maxDistance, long* distSum) {
	bool* visited = new bool[nodeNum()];
	for (int i = 0; i < nodeNum(); ++i) visited[i] = false;

	visited[startNode] = true;
	int visitCount = startNode + 1;
	int depth = 1;
	*distSum = 0;

	IntQueue queue(nodeNum());
	queue.enqueue(startNode);

	/* BFS */
	while (queue.count() > 0) {
		int leafNum = 0;
		int rootNum = queue.count();

		for (int i = 0; i < rootNum; ++i) {
			int n1 = queue.dequeue();

			for (int d = 0; d < degree(); ++d) {
				int n2 = edges[n1][d];

				if (n2 == -1) continue;
				if (visited[n2]) continue;

				visited[n2] = true;
				queue.enqueue(n2);

				if (n2 <= startNode) continue;

				visitCount++;
				leafNum++;

				if (visitCount == nodeNum()) {
					*distSum += leafNum * depth;
					*maxDistance = depth;
					delete[] visited;
					return;
				}
			}
		}

		*distSum += leafNum * depth;
		depth++;
	}

	/* graph is dislinked -------------------------- */
	for (int i = 0; i < nodeNum(); ++i) {
		if (!visited[i]) {
			_dislinkedNode = i;
			break;
		}
	}

	*maxDistance = INF;
	*distSum = INF;
	delete[] visited;
	return;
}
