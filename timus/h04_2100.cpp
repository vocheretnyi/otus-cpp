#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    size_t guests = 2;
    for (std::string s; n > 0 && std::cin >> s; --n) {
        ++guests;
        if(s.ends_with("+one")) {
            ++guests;
        }
    }
    guests += (guests == 13);
    std::cout << guests * 100;
    return 0;
}