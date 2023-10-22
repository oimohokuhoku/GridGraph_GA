#include"mgg.hpp"
#include"copy_selector.hpp"
#include"survive_selector.hpp"
#include"ga/individual.hpp"
#include"other/collection.hpp"
#include"other/random.hpp"
#include "other/array_util.hpp"

void MGG::copySelect(int* index1, int* index2, int numParent) {
    RandomSelectWithoutReplace::select(index1, index2, numParent);
}

void MGG::surviveSelect(const Collection<Individual>& parents, const Collection<Individual>& childs, Individual& survivorA, Individual& survivorB) {
    Collection<Individual> indivs = parents.concat(childs);
    bool* selected = ArrayUtil::newArray<bool>(indivs.size(), false);

    ElitistSelect selectA;
    RouletteSelect selectB;
    survivorA = selectA.select(indivs, selected);
    survivorB = selectB.select(indivs, selected);
}