#include "edges_file_writer.hpp"
#include "../ga/grid_graph_base.hpp"
#include "../ga/individual.hpp"
#include <string>
#include <fstream>
using std::string;

void EdgeFileWriter::save(const Individual& indiv, const string& filepath) {
    std::ofstream stream(filepath);

    for(int nA = 0; nA < numNode(); ++nA) {
        for(int d = 0; d < degree(); ++d) {
            int nB = indiv.adjacent[nA][d];

            if(nB == -1) continue;
            if(nB <= nA) continue;

            int columnA = getColumn(nA);
            int rowA = getRow(nA);
            int columnB = getColumn(nB);
            int rowB = getRow(nB);

            stream << columnA << "," << rowA << " " << columnB << "," << rowB << std::endl;
        }
    }
}