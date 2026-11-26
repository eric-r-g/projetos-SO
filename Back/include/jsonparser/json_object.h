#include <vector>
#include <string>
#include <sstream>

struct json_object {
    std::vector<std::string> key;
    std::vector<std::string> val;
    template<typename T1, typename T2> void add(T1 k, T2 v) {
        std::ostringstream osk, osv;
        osk << k;
        osv << v;
        key.push_back(osk.str());
        val.push_back(osv.str());
    }
    std::string text();
};
