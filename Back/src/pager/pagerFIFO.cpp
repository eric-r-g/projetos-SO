#include "pager/pagerFIFO.h"

saida_pag PagerFIFO::access() {
    for (acc a : accseq) {
	if (!pages.count(a.page_id)) {
	    sp.horms.push_back(MISS);
	    if (!available) {
		treat_dirty(q.front());
		pages.erase(q.front());
		q.pop();
		available++;
	    }
	    pages.insert(a.page_id);
	    q.push(a.page_id);
	    available--;
	} else {
	    sp.horms.push_back(HIT);
	}
	if (a.at == WRITE) {
	    dirty.insert(a.page_id);
	}
    }

    return sp;
};
