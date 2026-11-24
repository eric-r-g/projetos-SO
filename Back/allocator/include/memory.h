#ifndef MEMORY_H
#define MEMORY_H

#include "aux_alloc.h"
#include <set>

class memory{
    public:
    memory(int sz);
    bool alloc(int n, alloc_mode am);
    bool freeid();
    bool freeaddr();
    void show(); // definir saida
    void stats(); // definir saida
    void defrag();
    
    private:
        int memory_sz;
        int cnt_id;
        std::set <bloco> blocos;
};

#endif