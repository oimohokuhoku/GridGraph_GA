#pragma once

#include "../grid_graph_base.hpp"
class Individual;

class ICrosser: public GridGraph{
public:
    virtual ~ICrosser() = default;
    virtual void operator() (const Individual& parentA, const Individual& parentB, Individual& child) = 0;

protected:
    void virtual modifyGraph(Individual& indiv);
};

class DMSX : public ICrosser {
public:
    DMSX(int unitWidth);
    ~DMSX();
    void operator() (const Individual& parentA, const Individual& parentB, Individual& child) override;
private:
    const int _UNIT_WIDTH;
    const int _NUM_UNIT_COLUMN;
    const int _NUM_UNIT_ROW;
    bool** exchandegEdgeInStartParent;
    bool** exchandegEdgeInEndParent;
    void generateNeighbor(const Individual& startParent, const Individual& endParent, int unitIndex, Individual& baseIndiv);
    int findShortestABCycle(const Individual& startParent, const Individual& endParent, int startEdgeNode1, int startEdgeNode2, int* abCycle);
    int toPath(int* preNodes, int startNode, int endNode, int* path);
    void embedABCycle(int* abCycle, int cycleLength, Individual& baseIndiv);
    void recordEmbedded(int* abCycle, int cycleLength, const Individual& startParent, const Individual& endParent);
};

class GraftingCrossover : public ICrosser {
public:
    GraftingCrossover(int maxNumLS);
    ~GraftingCrossover() = default;
    void operator() (const Individual& parentA, const Individual& parentB, Individual& child) override;
private:
    const int _MAX_NUM_LS;
    void decideNodeBelonging(int* array);
    int* decideDivideLine(int width, int numDevideLine);
    void obtainEdgeWithinRange(const Individual& parentA, const Individual& parentB, int* nodeBelonging, Individual& child);
    void obtainDuplicateEdge(const Individual& parentA, const Individual& parentB, Individual& child);
    void obtainOtherEdge(const Individual& parentA, const Individual& parentB, Individual& child);
    void localSearch(int const *const nodeBelonging, Individual& child);
    void collectBorderNodes(int const *const nodeBelonging, int* result);
    void collectNearNodes(int node, int* result, Individual& test);
    void fill(int top, int bottom, int left, int right, int value, int* array);
};

class TourWeavingCrossover: public ICrosser {
public:
	TourWeavingCrossover(int numAllSelect, int numPerimeterSelect);
	~TourWeavingCrossover() = default;
	void operator() (const Individual& parentA, const Individual& parentB, Individual& child) override;

private:
	int _degreeCount;
	int _numAllSelect;
	int _numPeriSelect;

	int selectNodeFromPerimeter(const Individual& child);
	int selectNodeFromAll(const Individual& child);
	void getMinTour(const Individual& parent, const int startNode, const int endNode, int* tourOut);
	void getTour(const Individual& parent, const int startNode, const int endNode, int* tourOut);
	void addTour(Individual& indiv, int tour[]);
};

class SimpleEdgeSelectCrossover : public ICrosser {
public:
    SimpleEdgeSelectCrossover() =default;
    ~SimpleEdgeSelectCrossover() = default;
    void operator() (const Individual& parentA, const Individual& parentB, Individual& child) override;
private:
    void GetRandomEdge(const Individual& parent, int* nodeA, int* nodeB);
};
