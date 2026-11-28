#include "pager/pagerNRU.h"

struct pag_NRU{
    int t;
    int r, m;
    int id;

    bool operator <(const pag_NRU& o) const {
        int v = 2 * r + m;
        int vo = 2 * o.r + o.m;
        if(v != vo) return v < vo;
        if(t != o.t) return t < o.t;
        return id < o.id;
    }
};

saida_pag PagerNRU::access() {
    int t = 0;
    std::set <pag_NRU> s;

    for (acc a : accseq) {
	pag_NRU atual;
	atual.id = a.page_id;
	atual.r = 1;
	atual.m = 0;
	atual.t = t++;

	if (!pages.count(a.page_id)) {
	    sp.horms.push_back(MISS);
        
	    if(!available){
		sp.evictions++;
		int id_rem = (s.begin()) -> id;
		treat_dirty(id_rem);
		pages.erase(id_rem);
		s.erase(s.begin());
		available++;
	    }

	    pages.insert(a.page_id);
	    available--;
	} else {
	    sp.horms.push_back(HIT);
	    pag_NRU rem;
	    for(pag_NRU p : s){
		if(p.id == a.page_id){
		    atual.m |= p.m;
		    atual.t = p.t;
		    rem = p;
		    break;
		}
	    }
	    s.erase(rem);
	}

	if (a.at == WRITE) {
	    atual.m = 1;
	    dirty.insert(a.page_id);
	}
        
	s.insert(atual);
	if(t % 4 == 0){

	    std::vector <pag_NRU> rem;
	    for(pag_NRU p : s){
		if(p.r == 1)
		    rem.push_back(p);
	    }

	    for(pag_NRU p : rem){
		s.erase(p);
		p.r = 0;
		s.insert(p);
	    }
	}
    }

    return sp;
};
