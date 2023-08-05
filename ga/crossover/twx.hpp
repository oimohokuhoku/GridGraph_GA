#pragma once

#include "i_crosser.hpp"
using namespace std;
class Individual;

class TWX: public ICrosser {
public:
	TWX() = default;
	~TWX() = default;
	void cross(const Individual& parentA, const Individual& parentB, Individual& child) override;

private:
	int _degreeCount;

	int selectNodeFromPerimeter(const Individual& child);
	int selectNodeFromAll(const Individual& child);
	void getMinTour(const Individual& parent, const int startNode, const int endNode, int* tourOut);
	void getTour(const Individual& parent, const int startNode, const int endNode, int* tourOut);
	void addTour(Individual& indiv, int tour[]);
};