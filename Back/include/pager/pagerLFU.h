#ifndef PAGER_LFU_H
#define PAGER_LFU_H

#include "pager/pager.h"


class PagerLFU : Pager {
    protected:

    public:
    using Pager::Pager;
    saida_pag access() override;
};

#endif
