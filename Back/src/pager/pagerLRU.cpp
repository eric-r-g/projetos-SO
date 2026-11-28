#include "pager/pagerLRU.h"

saida_pag PagerLRU::access() {
    saida_pag sp;
    for (acc a : accseq) {
	if (!pages.count(a.page_id)) {
	    sp.horms.push_back(MISS);
	    if (!available) {
		sp.evictions++;
		treat_dirty();
		auto[t, page_id] = last_use.top();
		last_use.pop();
		while (!pages.count(page_id)) {
		    auto[t, page_id] = last_use.top();
		    last_use.pop();
		}
		pages.erase(page_id);
		available++;
	    }
	    pages.insert(a.page_id);
	    available--;
	} else {
	    sp.horms.push_back(HIT);
	}
	if (a.at == WRITE) {
	    dirty.insert(a.page_id);
	}

	last_use.push(std::make_pair(curr_time, a.page_id));
    }

    return sp;
};
