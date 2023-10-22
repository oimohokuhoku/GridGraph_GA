#pragma once

#include<random>
#include<time.h>

class ArrayUtil {
public:
    template<class T> static T* newArray(int size) {
        return new T[size];
    }

    template<class T> static T* newArray(int size, const T& value) {
        T* array = newArray<T>(size);
        for(int i = 0; i < size; ++i) array[i] = value;
        return array;
    }

    template<class T> static T** newArray2D(int size1D, int size2D) {
        T** array = new T*[size1D];
        for(int i = 0; i < size1D; ++i) array[i] = new T[size2D];
        return array;
    }

    template<class T> static T** newArray2D(int size1D, int size2D, T value) {
        T** array = newArray2D<T>(size1D, size2D);
        for(int i = 0; i < size1D; ++i) {
            for(int j = 0; j < size2D; ++j) {
                array[i][j] = value;
            }
        }
        return array;
    }

    template<class T> static void deleteArray2D(T** array, int size1D) {
        for(int i = 0; i < size1D; ++i) delete[] array;
        delete[] array;
    }

    template<class T> static void shaffle(T* array, int length) {
        std::random_device seed;
        std::mt19937 rand(seed());

        for(int target = length - 1; target >= 0; --target) {
            int i = rand() % (target + 1);

            int temp = array[i];
            array[i] = array[target];
            array[target] = temp;
        }
    }

    template<class T> static void reverse(T* array, int length) {
        T* temp = new T[length];
        for (int i = 0; i < length; ++i) {
            temp[i] = array[length - 1 - i];
        }
        for(int i = 0; i < length; ++i) {
            array[i] = temp[i];
        }
        delete[] temp;
    }

    static void fillSequence(int* array, int length) {
        for(int i = 0; i < length; ++i) {
            array[i] = i;
        }
    }

    ArrayUtil() = delete;
};
