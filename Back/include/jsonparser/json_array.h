#include <vector>
#include <string>
#include <sstream>

struct json_array {
    std::vector<std::string> val;
    template<typename T> void add(T v) {
        std::ostringstream osv;
        osv << v;
        val.push_back(osv.str());
    }
    std::string text();
};
