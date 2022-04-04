#include <cassert>
#include <iostream>
#include <tuple>

#include "matrix.hpp"

int main() {
    Matrix<int, 0> matrix;
    for (int i = 0; i < 10; ++i) {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }
    std::cout << "Submatrix [1,1 - 8,8]:\n";
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "Matrix size: " << matrix.size() << "\n";
    assert(matrix.size() == 18);

    std::cout << "Matrix setted non-default cells:\n";
    for (auto cell : matrix) {
        std::array<size_t, 2> indexes{};
        size_t row, column;
        int value;
        std::tie(indexes, value) = cell;
        std::tie(row, column) = indexes;
        std::cout << row << ' ' << column << ' ' << value << "\n";
    }

    return 0;
}
