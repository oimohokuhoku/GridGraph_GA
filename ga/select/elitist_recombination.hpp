#pragma once

template<class T> class Collection;
class Individual;

class Elitist_Recombination{
public:
    Elitist_Recombination(int numParent);
    ~Elitist_Recombination();
    void copySelect(int* index1, int* index2);
    void surviveSelect(const Collection<Individual>& parents, const Collection<Individual>& childs, Individual& survivorA, Individual& survivorB);

private:
    const int _NUM_PAERNT;
    int* _sequence;
    int _index;
};