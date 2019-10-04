/*
 * main.cpp: Kernels
                   Compressed Sparse Column (CSC)   SpMV/SpMSpV
            Double Compressed Sparse Column (DCSC)  SpMV/SpMSpV
   Optmized Double Compressed Sparse Column (ODCSC) SpMSpV (LA3)
            Triple Compressed Sparse Column (TCSC)  SpMSpV (GraphTap)
 * (c) Mohammad Mofrad, 2018
 * (e) m.hasanzadeh.mofrad@gmail.com 
 * Compile: g++ -o main main.cpp -std=c++14 -DNDEBUG -O3 -flto -fwhole-program -march=native
 */
 
#include <iostream>
#include <vector>

uint32_t nrows_global = 0;

#include "csc_spmv.hpp"
#include "csd_spmv.hpp"
#include "csad_spmv.hpp"
#include "dcsc_spmv.hpp"
#include "dcsd_spmv.hpp"
/*


#include "dcsc_spmspv.hpp"
#include "odcsc_spmspv.hpp"
#include "tcsc_spmspv.hpp"
*/

int main(int argc, char** argv) { 
    if(argc != 5) {
        std::cout << "PageRank benchmark for testing compression algorithms" << std::endl;
        std::cout << "\"Usage: " << argv[0] << " <CSC SpMV|DCSC SpMV|CSC SpMSpV|DCSC SpMSpV|ODCSC SpMSpV(LA3)|TCSC SpMSpV(GraphTap)|> <file> <nvertices> <niters>\"" << std::endl;
        exit(1);
    }
    int which = std::atoi(argv[1]);
    std::string file_path = argv[2];
    uint32_t nvertices = std::atoi(argv[3]) + 1; // For vertex id 0
    uint32_t niters = std::atoi(argv[4]);
    if(which == 0) {
        CSC csc(file_path, nvertices, niters);
        csc.run_pagerank();
    }
    else if(which == 1) {
        CSAD csad(file_path, nvertices, niters);
        csad.run_pagerank();
    }
    else if(which == 2) {
        CSD csd(file_path, nvertices, niters);
        csd.run_pagerank();
    }
    else if(which == 3) {
        DCSC dcsc(file_path, nvertices, niters);
        dcsc.run_pagerank();
    }
    else if(which == 4) {
        DCSD dcsd(file_path, nvertices, niters);
        dcsd.run_pagerank();
    }
    /*
    else if(which == 1) {
        DCSC dcsc(file_path, nvertices, niters);
        dcsc.run_pagerank();
    }
    else if(which == 2) {
        CSC_ csc(file_path, nvertices, niters);
        csc.run_pagerank();
    }
    else if(which == 3) {
        DCSC_ dcsc(file_path, nvertices, niters);
        dcsc.run_pagerank();
    }
    else if(which == 4) {
        ODCSC odcsc(file_path, nvertices, niters);
        odcsc.run_pagerank();
    }
    else if(which == 5) {
        TCSC tcsc(file_path, nvertices, niters);
        tcsc.run_pagerank();
    }
    */
    return(0);
}