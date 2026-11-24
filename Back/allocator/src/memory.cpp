#include "../include/memory.h"

memory::memory(int sz){
    memory_sz = sz;
    cnt_id = 1;

    blocos.insert({-1, 0, 0});
    blocos.insert({-1, sz, 0});
}

bool memory::alloc(int n, alloc_mode am){
    bloco new_b = {cnt_id, -1, n};

    int fim, cand = -1;
    switch (am) {
    case FIRST:
        fim = 0;
        for(bloco b : blocos){
            if(b.ini - fim >= n){
                new_b.ini = fim;
                blocos.insert(new_b);
                return true;
            }
            fim = b.ini + b.sz;
        }
        return false;


    case BEST: // TODO: correção
        fim = 0, cand = -1;
        int cand_sz = n + 1;
        for(bloco b : blocos){
            if(b.ini - fim >= std::min(n, cand_sz)){
               cand = fim;
               cand_sz = b.ini - fim;
            }
            fim = b.ini + b.sz;
        }

        if(cand != -1){
            new_b.ini = cand;
            blocos.insert(new_b);
            return true;
        }
        return false;

    case WORST:
    
        break;

    case NEXT:
    
        break;
    
    default:
        return false;
    }
}