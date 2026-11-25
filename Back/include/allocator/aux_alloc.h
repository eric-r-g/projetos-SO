#ifndef AUX_ALLOC_H
#define AUX_ALLOC_H

enum alloc_mode{
    FIRST,
    BEST,
    WORST,
    NEXT
};

enum defrag_mode{
    INIT,
    END,
    OPT
};

struct bloco{
    int id;
    int ini;
    int sz;
    bloco(int nid, int nini, int nsz) : id(nid), ini(nini), sz(nsz){}
    bloco(){}

    bool operator <(const bloco& o) const {
        if(ini != o.ini) return ini < o.ini;
        return id < o.id;
    }
};

#endif