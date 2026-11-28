#include "pager/pagerLFU.h"

struct pag_LFU{
    int t;
    int uses;
    int id;

    bool operator <(const pag_LFU& o) const {
        if(uses != o.uses) return uses < o.uses;
        if(t != o.t) return t < o.t;
        return id < o.id;
    }
};


saida_pag PagerLFU::access() {
    int t = 0;
    std::set <pag_LFU> s;

    for (acc a : accseq) {
    pag_LFU atual;
    atual.id = a.page_id;
    atual.t = t++;

	if (!pages.count(a.page_id)) {
	    if (!available) {
            int id_rem = (*s.begin()).id;
            treat_dirty(id_rem);
            pages.erase(id_rem);
            s.erase(s.begin());
            available++;
	    }

        atual.uses = 1;
        pages.insert(a.page_id);
        available--;
	} else {
        sp.horms.push_back(HIT);
        pag_LFU rem;
        for(pag_LFU p : s){
            if(p.id == a.page_id){
                atual.uses = p.uses + 1;
                atual.t = p.t;
                rem = p;
                break;
            }
        }
        s.erase(rem);
	}

    s.insert(atual);
	if (a.at == WRITE) {
	    dirty.insert(a.page_id);
    }
    }

    return sp;
};
