#ifndef PAGER_LRU_H
#define PAGER_LRU_H

#include "pager/pager.h"

class PagerLRU : Pager {
    protected:
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> last_use;
    int curr_time = 0;

    public:
    using Pager::Pager;
    saida_pag access() override;
};

#endif
