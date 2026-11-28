#ifndef PAGER_FIFO_H
#define PAGER_FIFO_H

#include "pager/pager.h"

#include <queue>

class PagerFIFO : Pager {
    protected:
    std::queue<int> q;

    public:
    using Pager::Pager;
    saida_pag access() override;
};

#endif
