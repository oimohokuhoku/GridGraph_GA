#include<cstdlib>
#include"grid_graph_base.hpp"

/* public */
void GridGraph::setParameter(int columnNum, int rowNum, int degree, int length) {
    GridGraph::_COLUMN_NUM = columnNum;
    GridGraph::_ROW_NUM = rowNum;
    GridGraph::_NODE_NUM = columnNum * rowNum;
    GridGraph::_DEGREE = degree;
    GridGraph::_LENGTH = length;
	GridGraph::_PAIR_NUM = (_NODE_NUM * (_NODE_NUM - 1)) / 2;

	int perimeterNum = (_COLUMN_NUM * 2) + ((_ROW_NUM - 2) * 2);
	GridGraph::_PERIMETER = Collection<int>(perimeterNum);

	int n = 0;
	for (int i = 0; i < _COLUMN_NUM; ++i) { 
		_PERIMETER[n] = i; 
		n++;
	}
	for (int i = 1; i < _ROW_NUM; ++i) {
		_PERIMETER[n] = ((_COLUMN_NUM * i) + (_COLUMN_NUM - 1));
		n++;
	}
	for (int i = _COLUMN_NUM - 2; i >= 0; --i) {
		_PERIMETER[n] = (_COLUMN_NUM * (_ROW_NUM - 1)) + i;
		n++;
	}
	for (int i = _ROW_NUM - 2; i >= 1; --i) {
		_PERIMETER[n] = _COLUMN_NUM * i;
		n++;
	}
}

/* protected */
int GridGraph::fromAxis(int column, int row) {
	return (row * _COLUMN_NUM) + column;
}

int GridGraph::getColumn(int node) {
	return node % _COLUMN_NUM;
}

int GridGraph::getRow(int node) {
	return node / _COLUMN_NUM;
}

int GridGraph::getLength(int nodeA, int nodeB) {
	int columnA = getColumn(nodeA);
	int rowA = getRow(nodeA);
	int columnB = getColumn(nodeB);
	int rowB = getRow(nodeB);

	int dColumn = columnA - columnB;
	int dRow = rowA - rowB;

	if (dColumn < 0) dColumn *= -1;
	if (dRow < 0) dRow *= -1;

	return dColumn + dRow;
}

bool GridGraph::isPerimeter(int node) {
	for(int i = 0; i < _PERIMETER.size(); ++i) {
		if(_PERIMETER[i] == node) return true;
	}
	return false;
}

/* private */
int GridGraph::_COLUMN_NUM;
int GridGraph::_ROW_NUM;
int GridGraph::_NODE_NUM;
int GridGraph::_DEGREE;
int GridGraph::_LENGTH;
int GridGraph::_PAIR_NUM;
Collection<int> GridGraph::_PERIMETER;
