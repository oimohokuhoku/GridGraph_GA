#pragma once

#include <string>
#include "../ga/grid_graph_base.hpp"
using std::string;
class Individual;

class EdgeFileWriter: public GridGraph {
public:
    static void save(const Individual& indiv, const string& filepath);

private: /* prohibit */
    EdgeFileWriter() = delete;
};