#include "copy_selector.hpp"
#include "ga/individual.hpp"
#include "other/array_util.hpp"
#include "other/random.hpp"
#include <iostream>

void RandomSelectWithoutReplace::select(int* index1, int* index2, int numParent){
    Random rand;

    *index1 = rand.randomInt(0, numParent);

    while(true) {
        *index2 = rand.randomInt(0, numParent);
        if(*index1 != *index2) break;
    }
}