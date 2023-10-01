#pragma once

template<class T>
class ArrayUtil {
public:
    static T* newArray(int size) {
        return new T[size];
    }

    static T* newArray(int size, T value) {
        T* array = newArray(size);
        for(int i = 0; i < size; ++i) array[i] = value;
        return array;
    }
};