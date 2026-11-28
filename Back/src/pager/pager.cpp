#include "pager/pager.h"

Pager::Pager(int sz, std::vector<acc> accseq) {
    this->available = sz;
    this->accseq = accseq;
}
