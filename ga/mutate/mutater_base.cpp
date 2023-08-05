#include "mutater_base.hpp"

void Mutater_Base::SetParameter(double indivMutateProbability, double geneMutateProbability) {
    Mutater_Base::_INDIV_MUTATE_PROBABILITY = indivMutateProbability;
    Mutater_Base::_GENE_MUTATE_PROBABILITY = geneMutateProbability;
}

double Mutater_Base::_INDIV_MUTATE_PROBABILITY;
double Mutater_Base::_GENE_MUTATE_PROBABILITY;