#pragma once

#include <string>
#include "../ga/grid_graph_base.hpp"
using std::string;
class Individual;

class EdgeFileReader: GridGraph {
public:
    ~EdgeFileReader();
    static Individual GenerateIndividual(const string& filename);

private:
    static int GetEdgeNum(const string& filename);
    static void ReadEdges(const string& filename, int** edges, int edgeNum);
    static int getColumnNum(int** edges, int edgeNum);
    static int getRowNum(int** edges, int edgeNum);
    static int getDegree(int** edges, int edgeNum, int columnNum, int rowNum);
    static int getMaxLength(int** edges, int edgeNum, int columnNum);
    static void setGraph(int**edges, int edgeNum, Individual& indiv);

    EdgeFileReader() = delete;
};
