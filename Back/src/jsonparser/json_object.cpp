#include "jsonparser/json_object.h"

std::string json_object::text() {
    std::string txt = "";
    txt += "{\n";
    int qtt = key.size();
    for (int i = 0; i < qtt; i++) {
        txt += key[i];
        txt += ": ";
        txt += val[i];
        if (i < qtt - 1) txt += ",";
        txt += "\n";
    }
    txt += "}";

    return txt;
}
