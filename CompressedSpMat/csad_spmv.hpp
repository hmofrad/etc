/*
 * csad_spmv.hpp: CSAD SpMV implementation
 * (c) Mohammad Mofrad, 2018
 * (e) m.hasanzadeh.mofrad@gmail.com 
 */

#ifndef CSAD_SPMV_HPP
#define CSAD_SPMV_HPP

#include <chrono>

#include "pair.hpp" 
#include "io.cpp" 
#include "csad_base.hpp" 

class CSAD {
    public:
        CSAD() {};
        CSAD(const std::string file_path_, const uint32_t nvertices_, const uint32_t niters_) 
            : file_path(file_path_), nvertices(nvertices_), niters(niters_) {}
        ~CSAD() {};
        virtual void run_pagerank();
    protected:
        std::string file_path = "\0";
        uint32_t nvertices = 0;
        uint32_t niters = 0;
        uint64_t nedges = 0;
        uint32_t nrows = 0;
        uint32_t diameter = 0;
        uint32_t ndiagonals = 0;
        std::vector<struct Pair>* pairs = nullptr;
        struct CSAD_BASE* csad = nullptr;
        std::vector<double> v;
        std::vector<double> d;
        std::vector<double> x;
        std::vector<double> y;
        double alpha = 0.15;
        uint64_t noperations = 0;
        uint64_t total_size = 0;
        std::vector<char> rows;

        virtual void construct_filter();
        virtual void destruct_filter();
        void populate();
        void walk();
        void construct_vectors();
        void destruct_vectors();
        virtual void message();        
        virtual uint64_t spmv();
        virtual void update();
        virtual void space();
        void display(uint64_t nums = 10);
        double checksum();
        void stats(double elapsed_time, std::string type);
        void adiag_sort(std::vector<struct Pair>*);
};

struct ADiagSort {
    bool operator()(const struct Pair& a, const struct Pair& b) {
        //printf("%d %d vs %d %d\n", a.row, a.col, b.row, b.col);
        
        if((a.row + a.col) == (b.row + b.col)) {
            if((a.row < b.row) and (a.col > b.col)) {
                return(true);
            }
            else 
                return(false);
        }
        else {
            if((a.row + a.col) < (b.row + b.col))
                return(true);
            else
                return(false);
        }
    }
};

void CSAD::adiag_sort(std::vector<struct Pair>*) {

    ADiagSort f_diag;
    std::sort(pairs->begin(), pairs->end(), f_diag);    
    
    //std::vector<std::vector<char> > A{{'a', 'b', 'c'}, {'d', 'e', 'f'}, {'g', 'h', 'i'}};
    /*
    std::vector<std::vector<char> > M{{'a', 'b', 'c', 'd'}, {'e', 'f', 'g', 'h'}, {'i', 'j', 'k', 'l'}, {'m', 'n', 'o', 'p'}};    
    int m = 4;
    int n = (2 * m) - 1;
    
    int d = n/2;
    int i = 0, j = 0, k = 0;
    int i1 = 0, i2 = 0, j1 = 0, j2 = 0;
    for(k = 0; k <= d; k++) {
        printf("k=%d i1=%d i2=%d, j1=%d j2=%d\n", k, k/(d+1), k%(d+1), k%(d+1), k/(d+1));
        i1 = k/(d+1);
        i2 = k%(d+1);
        j1 = k%(d+1);
        j2 = k/(d+1);
        j = j1;
        for(i = i1; i <= i2; i++) {
            printf("[%d %d %c] ", i, j, M[i][j]);
            //printf("%d %d\n", i, j);
            j--;
        }
        printf("\n");
    }
    printf("\n");
    for(k = d + 1; k < n; k++) {
        printf("k=%d i1=%d i2=%d, j1=%d j2=%d\n", k, k/(d+1) + k%(d+1), m-1, m-1, k/(d+1) + k%(d+1));
        i1 = k/(d+1) + k%(d+1);
        i2 = m-1;
        j1 = m-1;
        j2 = k/(d+1) + k%(d+1);
        j = j1;
        for(i = i1; i <= i2; i++) {
            printf("[%d %d %c] ", i, j, M[i][j]);
            //printf("%d %d\n", i, j);
            j--;
        }
        printf("\n");
        
    }
    printf("\n");
    
    //for(k = 0; k < n; k++) {
    //    printf("k=%d i1=%d i2=%d, j1=%d [%d, %d]\n", k, k/(d+1), k%(d+1), k/(d+2), k/(d+1) + k/(d+2), k/d);
    //}
    //printf("\n");
    
    
    std::vector<struct Pair> *list = new std::vector<struct Pair>;

    
    //column_sort(list);
    for(uint32_t i = 0; i < m; i++) {
        for(uint32_t j = 0; j < m; j++) {
            list->push_back({i,j});
        }
    }
    
    //for(auto &pair: list) {
    //    printf("%d %d\n", pair.row, pair.col);
    //}
    
    std::random_shuffle(list->begin(), list->end());
    
    DiagSort f_diag;
    std::sort(list->begin(), list->end(), f_diag);    
    std::exit(0);
    */
    //std::sort(pairs->begin(), pairs->end(), f_diag);    
    
    //for(auto &pair: *list) {
    //for(auto &pair: *pairs) {
        //printf("%d %d %c\n", pair.row, pair.col, M[pair.row][pair.col]);
        //printf("%d %d\n", pair.row, pair.col);
    //}
    
    
    //csd = new struct CSD_BASE(nedges, nvertices);
    //csd = new struct CSD_BASE(m^2, n);
    /*
    uint32_t* A  = (uint32_t*) csd->A;  // WEIGHT      
    uint32_t* IA = (uint32_t*) csd->IA; // ROW_IDX
    uint32_t* JA = (uint32_t*) csd->JA; // COL_PTR
    uint32_t ncols = csd->ncols - 1;

    uint32_t i = 0;
    uint32_t j = 1;
    JA[0] = 0;    
    for(auto &pair: *list) {
        printf("%d %d %c\n", pair.row, pair.col, M[pair.row][pair.col]);
        while((j - 1) != (pair.row + pair.col)) {
            j++;
            JA[j] = JA[j - 1];
        }      
        A[i] = M[pair.row][pair.col];
        JA[j]++;
        IA[i] = pair.row;
        i++;        
    }
    while((j + 1) < (ncols + 1)) {
        j++;
        JA[j] = JA[j - 1];
    }
*/    
    
    
    
    /*

    for(auto& pair : *pairs) {
        while((j - 1) != pair.col) {
            j++;
            JA[j] = JA[j - 1];
        }                  
        A[i] = 1;
        JA[j]++;
        IA[i] = pair.row;
        i++;
    }
    while((j + 1) < (ncols + 1)) {
        j++;
        JA[j] = JA[j - 1];
    }
    */
    
  /*  
    for(uint32_t j = 0; j < ncols; j++) {
        printf("j=%d\n", j);
        for(uint32_t i = JA[j]; i < JA[j + 1]; i++) {
            printf("    i=%d, j=%d, value=%c\n", IA[i], j, A[i]);
        }
    }
    
    
    
    
    
    std::exit(0);
    
*/
    //pairs->begin(), pairs->end()
    
}


void CSAD::run_pagerank() {
    // Degree program
    nrows = nvertices;
    diameter = nvertices / 2;
    ndiagonals = (2 * nvertices) - 1;
    
    pairs = new std::vector<struct Pair>;
    nedges = read_binary(file_path, pairs);
    //column_sort(pairs);
    adiag_sort(pairs);
    
    csad = new struct CSAD_BASE(nedges, ndiagonals);
    //csd = new struct CSD_BASE(nedges, nvertices);
    populate();
    struct Pair B = pairs->back();
    struct Pair B1 = pairs->front();
    //pairs->clear();
    //pairs->shrink_to_fit();
    //pairs = nullptr;  
    //walk();
    printf("%d %d\n", B1.row, B1.col);
    printf("%d %d\n", B.row, B.col);
    //std::exit(0);
    
    construct_vectors();
    //printf("1\n");
    //(void)spmv();
    noperations = spmv();
    //printf("2\n");
    int s = 0;
    //for(int i = 0; i < nrows; i++)
    //    s += y[i];
        
    v = y;
    printf("checkum=%f %lu\n", checksum(), noperations);
    //(void)checksum();
    display();
    delete csad;
    csad = nullptr;
    //std::exit(0);
    // PageRank program
    pairs = new std::vector<struct Pair>;
    nedges = read_binary(file_path, pairs, true);
    //column_sort(pairs);
    adiag_sort(pairs);
    construct_filter();
    csad = new struct CSAD_BASE(nedges, ndiagonals);
    populate();
    total_size += csad->size;
    pairs->clear();
    pairs->shrink_to_fit();
    pairs = nullptr;
    
    for(uint32_t i = 0; i < nrows; i++) {
        if(rows[i] == 1)
            d[i] = v[i];
    }  
    //d = v;
    std::fill(v.begin(), v.end(), alpha);
    std::chrono::steady_clock::time_point t1, t2;
    t1 = std::chrono::steady_clock::now();
    for(uint32_t i = 0; i < niters; i++)
    {
        std::fill(x.begin(), x.end(), 0);
        std::fill(y.begin(), y.end(), 0);
        message();
        noperations += spmv();
        update();
    }
    t2 = std::chrono::steady_clock::now();
    auto t  = (std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    stats(t, "CSAD SpMV");
    display();
    destruct_vectors();
    destruct_filter();  
    delete csad;
    csad = nullptr;
}

void CSAD::construct_filter() {
    rows.resize(nvertices);
    for(auto &pair: *pairs)
        rows[pair.row] = 1;
}

void CSAD::destruct_filter() {
    rows.clear();
    rows.shrink_to_fit();
}

void CSAD::populate() {
    uint32_t* A  = (uint32_t*) csad->A;  // WEIGHT      
    uint32_t* IA = (uint32_t*) csad->IA; // ROW_IDX
    uint32_t* JA = (uint32_t*) csad->JA; // COL_PTR
    //uint32_t* JI = (uint32_t*) csad->JI; // COL_IDX
    uint32_t ncols = csad-> ncols - 1;
    uint32_t i = 0;
    uint32_t j = 1;
    JA[0] = 0;
    for(auto& pair : *pairs) {
        while((j - 1) != (pair.row + pair.col)) {
            j++;
            JA[j] = JA[j - 1];
        }      
        A[i] = 1;
        JA[j]++;
        IA[i] = pair.row;
        //JI[i] = pair.col;
        i++;
    }
    while((j + 1) < (ncols + 1)) {
        j++;
        JA[j] = JA[j - 1];
    }
}

void CSAD::walk() {
    uint32_t* A  = (uint32_t*) csad->A;
    uint32_t* IA = (uint32_t*) csad->IA;
    uint32_t* JA = (uint32_t*) csad->JA;
    //uint32_t* JI = (uint32_t*) csd->JI;
    uint32_t ncols = csad->ncols;
    /*
    for(uint32_t j = 0; j < ncols; j++) {
        printf("j=%d\n", j);
        for(uint32_t i = JA[j]; i < JA[j + 1]; i++) {
            printf("    i=%d, j=%d, value=%d\n", IA[i], j, A[i]);
        }
    }
    */
    //uint32_t d = nvertices / 2;
    //int n = (2 * nvertices) - 1;
    int j = 0;
    int k = 0;
    int l = 0;
    for(j = 0; j <= diameter; j++) {
        printf("j=%d\n", j);
        
        for(uint32_t i = JA[j]; i < JA[j + 1]; i++) {
            k = j-IA[i];
            //printf(" A[%d,%d]=%d i=%d j=%d %d\n", IA[i], JI[i], A[i], i, j, k);
            
            //printf("    i=%d, j=%d, value=%d col=%d\n", IA[i], j, A[i], JI[i]);
            //printf("    i=%d, j=%d, value=%d col=%d %d %d\n", IA[i], j, A[i], JI[i], j%(d+1), j/(d+1));
        }
        /*
        printf("k=%d i1=%d i2=%d, j1=%d j2=%d\n", k, k/(d+1), k%(d+1), k%(d+1), k/(d+1));
        i1 = k/(d+1);
        i2 = k%(d+1);
        j1 = k%(d+1);
        j2 = k/(d+1);
        j = j1;
        for(i = i1; i <= i2; i++) {
            printf("[%d %d %c] ", i, j, M[i][j]);
            //printf("%d %d\n", i, j);
            j--;
        }
        printf("\n");
        */
    }
    printf("\n");
    printf("++++++++++++++++++++++++++++++\n");
    /*
    int h = 0;
    for(auto& pair: *pairs) {
        printf("%d %d %d\n", h, pair.row, pair.col);
        h++;
        if(h > 10)
            break;
    }
    */

 //ndiagonals
    for(j = diameter + 1; j < diameter + 5; j++) {
        //int l = j/(diameter+1) + j%(diameter+1) + diameter + 1;
        printf("j=%d \n", j);
        for(uint32_t i = JA[j]; i < JA[j + 1]; i++) {
            k = j-IA[i];
            //printf(" A[%d,%d]=%d i=%d j=%d %d\n", IA[i], JI[i], A[i], i, j, k);
            //printf("    i=%d, j=%d, value=%d  %d\n", IA[i], j, A[i], l);
            //printf("    i=%d, j=%d, value=%d col=%d %d\n", IA[i], j, A[i], JI[i], l);
            //printf("    i=%d, j=%d, value=%d col=%d %d %d\n", IA[i], j, A[i], JI[i], nvertices-1, j/(d+1) + j%(d+1) + d + 1);
        }
    }
    
    printf("\n");
    std::exit(0);
   
}

void CSAD::construct_vectors() {
    v.resize(nrows);
    x.resize(nrows, 1);
    y.resize(nrows);
    d.resize(nrows);
}

void CSAD::destruct_vectors() {
    v.clear();
    v.shrink_to_fit();
    x.clear();
    x.shrink_to_fit();
    y.clear();
    y.shrink_to_fit();
    d.clear();
    d.shrink_to_fit();
}

void CSAD::message() {
    for(uint32_t i = 0; i < nrows; i++)
        x[i] = d[i] ? (v[i]/d[i]) : 0;   
}

uint64_t CSAD::spmv() {
    uint64_t noperations = 0;
    uint32_t* A  = (uint32_t*) csad->A;
    uint32_t* IA = (uint32_t*) csad->IA;
    uint32_t* JA = (uint32_t*) csad->JA;
    //uint32_t* JI = (uint32_t*) csad->JI;
    uint32_t ncols = csad->ncols;
    /*
    for(uint32_t j = 0; j < ncols; j++) {
        for(uint32_t i = JA[j]; i < JA[j + 1]; i++) {
            y[IA[i]] += (A[i] * x[j]);
            noperations++;
        }
    }
    */
    int j = 0;
    int k = 0;
    int l = 0;
    //std::vector<int> ll(nrows);
    for(j = 0; j < ndiagonals; j++) {
        
        //printf("j=%d\n", j);
        for(uint32_t i = JA[j]; i < JA[j + 1]; i++) {
            k = j-IA[i];
            //l = j%(k+1);
            //printf(" A[%d,%d]=%d\n", IA[i], JI[i], A[i]);
            //if(j%IA[i] != JI[i])  {
            //    printf("JI=%d j=%d k=%d l=%d/%d\n", JI[i], j, k, l, j%IA[i]);
            //    std::exit(0);
            //}
            y[IA[i]] += (A[i] * x[k]);
            noperations++;
            //ll[IA[i]]++;
        }
        
        
    }
    
    
      /*  
    for(uint32_t j = 0; j < nrows; j++) {
        printf("N[%d]=%d\n", j, ll[j]);
        l +=ll[j];
    }
    printf("Avg=%f/%d\n", (double) l/ncols, l);
    
    std::exit(0);
    */
    /*
    std::exit(0);
    for(j = k; j < ndiagonals; j++) {
        //int l = j/(diameter+1) + j%(diameter+1) + diameter + 1;
        l = (j/k) + (j%k) + k;
        //printf("%d %d\n", j, l);
        for(uint32_t i = JA[j]; i < JA[j + 1]; i++) {
            if(l != JI[i])  {
                printf("JI=%d j=%d k=%d l=%d\n", JI[i], j, k, l);
                std::exit(0);
            }
            y[IA[i]] += (A[i] * x[l]);
            noperations++;
        }
    }
    */
    
    
    return(noperations);
}

void CSAD::update() {
    for(uint32_t i = 0; i < nrows; i++)
        v[i] = alpha + (1.0 - alpha) * y[i];
}

void CSAD::space() {
    total_size += csad->size;
}

double CSAD::checksum() {
    double value = 0.0;
    for(uint32_t i = 0; i < nrows; i++)
        value += v[i];
    return(value);
}

void CSAD::display(uint64_t nums) {
    for(uint32_t i = 0; i < nums; i++)
        std::cout << "V[" << i << "]=" << v[i] << std::endl;
}

void CSAD::stats(double time, std::string type) {
    std::cout << type << " kernel unit test stats:" << std::endl;
    std::cout << "Utilized Memory: " << total_size / 1e9 << " GB" << std::endl;
    std::cout << "Elapsed time   : " << time / 1e6 << " Sec" << std::endl;
    std::cout << "Num Operations : " << noperations << std::endl;
    std::cout << "Final value    : " << checksum() << std::endl;
}
#endif