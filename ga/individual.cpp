#include "individual.hpp"
#include<iostream>
#include "../other/random.hpp"
using std::cout, std::endl;

Individual::Individual() {
	adjacent = new int*[numNode()];
	for(int i = 0; i < numNode(); ++i) adjacent[i] = new int[degree()];
	degrees = new int[numNode()];

	diameter = -1;
	aspl = -1;
}

Individual::Individual(const Individual& obj) : Individual(){
	for (int n = 0; n < numNode(); ++n) {
		for (int d = 0; d < degree(); ++d) {
			this->adjacent[n][d] = obj.adjacent[n][d];
		}
	}

	for (int n = 0; n < numNode(); ++n) {
		this->degrees[n] = obj.degrees[n];
	}

	this->diameter = obj.diameter;
	this->aspl = obj.aspl;
}

Individual::Individual(int const *const *const edges) : Individual() {
	for (int n = 0; n < numNode(); ++n) {
		for(int d = 0; d < degree(); ++d) {
			this->adjacent[n][d] = edges[n][d];
		}
	}

	for (int n = 0; n < numNode(); ++n) {
		this->degrees[n] = 0;
		for(int d = 0; d < degree(); ++d) {
			if(this->adjacent[n][d] == -1) break;
			this->degrees[n]++;
		}
	}
}

Individual::~Individual() {
	if (adjacent != nullptr) {
		for(int i = 0; i < numNode(); ++i) delete[] adjacent[i];
		delete[] adjacent;
		adjacent = nullptr;
	}

	if(degrees != nullptr) {
		delete[] degrees;
		degrees = nullptr;
	}
}

Individual& Individual::operator =(const Individual& operand) {
	this->diameter = operand.diameter;
	this->aspl = operand.aspl;

	for (int n = 0; n < numNode(); ++n) {
		for (int d = 0; d < degree(); ++d) {
			this->adjacent[n][d] = operand.adjacent[n][d];
		}
	}

	for (int n = 0; n < numNode(); ++n) {
		this->degrees[n] = operand.degrees[n];
	}

	return *this;
}

bool Individual::operator==(const Individual& operand)const {
	bool diam = (this->diameter == operand.diameter);
	bool aspl = (this->aspl == operand.aspl);
	return (diam && aspl);
}

bool Individual::operator<(const Individual& operand)const {
	if (this->diameter < operand.diameter) return true;

	if(this->diameter == operand.diameter) {
		if (this->aspl < operand.aspl) return true;
	}

	return false;
}

bool Individual::operator>(const Individual& operand)const {
	if (this->diameter > operand.diameter) return true;

	if (this->diameter == operand.diameter) {
		if (this->aspl > operand.aspl) return true;
	}

	return false;
}

bool Individual::operator<=(const Individual& operand)const {
	return !(*this > operand);
}

bool Individual::operator>=(const Individual& operand)const {
	return !(*this < operand);
}

void Individual::clear() {
	for (int i = 0; i < numNode(); ++i) {
		for (int d = 0; d < degree(); ++d) {
			this->adjacent[i][d] = -1;
		}
	}

	for(int i = 0; i < numNode(); ++i) {
		this->degrees[i] = 0;
	}
}

void Individual::addEdge(int nodeA, int nodeB) {
	if(nodeA == nodeB) return;
	if(this->haveEdge(nodeA, nodeB)) return;
	if(this->degrees[nodeA] == degree()) return;
	if(this->degrees[nodeB] == degree()) return;

	adjacent[nodeA][degrees[nodeA]] = nodeB;
	degrees[nodeA]++;
	adjacent[nodeB][degrees[nodeB]] = nodeA;
	degrees[nodeB]++;
}

bool Individual::haveEdge(int nodeA, int nodeB) const {
	for (int d = 0; d < degree(); ++d) {
		if (adjacent[nodeA][d] == nodeB) return true;
	}
	return false;
}

bool Individual::sameGraph(const Individual& indiv) const {
	for(int n = 0; n < numNode(); ++n) {
		int degA = this->degrees[n];
		int degB = indiv.degrees[n];
		for(int dA = 0; dA < degA; ++dA) {
			bool match = false;
			for(int dB = 0; dB < degB; ++dB) {
				if(this->adjacent[n][dA] == indiv.adjacent[n][dB]) {
					match = true;
					break;
				}
			}
			if(!match) return false;
		}
	}

	return true;
}

void Individual::showNodes() const {
	for (int n = 0; n < numNode(); ++n) {
		cout << n << ": ";
		for (int d = 0; d < degree(); ++d) {
			cout << this->adjacent[n][d] << " ";
		}
		cout << degrees[n] << endl;
	}
	cout << endl;
}
