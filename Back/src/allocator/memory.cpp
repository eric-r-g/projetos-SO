#include "../../include/allocator/memory.h"
#include <bits/stdc++.h>

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

std::vector<std::pair<std::string, int>> memory::show() {
    std::vector<std::pair<std::string, int>> saida;
    for (auto b : blocos) {
        saida.emplace_back("\"id\"", b.id);
        saida.emplace_back("\"ini\"", b.ini);
        saida.emplace_back("\"sz\"", b.sz);
    }

    return saida;
}

void memory::defrag(defrag_mode dm){
    std::vector <bloco> temp;
    
    int ini = 0, end = memory_sz;
    switch (dm)
    {
    case INIT:
        for (auto it = blocos.rbegin(); it != blocos.rend(); ++it) {
            bloco b = *it;
            b.ini = end - b.sz;
            end = b.ini;
            temp.push_back(b);
        }
        break;

    case END:
        for (auto it = blocos.begin(); it != blocos.end(); ++it) {
            bloco b = *it;
            b.ini = ini;
            ini = b.ini + b.sz;
            temp.push_back(b);
        }
    case OPT: {
        auto l = blocos.begin();
        auto r = blocos.end();
        
        while(l != r){
            bloco bl = *l;
            bloco br = *prev(r);
            
            int ldif = bl.ini - ini;
            int rdif = end - (br.ini + br.sz);
            if(ldif < rdif){
                bl.ini = ini;
                ini = bl.ini + bl.sz;
                temp.push_back(bl);
                l++;
            } else {
                br.ini = end - br.sz;
                end = br.ini;
                temp.push_back(br);
                r--;
            }
        }
        break;
        }
    default:
        break;
    }

    blocos.clear();
    for(bloco b : temp)
        blocos.insert(b);
}
