#ifndef MEMORY_H
#define MEMORY_H

#include "aux_alloc.h"
#include <set>
#include <vector>

class memory{
    public:
    //memory(int sz);
    void init(int sz);
    std::pair <int, int> alloc(int n, alloc_mode am);
    bool freeid(int id);
    bool freeaddr(int addr);
    void show(); // definir saida
    void stats(); // definir saida
    void defrag(defrag_mode dm);

    //private:
    int memory_sz;
    int cnt_id;
    int next_fit;
    std::set <bloco> blocos;
    int ocuppation;
    int l_hole();
    int cnt_holes();
};

#endif
