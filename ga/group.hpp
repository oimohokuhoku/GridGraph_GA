#pragma once

#include <memory>
#include <string>
#include "individual.hpp"
#include "../other/collection.hpp"
using std::unique_ptr, std::string;

extern const int GROUP_SIZE;

class Group {
public:
	Group();

	bool doLocalSearch = false;

	void createRandomIndivs();
	void changeGeneration();
	void createBestEdgesFile(string filePath) const;
	Individual getBestIndiv();

	static void setParameter(int groupSize, int createdChildNum);
	inline int bestDiameter() const { return _indivs[_bestIndex].diameter; }
	inline double bestASPL() const { return  _indivs[_bestIndex].aspl; }
	inline int worstDiameter() const { return _indivs[_worstIndex].diameter; }
	inline double worstASPL() const { return _indivs[_worstIndex].aspl; }
	inline double averageASPL() const { return _averageASPL; }

private:
	Collection<Individual> _indivs;
	int _bestIndex;
	int _worstIndex;
	double _averageASPL;
	double _averageDiameter;

	static int _GROUP_SIZE;
	static int _CREATED_CHILD_NUM;

	void tallyFitness();
	void addIndiv(Collection<Individual>& survivors, int* childNum);

private: /* prohibit */
	Group(const Group& obj) = delete;
	Group(const Group&& obj) = delete;
	Group& operator=(const Group& obj) = delete;
	Group& operator=(const Group&& obj) = delete;
};
