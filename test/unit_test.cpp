#include<iostream>
#include<cstdio>
#include<time.h>
#include "../ga/individual.hpp"
#include "../ga/evaluate/adj_aspl.hpp"
#include "../io/edges_file_reader.hpp"
#include "unit_test.hpp"
using namespace std;

void UnitTest::adjAsplTest() {
    int numData = 7;
    string label[] = {
        "n5x10 d4 r2", 
        "n10x10 d4 r2", 
        "n10x10 d6 r3", 
        "n10x10 d8 r4", 
        "n100x100 d4 r2", 
        "n100x100 d6 r3", 
        "n100x100 d8 r4"
    };
    string filePath[] {
        "edges_file/w5h10d4r2_best.edges", 
        "edges_file/w10h10d4r2_best.edges", 
        "edges_file/w10h10d6r3_best.edges",
        "edges_file/w10h10d8r4_best.edges",
        "edges_file/w100h100d4r2_best.edges",
        "edges_file/w100h100d6r3_best.edges",
        "edges_file/w100h100d8r4_best.edges"
    };
    int numLoop[] = { 100000, 10000, 10000, 10000, 100, 100, 100 };
    int correctDiameter[] = {7, 9, 6, 5, 99, 66, 50};
    double correctAspl[] = {
        3.106938775510204, 
        3.985858585858586, 
        2.903030303030303,
        2.426262626262626,
        33.672444684468445,
        22.574951135113512, 
        17.06150103010301
    };

    cout << "ADJ ASPL ==================================================" << endl;
    for(int num = 0; num < numData; ++num) {
        cout << label[num] << " : ";

        Individual indiv = EdgeFileReader::GenerateIndividual(filePath[num]);
        ADJ_ASPL evaluater;

        clock_t startTime = clock();
        for(int i = 0; i < numLoop[num]; ++i) {
            evaluater(indiv);
        }
        clock_t endTime = clock();

        if(indiv.diameter != correctDiameter[num]) {
            cout << "failed" << endl;
            cout << indiv.diameter << " " << indiv.aspl << endl; 
            continue;
        }
        if(indiv.aspl != correctAspl[num]) {
            cout << "failed" << endl;
            cout << indiv.diameter << " " << indiv.aspl << endl;
            continue;
        }
    
        double time = (((double)(endTime - startTime) / CLOCKS_PER_SEC) * 1000) / numLoop[num];
        cout << "success  " << time << "ms" << endl;
    }
}
