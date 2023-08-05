#pragma once

#include <memory>
#include"../../other/collection.hpp"
#include "i_selector.hpp"
using std::unique_ptr;
class Individual;

class MGG: public ISelectorFactory {
public:
    class CopySelector: public ICopySelector{
    public:
        CopySelector(Collection<Individual>& parents);
        ~CopySelector();
        Collection<Individual> select() override;
    private:
        Collection<Individual> _parents;
        int* _selectSeq;
        int _selectIndex;
        Individual& selectRandom_withoutReplace();
        void setRandomSequence(int* seq);
        CopySelector() = delete;
    };

    class SurviveSelector: public ISurviveSelector {
    public:
        SurviveSelector() = default;
        ~SurviveSelector() = default;
        Collection<Individual> select(Collection<Individual>& parents, Collection<Individual>& child) override;
    private:
        Collection<Individual> _parents;
        Collection<Individual> _childs;
        unique_ptr<bool[]> _selectedParent;
        unique_ptr<bool[]> _selectedChild;
        Individual selectElite();
        Individual& selectRoulette();
    };

    MGG() =default;
    ~MGG() = default;
    unique_ptr<ICopySelector> generateCopySelector(Collection<Individual>& parents) override;
    unique_ptr<ISurviveSelector> generateSurviveSelector() override;

private: /* prohibit */
    MGG(MGG& obj) = delete;
    MGG(MGG&& obj) = delete;
    MGG& operator=(MGG& obj) = delete;
    MGG& operator=(MGG&& obj) = delete;
};
