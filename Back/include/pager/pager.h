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
    saida_pag sp;

    public:
    Pager(int sz, std::vector<acc> accseq);
    void treat_dirty(int page_id);
    virtual saida_pag access() = 0;
};

#endif
