#pragma once

template<class T> class Collection;
class Individual;
class RandomSelectWithoutReplace;

class MGG {
public:
    void copySelect(int* index1, int* index2, int numParent);
    void surviveSelect(const Collection<Individual>& parents, const Collection<Individual>& childs, Individual& survivorA, Individual& survivorB);
};
