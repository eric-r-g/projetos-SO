#include "pager/pager.h"

Pager::Pager(int sz, std::vector<acc> accseq) {
    this->available = sz;
    this->accseq = accseq;
}

void Pager::treat_dirty(int page_id) {
    if (dirty.find(page_id) != dirty.end()) {
	sp.disk_writes++;
	dirty.erase(page_id);
    }
}
