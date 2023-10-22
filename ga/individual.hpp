#pragma once

#include "grid_graph_base.hpp"

class Individual: public GridGraph{
public:
	/// @brief int[numNode][degree] : adjacent[i][j] is j-th node adjacent to i-th node
	int** adjacent;
	/// @brief int[numNode] : degree[i] is degree of i-th node;
	int* degrees;
	int diameter;
	double aspl;

	Individual();
	Individual(const Individual& obj);
	Individual(int const *const *const edges);
	~Individual();
	
	Individual& operator =(const Individual& operand);
	bool equalFitness(const Individual& operand)const;
	bool worseThan(const Individual& operand)const;
	bool betterThan(const Individual& operand)const;

	void clear();
	void addEdge(int nodeA, int nodeB);
	void removeEdge(int nodeA, int nodeB);
	bool haveEdge(int nodeA, int nodeB) const;
	bool sameGraph(const Individual& indiv) const;
	int getDegreeIndex(int nodeA, int nodeB) const;

	void showNodes() const;
};
