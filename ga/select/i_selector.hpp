#pragma once

#include <memory>
#include "../../other/collection.hpp"
using std::unique_ptr;
class Individual;

class ICopySelector {
public:
    ICopySelector() = default;
    virtual ~ICopySelector() = default;

    /// @brief select parents
    virtual Collection<Individual> select() = 0;
};

class ISurviveSelector {
public:
    ISurviveSelector() = default;
    virtual ~ISurviveSelector() = default;

    /// @brief select survivor
    virtual Collection<Individual> select(Collection<Individual>& parents, Collection<Individual>& child) = 0;
};

class ISelectorFactory {
public:
    ISelectorFactory() = default;
    virtual ~ISelectorFactory() = default;
    /// @brief Generate ICopySelector instance
    /// @param parents Parents intended for selection
    virtual unique_ptr<ICopySelector> generateCopySelector(Collection<Individual>& parents) = 0; 

    /// @brief Generate ISurvivorSelector instance
    /// @param parents Parents intended for selection
    /// @param child Childs intended for selection
    virtual unique_ptr<ISurviveSelector> generateSurviveSelector() = 0;

private: /* prohibit */
    ISelectorFactory(ISelectorFactory& obj) = delete;
    ISelectorFactory(ISelectorFactory&& obj) = delete;
    ISelectorFactory& operator=(ISelectorFactory& obj) = delete;
    ISelectorFactory& operator=(ISelectorFactory&& obj) = delete;
};
