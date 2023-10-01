#include<iostream>
#include<cstdio>
#include<stdio.h>
#include<fstream>
#include "../ga/individual.hpp"
#include "../ga/grid_graph_base.hpp"
#include "edges_file_reader.hpp"

using namespace std;

Individual EdgeFileReader::GenerateIndividual(const string& filename) {
    int edgeNum = GetEdgeNum(filename);

    int** edges = new int*[edgeNum];
    for(int i = 0; i < edgeNum; ++i) edges[i] = new int[4];

    ReadEdges(filename, edges, edgeNum);

    int columnNum = getColumnNum(edges, edgeNum);
    int rowNum = getRowNum(edges, edgeNum);
    int degree = getDegree(edges, edgeNum, columnNum, rowNum);
    int length = getMaxLength(edges, edgeNum, columnNum);

    GridGraph::setParameter(columnNum, rowNum, degree, length);
    Individual indiv;
    setGraph(edges, edgeNum, indiv);

    for(int i = 0; i < edgeNum; ++i) delete[] edges[i];
    delete[] edges;

    return indiv;
}

int EdgeFileReader::GetEdgeNum(const string& filename) {
    ifstream ifs(filename);
    if(ifs.fail()) {
        cerr << "file \"" << filename << "\" can't be opened" << endl;
        exit(-1);
    }

    int lineNum = 0;
    string dummy;

    while(getline(ifs, dummy)) {
        lineNum++;
    }

    return lineNum;
}

void EdgeFileReader::ReadEdges(const string& filename, int** edges, int edgeNum) {
    FILE* fp;
    fp = fopen(filename.c_str(), "r");

    if(fp == NULL) {
        cerr << "file \"" << filename << "\" can't be opened" << endl;
        exit(-1);
    }

    int columnA, rowA, columnB, rowB;
    for(int i = 0; i < edgeNum; ++i) {
        fscanf(fp, "%d,%d %d,%d", &columnA, &rowA, &columnB, &rowB);
        edges[i][0] = columnA;
        edges[i][1] = rowA;
        edges[i][2] = columnB;
        edges[i][3] = rowB;
    }

    fclose(fp);
}

int EdgeFileReader::getColumnNum(int** edges, int edgeNum) {
    int maxColumn = 0;

    for(int i = 0; i < edgeNum; ++i) {
        if(maxColumn < edges[i][0]) maxColumn = edges[i][0];
        if(maxColumn < edges[i][2]) maxColumn = edges[i][2];
    }

    return maxColumn + 1;
}

int EdgeFileReader::getRowNum(int** edges, int edgeNum) {
    int maxRow = 0;

    for(int i = 0; i < edgeNum; ++i) {
        if(maxRow < edges[i][1]) maxRow = edges[i][1];
        if(maxRow < edges[i][3]) maxRow = edges[i][3];
    }

    return maxRow + 1;
}

int EdgeFileReader::getDegree(int** edges, int edgeNum, int columnNum, int rowNum) {
    int** degree = new int*[columnNum];
    for(int i = 0; i < columnNum; ++i) {
        degree[i] = new int[rowNum];
    }

    for(int c = 0; c < columnNum; ++c) {
        for(int h = 0; h < rowNum; ++h) {
            degree[c][h] = 0;
        }
    }

    for(int i = 0; i < edgeNum; ++i) {
        int columnA = edges[i][0];
        int rowA = edges[i][1];
        int columnB = edges[i][2];
        int rowB = edges[i][3];

        degree[columnA][rowA]++;
        degree[columnB][rowB]++;
    }

    int maxDegree = 0;
    for(int c = 0; c < columnNum; ++c) {
        for(int h = 0; h < rowNum; ++h) {
            if(maxDegree < degree[c][h]) maxDegree = degree[c][h];
        }
    }

    for(int i = 0; i < columnNum; ++i) {
        delete[] degree[i];
    }
    delete[] degree;

    return maxDegree;
}

int EdgeFileReader::getMaxLength(int** edges, int edgeNum, int columnNum) {
    int maxLength = 0;

    for(int i = 0; i < edgeNum; ++i) {
        int dColumn = edges[i][0] - edges[i][2];
        int dRow = edges[i][1] - edges[i][3];

        if(dColumn < 0) dColumn *= -1;
        if(dRow < 0) dRow *= -1;

        int length = dColumn + dRow;

        if(maxLength < length) maxLength = length;
    }

    return maxLength;
}

void EdgeFileReader::setGraph(int**edges, int edgeNum, Individual& indiv) {
    int* degrees = new int[numNode()];
    for(int i = 0; i < numNode(); ++i) degrees[i] = 0;

    int** resultEdges = new int*[numNode()];
    for(int i = 0; i < numNode(); ++i) resultEdges[i] = new int[degree()];

    for(int n = 0; n < numNode(); ++n) {
        for(int d = 0; d < degree(); ++d) {
            resultEdges[n][d] = -1;
        }
    }

    for(int i = 0; i < edgeNum; ++i) {
        int nodeA = fromAxis(edges[i][0], edges[i][1]);
        int nodeB = fromAxis(edges[i][2], edges[i][3]);

        int degreeA = degrees[nodeA];
        resultEdges[nodeA][degreeA] = nodeB;
        degrees[nodeA]++;

        int degreeB = degrees[nodeB];
        resultEdges[nodeB][degreeB] = nodeA;
        degrees[nodeB]++;
    }

    indiv = Individual(resultEdges);

    for(int i = 0; i < numNode(); ++i) delete[] resultEdges[i];
    delete[] resultEdges;
    delete[] degrees;
}