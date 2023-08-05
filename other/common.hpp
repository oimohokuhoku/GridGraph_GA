#pragma once

#include <cstdio>
#include <climits>

using namespace std;

const int INF = INT_MAX;

void initRand();
int randInt(int min, int max);
double randDouble();

template<class T>
inline T** newArray_2D(int size1D, int size2D) {
    T** array = new T*[size1D];
    for(int i = 0; i < size1D; ++i) {
        array[i] = new T[size2D];
    }
    return array;
}

template<class T>
inline T** newArray_2D(int size1D, int size2D, int initValue) {
    T** array = new T*[size1D];
    for(int i = 0; i < size1D; ++i) array[i] = new T[size2D];

    for(int i = 0; i < size1D; ++i) {
        for(int j = 0; j < size2D; ++j) {
            array[i][j] = initValue;
        }
    }
 
    return array;
}

template<class T>
inline void deleteArray_2D(T** array, int size1D) {
    for(int i = 0; i < size1D; ++i) delete array[i];
    delete array;
}