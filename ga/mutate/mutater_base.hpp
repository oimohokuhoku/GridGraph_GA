#pragma once

class Individual;

class Mutater_Base {
public:
    virtual ~Mutater_Base() = default;
    virtual void operator() (Individual& indiv) = 0;
    static void SetParameter(double indivProbability, double geneProbability);

protected:
    static double _INDIV_MUTATE_PROBABILITY;
    static double _GENE_MUTATE_PROBABILITY;
};
