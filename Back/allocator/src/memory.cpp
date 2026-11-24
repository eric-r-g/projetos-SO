#include "../include/memory.h"

memory::memory(int sz){
    memory_sz = sz;
    cnt_id = 1;
    next_fit = 0;
    ocuppation = 0;

    blocos.insert({-1, 0, 0});
    blocos.insert({-1, sz, 0});
}

std::pair <int, int> memory::alloc(int n, alloc_mode am){
    bloco new_b = {cnt_id, -1, n};

    int fim, cand = -1, cand_sz;
    bool find = false;
    switch (am) {
    case FIRST:
        fim = 0;

        for(bloco b : blocos){
            int dist = b.ini - fim;
            if(dist >= n){
                new_b.ini = fim;
                find = true;
                break;
            }
            fim = b.ini + b.sz;
        }
        break;

    case BEST:
        fim = 0, cand = -1;
        cand_sz = memory_sz + 1;
        for(bloco b : blocos){
            int dist = b.ini - fim;
            if(dist >= n && dist < cand_sz){
               cand = fim;
               cand_sz = dist;
            }
            fim = b.ini + b.sz;
        }

        if(cand != -1){
            new_b.ini = cand;
            find = true;
        }
        break;

    case WORST:
        fim = 0, cand = -1;
        cand_sz = 0;
        for(bloco b : blocos){
            int dist = b.ini - fim;
            if(dist >= n && dist > cand_sz){
               cand = fim;
               cand_sz = dist;
            }
            fim = b.ini + b.sz;
        }

        if(cand != -1){
            new_b.ini = cand;
            find = true;
        }
        break;

    case NEXT:
        fim = 0;
        for(bloco b : blocos){
            if(b.ini < next_fit){
                fim = b.ini + b.sz;
                continue;
            }

            int dist = b.ini - fim;
            if(dist >= n){
                new_b.ini = fim;
                find = true;
                break;
            }
            fim = b.ini + b.sz;
        }

        if (!find){
            fim = 0;
            for(bloco b : blocos){
                if(b.ini >= next_fit)
                    break;

                int dist = b.ini - fim;
                if(dist >= n){
                    new_b.ini = fim;
                    find = true;
                    break;
                }
                fim = b.ini + b.sz;
            }
        }
        break;
    
    default:
        break;
    }

    if(find){
        next_fit = new_b.ini + new_b.sz;
        blocos.insert(new_b);
        cnt_id++;

        ocuppation += n;
        return {-1, -1}; // sucesso
    }

    std::pair <int, int> p_ret = {memory_sz - ocuppation, l_hole()};
    return p_ret; // falha
}

bool memory::freeid(int id){
    bloco remov;
    bool find = false;
    for(bloco b : blocos){
        if(b.id == id){
            find = true;
            remov = b;
            break;
        }
    }

    if(find){
        blocos.erase(remov);
        ocuppation -= remov.sz;
        return true;
    }
    return false;
}

bool memory::freeaddr(int addr){
    bloco remov;
    bool find = false;
    for(bloco b : blocos){
        if(b.ini == addr && b.id != -1){
            find = true;
            remov = b;
            break;
        }
    }

    if(find){
        blocos.erase(remov);
        ocuppation -= remov.sz;
        return true;
    }
    return false;
}

int memory::l_hole(){
    int fim = 0;
    int hole = 0;
    for(bloco b : blocos){
        hole = std::max(hole, b.ini - fim);
        fim = b.ini + b.sz;
    }
    return hole;
}