#ifndef PAGER_H
#define PAGER_H

#include "aux_pag.h"

#include <vector>
#include <set>

class Pager {
    protected:
    std::set<int> pages;
    std::set<int> dirty;
    int available;
    std::vector<acc> accseq;

    public:
    Pager(int sz, std::vector<acc> accseq);
    virtual saida_pag access() = 0;
};

#endif
