#include <fstream>
#include <iostream>

#include "ip_filter.hpp"

int main() {
    std::fstream input(std::string(RESOURCES_PATH) + "/ip_filter.tsv");
    Solve(input, std::cout);
    return 0;
}
