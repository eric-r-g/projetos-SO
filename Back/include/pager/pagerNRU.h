#ifndef PAGER_NRU_H
#define PAGER_NRU_H

#include "pager/pager.h"
#include <set>
#include <vector>

class PagerNRU : Pager {

    public:
    using Pager::Pager;
    saida_pag access() override;
};

#endif
