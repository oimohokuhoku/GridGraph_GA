#pragma once

#include <memory>
#include "../other/common.hpp"
#include "grid_graph_base.hpp"
#include "mutate/two_opt.hpp"
#include "crossover/i_crosser.hpp"

using std::unique_ptr;
class IEvaluater;
class IGeneEditor;

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
	bool operator ==(const Individual& operand)const;
	/// @brief smaller graph is better
	bool operator <(const Individual& operand)const;
	/// @brief smaller graph is better
	bool operator >(const Individual& operand)const;
	/// @brief smaller graph is better
	bool operator <=(const Individual& operand)const;
	/// @brief smaller graph is better
	bool operator >=(const Individual& operand)const;

	/// @brief delete all edges
	void clear();
	/// @brief add edge. illegal edge(duplicated, over degree) is ignored
	/// @param nodeA end of edge
	/// @param nodeB end of edge 
	void addEdge(int nodeA, int nodeB);
	/// @brief whether this has edge
	/// @param nodeA end of edge
	/// @param nodeB end of edge
	/// @return If this already has edge, return true
	bool haveEdge(int nodeA, int nodeB) const;
	/// @brief check all edge matching. graph which same as rotated, regard as not same.
	/// @param indiv 
	/// @return 
	bool sameGraph(const Individual& indiv) const;
	void showNodes() const;

	void editGene(IGeneEditor& editor);
};
