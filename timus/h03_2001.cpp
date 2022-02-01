#include <iostream>

int main() {
    int a[3], b[3];

    for (int i = 0; i < 3; ++i) {
        std::cin >> a[i] >> b[i];
    }

    std::cout << a[0] - a[2] << ' ' << b[0] - b[1] << "\n";
    return 0;
}