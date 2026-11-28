#ifndef PAGER_MFU_H
#define PAGER_MFU_H

#include "pager/pager.h"


class PagerMFU : Pager {
    protected:

    public:
    using Pager::Pager;
    saida_pag access() override;
};

#endif
