#pragma once

#include <string>
#include "individual.hpp"
#include "../other/collection.hpp"
using std::string;
class ICrosser;

class Group {
public:
	Group(int groupSize, int numCreatedChild);
	~Group();

	bool doLocalSearch = false;
	ICrosser* crossover;

	void createRandomIndivs();
	void changeGeneration();
	Individual getBestIndiv();

	inline Individual bestIndividual() const { return _indivs[_bestIndex]; }
	inline int bestDiameter() const { return _indivs[_bestIndex].diameter; }
	inline double bestASPL() const { return  _indivs[_bestIndex].aspl; }
	inline int worstDiameter() const { return _indivs[_worstIndex].diameter; }
	inline double worstASPL() const { return _indivs[_worstIndex].aspl; }
	inline double averageASPL() const { return _averageASPL; }

private:
	const int _GROUP_SIZE;
	const int _NUM_CREATED_CHILD;
	Collection<Individual> _indivs;
	int _generation;
	int _bestIndex;
	int _worstIndex;
	double _averageASPL;
	double _averageDiameter;

	void tallyFitness();
	void addIndiv(Collection<Individual>& survivors, int* childNum);

private: /* prohibit */
	Group() = delete;
	Group(const Group& obj) = delete;
	Group(const Group&& obj) = delete;
	Group& operator=(const Group& obj) = delete;
	Group& operator=(const Group&& obj) = delete;
};
