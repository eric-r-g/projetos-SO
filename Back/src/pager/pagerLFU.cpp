#include "pager/pagerLFU.h"

saida_pag PagerLFU::access() {
    saida_pag sp;
    for (acc a : accseq) {
	if (!pages.count(a.page_id)) {
	    if (!available) {
            if (dirty.find(0) != dirty.end()) {
                
            }
		
	    }
	    
	} else {

	}
	if (a.at == WRITE) {

    }
    }

    return sp;
};
