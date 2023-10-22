#include "survive_selector.hpp"
#include "ga/individual.hpp"
#include "other/random.hpp"
#include "other/array_util.hpp"
#include<iostream>

Individual ElitistSelect::select (Collection<Individual>& indivs, bool* selected) {
    Individual& elitist = indivs[0];
    int eliteIndex = 0;

    for(int i = 1; i < indivs.size(); ++i) {
        if(selected[i]) continue;
        if(indivs[i].betterThan(elitist)) {
            elitist = indivs[i];  
            eliteIndex = i;
        }
    }

    selected[eliteIndex] = true;
    return elitist;
}

Individual RouletteSelect::select (Collection<Individual>& indivs, bool* selected) {
    double total = 0.0;
    for (int i = 0; i < indivs.size(); ++i) {
        if(selected[i]) continue;
        total += 1.0 / indivs[i].aspl;
    }

    Random rand;
    double randomValue = rand.randomDouble(0, total);

    double accumulatedInverseFitness = 0.0;
    for (int i = 0; i < indivs.size(); ++i) {
        if(selected[i]) continue;

        accumulatedInverseFitness += 1.0 / indivs[i].aspl;
        if (accumulatedInverseFitness >= randomValue) {
            return indivs[i];
        }
    }

    return 0;
}

Individual RankingSelect::select (Collection<Individual>& indivs, bool* selected) {
    int NUM_INDIV = indivs.size();
    int* ranking = new int[NUM_INDIV];
    ArrayUtil::fillSequence(ranking, NUM_INDIV);

    //sort昇順
    for (int i = 0; i < NUM_INDIV - 1; i++) {
        for (int j = 0; j < NUM_INDIV - i - 1; j++) {
            if (indivs[j].betterThan(indivs[j + 1])) {
                Individual temp = indivs[j];
                indivs[j] = indivs[j + 1];
                indivs[j + 1] = temp;
            }
        }
    }
    
    int totalRank = (NUM_INDIV - 2) * (NUM_INDIV - 1) / 2.0; //except elitist

    // ルーレット選択を行う
    Random random;
    double randomValue = random.randomInt(0, totalRank);

    double cumulativeProbability = 0.0;
    for (int i = 0; i < NUM_INDIV; i++) {
        cumulativeProbability += ranking[i];
        if (randomValue <= cumulativeProbability) {
            return indivs[ranking[i]];
        }
    }

    // 通常、ここには到達しないはず
    return 0;
}
