#include "jsonparser/json_array.h"

std::string json_array::text() {
    std::string txt = "";
    txt += "[\n";
    int qtt = val.size();
    for (int i = 0; i < qtt; i++) {
        txt += val[i];
        if (i < qtt - 1) txt += ",";
        txt += "\n";
    }
    txt += "]";

    return txt;
}
