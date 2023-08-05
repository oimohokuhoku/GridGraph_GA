#include<iostream>
#include<cstdlib>
#include"twx.hpp"
#include"../individual.hpp"
#include"../../other/common.hpp"
#include"../../other/int_queue.hpp"
#include"../evaluate/adj_aspl.hpp"
using namespace std;

/* public */
void TWX::cross(const Individual& parentA, const Individual& parentB, Individual& child) {
	int startNode, endNode;
	int* tour = new int[nodeNum()];
	_degreeCount = 0;

	child.clear();

	for (int i = 0; i < 0; ++i) {
		startNode = selectNodeFromPerimeter(child);
		endNode = selectNodeFromPerimeter(child);

		if(startNode == -1) continue;
		if(endNode == -1) continue;
		if (startNode == endNode) continue;

		if (i % 2 == 0) getMinTour(parentA, startNode, endNode, tour);
		else 			getMinTour(parentB, startNode, endNode, tour);

		if (tour[2] == -1) continue;

		addTour(child, tour);
	}

	for (int i = 0; i < 400; ++i) {
		startNode = selectNodeFromAll(child);
		endNode = selectNodeFromAll(child);

		if(startNode == -1) continue;
		if(endNode == -1) continue;
		if (startNode == endNode) continue;

		if (i % 2 == 0) getMinTour(parentA, startNode, endNode, tour);
		else 			getMinTour(parentB, startNode, endNode, tour);

		if (tour[2] == -1) continue;

		addTour(child, tour);
	}

	modifyGraph(child);
	delete[] tour;
}

/* private */
int TWX::selectNodeFromAll(const Individual& child){
	int n = randInt(0, nodeNum());
	int rand = randInt(1, 2 + nodeNum() - (int)(_degreeCount / degree()));

	for (int i = 0; i < nodeNum(); ++i) {
		n = (n + 1) % nodeNum();
		if (child.degrees[n] < degree()) {
			rand--;
			if(rand <= 0) return n;
		}
	}

	return -1;
}

int TWX::selectNodeFromPerimeter(const Individual& child) {
	const int NUM_PERI = perimeters().size();
	int n;
	int rand = randInt(0, perimeters().size());
	int count = 0;
	while (count < NUM_PERI) {
		n = perimeters()[rand];
		if (child.degrees[n] < degree()) return n;
		rand = (rand + 1) % NUM_PERI;
		count++;
	}

	return -1;
}

void TWX::getMinTour(const Individual& parent, const int startNode, const int endNode, int* tourOut) {
	IntQueue queue(nodeNum());
	bool visited[nodeNum()];
	int edges[nodeNum()][2];
	int n1, n2, edgeIndex = 0;
	bool reach = false;

	for (int i = 0; i < nodeNum(); ++i) visited[i] = false;

	/* bfs */
	queue.enqueue(startNode);
	visited[startNode] = true;

	while (queue.count() > 0) {
		n1 = queue.dequeue();

		for (int d = 0; d < degree(); ++d) {
			n2 = parent.adjacent[n1][d];

			if (n2 == -1) continue;
			if (visited[n2]) continue;

			if(getLength(n1, n2) > 1) {
				double rand = randDouble();
				if(rand < 0.01) continue;
			}

			edges[edgeIndex][0] = n1;
			edges[edgeIndex][1] = n2;
			edgeIndex++;

			if (n2 == endNode) {
				reach = true;
				break;
			}

			visited[n2] = true;
			queue.enqueue(n2);
		}
		if (reach) break;
	}

	/* convert to array */
	tourOut[0] = edges[edgeIndex - 1][1];
	int node = edges[edgeIndex - 1][1];
	int tourIndex = 1;
	for (int i = edgeIndex - 1; i >= 0; i--) {
		if (edges[i][1] == node) {
			node = edges[i][0];
			tourOut[tourIndex] = node;
			tourIndex++;
		}
	}

	tourOut[tourIndex] = -1;
}

void TWX::getTour(const Individual& parent, const int startNode, const int endNode, int* tourOut) {
	int index = 0;
	int node = startNode;
	bool* finded = new bool[nodeNum()];

	for(int i = 0; i < nodeNum(); ++i) finded[i] = false;

	tourOut[index] = startNode;
	index++;

	while(true) {
		int minDeg = -1;
		int minDist = INT_MAX;
		int d = randInt(0, degree());
		for (int di = 0; di < degree(); ++di) {
			d = (d + 1) % degree();
			int n = parent.adjacent[node][d];

			if(finded[n]) continue;

			if(n == endNode) {
				tourOut[index] = endNode;
				tourOut[index + 1] = -1;
				delete[] finded;
				return;
			}

			int dist = getLength(n, endNode);
			if(dist < minDist) {
				minDeg = d;
				minDist = dist;
			}
		}

		if(minDeg) {
			tourOut[index] = -1;
			delete[] finded;
			return;
		}

		node = parent.adjacent[node][minDeg];
		finded[node] = true;
		tourOut[index] = node;
		index++;
	} 
}

void TWX::addTour(Individual& indiv, int tour[]) {
	for(int i = 0; tour[i + 1] != -1; ++i) {
		int n1 = tour[i];
		int n2 = tour[i + 1];

		if(indiv.haveEdge(n1, n2)) continue;
		if (indiv.degrees[n1] == degree()) return;
		if (indiv.degrees[n2] == degree()) return;

		indiv.adjacent[n1][indiv.degrees[n1]] = n2;
		indiv.degrees[n1]++;
		indiv.adjacent[n2][indiv.degrees[n2]] = n1;
		indiv.degrees[n2]++;

		_degreeCount += 2;
	}
}
