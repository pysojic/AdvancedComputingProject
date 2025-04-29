#include <cassert>
#include <algorithm>               // <— add this
#include "../include/LatencyAnalyzer.hpp"
#include <vector>
#include "config.h"

int main() {
    std::vector<long long> v = {10,20,30,40,50,60,70,80,90,100};
    analyzeLatencies(v);
    assert(*std::min_element(v.begin(), v.end()) == 10);
    assert(*std::max_element(v.begin(), v.end()) == 100);
    return 0;
}
