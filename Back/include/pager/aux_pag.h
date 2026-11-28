#ifndef AUX_PAG_H
#define AUX_PAG_H

#include <vector>

enum acctype {
    READ,
    WRITE
};

struct acc {
    int page_id;
    acctype at;
};

enum horm {
    HIT,
    MISS
};

struct saida_pag {
    std::vector<horm> horms;
    int disk_writes;
};

#endif
