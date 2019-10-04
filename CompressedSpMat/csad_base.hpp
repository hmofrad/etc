/*
 * CSD_BASE.hpp: Base class for Compressed Sparse Column (CSC) data structure
 * (c) Mohammad Mofrad, 2018
 * (e) m.hasanzadeh.mofrad@gmail.com 
 */

#ifndef CSAD_BASE_HPP
#define CSAD_BASE_HPP

#include <sys/mman.h>
#include <cstring> 

struct CSAD_BASE {
    public:
        CSAD_BASE(uint64_t nnz_, uint32_t ncols_);
        ~CSAD_BASE();
        uint64_t nnz;
        uint32_t ncols;
        uint64_t size;    
        void* A;  // WEIGHT
        void* IA; // ROW_IDX
        void* JA; // COL_PTR

        //void* JI; // COL_IDX
        
};

CSAD_BASE::CSAD_BASE(uint64_t nnz_, uint32_t ncols_) {
    nnz = nnz_;
    ncols = ncols_;
    
    if((A = mmap(nullptr, nnz * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == (void*) -1) {    
        fprintf(stderr, "Error mapping memory\n");
        exit(1);
    }
    memset(A, 0, nnz * sizeof(uint32_t));
    
    if((IA = mmap(nullptr, nnz * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == (void*) -1) {    
        fprintf(stderr, "Error mapping memory\n");
        exit(1);
    }
    memset(IA, 0, nnz * sizeof(uint32_t));
    /*
    if((JI = mmap(nullptr, nnz * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == (void*) -1) {    
        fprintf(stderr, "Error mapping memory\n");
        exit(1);
    }
    memset(JI, 0, nnz * sizeof(uint32_t));
    */
    if((JA = mmap(nullptr, (ncols + 1) * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == (void*) -1) {    
        fprintf(stderr, "Error mapping memory\n");
        exit(1);
    }
    memset(JA, 0, (ncols + 1) * sizeof(uint32_t));
    
    
    
    size = (nnz * sizeof(uint32_t)) + (nnz * sizeof(uint32_t)) + ((ncols + 1) * sizeof(uint32_t));
}

CSAD_BASE::~CSAD_BASE() {
    if(munmap(A, nnz * sizeof(uint32_t)) == -1) {
        fprintf(stderr, "Error unmapping memory\n");
        exit(1);
    }
    
    if(munmap(IA, nnz * sizeof(uint32_t)) == -1) {
        fprintf(stderr, "Error unmapping memory\n");
        exit(1);
    }
    /*
    if(munmap(JI, nnz * sizeof(uint32_t)) == -1) {
        fprintf(stderr, "Error unmapping memory\n");
        exit(1);
    }
    */
    if(munmap(JA, (ncols + 1) * sizeof(uint32_t)) == -1) {
        fprintf(stderr, "Error unmapping memory\n");
        exit(1);
    }
}
#endif