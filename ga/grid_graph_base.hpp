#pragma once

#include "../other/collection.hpp"

class GridGraph {
public:
	static void setParameter(int columnNum, int rowNum, int degree, int length);
    
protected:
    static inline int columnNum() { return GridGraph::_COLUMN_NUM; }
    static inline int rowNum() { return GridGraph::_ROW_NUM; }
    static inline int nodeNum() { return GridGraph::_NODE_NUM; }
    static inline int degree() { return GridGraph::_DEGREE; }
    static inline int length() { return GridGraph::_LENGTH; }
    static inline int pairNum() { return GridGraph::_PAIR_NUM; }
    static Collection<int>& perimeters() { return GridGraph::_PERIMETER; }

	static int fromAxis(int column, int row);
	static int getColumn(int node);
	static int getRow(int node);
	static int getLength(int nodeA, int nodeB);
    static bool isPerimeter(int node);
private:
	static int _COLUMN_NUM;
	static int _ROW_NUM;
    static int _NODE_NUM;
    static int _DEGREE;
    static int _LENGTH;
    static int _PAIR_NUM;
    static Collection<int> _PERIMETER;

    
};
