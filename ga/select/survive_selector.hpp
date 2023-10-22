#pragma once

template<class T> class Collection;
class Individual;

class ISurvivorSelect {
public:
    ISurvivorSelect() = default;
    virtual ~ISurvivorSelect() = default;
    virtual Individual select (Collection<Individual>& indivs, bool* selected) = 0;
};

class ElitistSelect : public ISurvivorSelect{
public:
    ElitistSelect() = default;
    ~ElitistSelect() = default;
    Individual select (Collection<Individual>& indivs, bool* selected) override;
};

class RouletteSelect : public ISurvivorSelect{
public:
    RouletteSelect() = default;
    ~RouletteSelect() = default;
    Individual select (Collection<Individual>& indivs, bool* selected) override;
};

class RankingSelect : public ISurvivorSelect{
public:
    RankingSelect() = default;
    ~RankingSelect() = default;
    Individual select (Collection<Individual>& indivs, bool* selected) override;
};