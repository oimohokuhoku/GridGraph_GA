#pragma once

#include <memory>
#include "mutater_base.hpp"
#include "../grid_graph_base.hpp"
using std::unique_ptr;

class Rotate: private Mutater_Base, GridGraph {
public:
    Rotate() = default;
    ~Rotate() = default;
    void operator() (Individual& indiv) override;

private:
    static unique_ptr<int[]> _rotatedNode90;
    static unique_ptr<int[]> _rotatedNode180;
    static unique_ptr<int[]> _rotatedNode270;
    static unique_ptr<int[]> _turnedNodeVertical;
    static unique_ptr<int[]> _turnedNodeHorizontal;

    void convert(Individual& indiv, unique_ptr<int[]>& convertedNode);
    void setRotatedNode90();
    void setRotatedNode180();
    void setRotatedNode270();
    void setTurnedNodeVertical();
    void setTurnedNodeHorizontal();

private: /* prohibit */
    Rotate(Rotate& obj) = delete;
    Rotate(Rotate&& obj) = delete;
    Rotate operator=(Rotate& obj) = delete;
    Rotate operator=(Rotate&& obj) = delete;
};