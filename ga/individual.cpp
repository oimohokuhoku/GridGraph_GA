#include<iostream>
#include <memory>
#include<cstdlib>
#include<climits>
#include "individual.hpp"
#include "i_gene_editor.hpp"
#include "../other/int_queue.hpp"
#include "../other/common.hpp"
#include "mutate/two_opt.hpp"
#include "evaluate/ievaluater.hpp"
using std::unique_ptr;

Individual::Individual() {
	adjacent = newArray_2D<int>(nodeNum(), degree());
	degrees = new int[nodeNum()];

	diameter = -1;
	aspl = -1;
}

Individual::Individual(const Individual& obj){
	this->diameter = obj.diameter;
	this->aspl = obj.aspl;

	this->adjacent = newArray_2D<int>(nodeNum(), degree());
	this->degrees = new int[nodeNum()];

	for (int n = 0; n < nodeNum(); ++n) {
		for (int d = 0; d < degree(); ++d) {
			this->adjacent[n][d] = obj.adjacent[n][d];
		}
	}

	for (int n = 0; n < nodeNum(); ++n) {
		this->degrees[n] = obj.degrees[n];
	}
}

Individual::Individual(int const *const *const edges) {
	adjacent = newArray_2D<int>(nodeNum(), degree());
	degrees = new int[nodeNum()];

	for (int n = 0; n < nodeNum(); ++n) {
		for(int d = 0; d < degree(); ++d) {
			this->adjacent[n][d] = edges[n][d];
		}
	}

	for (int n = 0; n < nodeNum(); ++n) {
		this->degrees[n] = 0;
		for(int d = 0; d < degree(); ++d) {
			if(this->adjacent[n][d] == -1) break;
			this->degrees[n]++;
		}
	}
}

Individual::~Individual() {
	if (adjacent != nullptr) {
		deleteArray_2D(this->adjacent, nodeNum());
		this->adjacent = nullptr;
	}

	if(degrees != nullptr) {
		delete[] this->degrees;
		this->degrees = nullptr;
	}
}

Individual& Individual::operator =(const Individual& operand) {
	this->diameter = operand.diameter;
	this->aspl = operand.aspl;

	for (int n = 0; n < nodeNum(); ++n) {
		for (int d = 0; d < degree(); ++d) {
			this->adjacent[n][d] = operand.adjacent[n][d];
		}
	}

	for (int n = 0; n < nodeNum(); ++n) {
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
	for (int i = 0; i < nodeNum(); ++i) {
		for (int d = 0; d < degree(); ++d) {
			this->adjacent[i][d] = -1;
		}
	}

	for(int i = 0; i < nodeNum(); ++i) {
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
	for(int n = 0; n < nodeNum(); ++n) {
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
	for (int n = 0; n < nodeNum(); ++n) {
		cout << n << ": ";
		for (int d = 0; d < degree(); ++d) {
			cout << this->adjacent[n][d] << " ";
		}
		cout << degrees[n] << endl;
	}
	cout << endl;
}

void Individual::editGene(IGeneEditor& editor) {
	editor.edit(*this);
}

