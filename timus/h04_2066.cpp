#include <iostream>
#include <limits>
#include <algorithm>
#include <cassert>

int F(int a, int b, char sign) {
    if (sign == '+') {
        return a + b;
    } else if (sign == '-') {
        return a - b;
    } else if (sign == '*') {
        return a * b;
    } else {
        assert(false);
    }
}

int main() {
    int a, b, c;
    std::cin >> a >> b >> c;

    int answer = std::numeric_limits<int>::max();

    for (char sign1 : {'+', '-', '*'}) {
        for (char sign2 : {'+', '-', '*'}) {
            int tmp;
            if (sign2 == '*') {
                tmp = F(a, (b * c), sign1);
            } else {
                tmp = F(F(a, b, sign1), c, sign2);
            }
            answer = std::min(answer, tmp);
        }
    }

    std::cout << answer;

    return 0;
}