#pragma once

#include<random>
#include<time.h>

template<class T>
class ArrayUtil {
public:
    static T* newArray(int size) {
        return new T[size];
    }

    static T* newArray(int size, const T& value) {
        T* array = newArray(size);
        for(int i = 0; i < size; ++i) array[i] = value;
        return array;
    }

    static void fillSequence(int* array, int length) {
        for(int i = 0; i < length; ++i) {
            array[i] = i;
        }
    }

    static void shaffle(T* array, int length) {
        std::random_device seed;
        std::mt19937 rand(seed());

        for(int target = length - 1; target >= 0; --target) {
            int i = rand() % (target + 1);

            int temp = array[i];
            array[i] = array[target];
            array[target] = temp;
        }
    }
};