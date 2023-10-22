#pragma once

template<class T> class Collection;
class Individual;

class RandomSelectWithoutReplace {
public:
    static void select(int* index1, int* index2, int numParent);
};