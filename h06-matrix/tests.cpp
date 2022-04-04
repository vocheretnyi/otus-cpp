#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <unordered_map>
#include <vector>

#include "matrix.hpp"

TEST(Hash, StdHashWithArray) {
    std::hash<std::array<int, 3>> hasher1;
    size_t res1 = hasher1({1, 2, 3});
    ASSERT_GT(res1, 0);
    size_t res2 = hasher1({3, 2, 1});
    ASSERT_GT(res2, 0);
    ASSERT_NE(res1, res2);

    std::hash<std::array<int, 3>> hasher2;
    size_t res3 = hasher2({1, 2, 3});
    ASSERT_EQ(res1, res3);
}

TEST(Hash, StdUnorderedMapWithArray) {
    std::unordered_map<std::array<int, 3>, int> hashtable;
    hashtable[{1, 2, 3}] = 1;
    hashtable[{1, 3, 2}] = 2;
    ASSERT_EQ(hashtable.size(), 2);
    ASSERT_TRUE(hashtable.count({1, 2, 3}));
    ASSERT_FALSE(hashtable.count({3, 2, 1}));
    ASSERT_EQ(hashtable.bucket_count(), 2);
    hashtable[{1, 2, 3}] = 3;
    ASSERT_EQ((hashtable[{1, 2, 3}]), 3);
    ASSERT_EQ((hashtable[{1, 3, 2}]), 2);
    ASSERT_EQ((hashtable[{1, 4, 2}]), 0);
    ASSERT_EQ(hashtable.size(), 3);
}

TEST(Matrix, Size) {
    Matrix<int, -1> matrix;
    ASSERT_EQ(matrix.size(), 0);
    matrix[0][0] = -1;
    ASSERT_EQ(matrix.size(), 0);
    matrix[1][0] = 0;
    ASSERT_EQ(matrix.size(), 1);
    matrix[0][1] = 3;
    ASSERT_EQ(matrix.size(), 2);
    matrix[1][0] = -1;
    ASSERT_EQ(matrix.size(), 1);
    int x = matrix[1][2];
    ASSERT_EQ(matrix.size(), 1);
    ASSERT_EQ(x, -1);
}

TEST(Matrix, StdOstream) {
    std::stringstream ss;
    Matrix<int, -1> matrix;
    ss << matrix[0][0];
    matrix[0][0] = 1;
    ss << matrix[0][0];
    auto x = matrix[0][1];
    ss << x;
    int y = matrix[1][0];
    ss << y;
    ASSERT_EQ(ss.str(), "-11-1-1");
}

TEST(Matrix, DefaultValue) {
    constexpr int defaultValue = -3;
    Matrix<int, defaultValue> matrix;
    ASSERT_EQ(matrix.default_value, defaultValue);
    ASSERT_EQ((Matrix<int, -3>::default_value), defaultValue);
    ASSERT_EQ(matrix[0][0], defaultValue);
    matrix[0][0] = 0;
    ASSERT_NE(matrix[0][0], defaultValue);
    matrix[0][0].setDefault();
    ASSERT_EQ(matrix[0][0], defaultValue);
}

TEST(Matrix, SetValues) {
    Matrix<int, -1> matrix;
    ((matrix[100][100] = 314) = 0) = 217;
    ASSERT_EQ(matrix[100][100], 217);
    matrix[0][0] = 3;
    int w = 4;
    ((matrix[100][100] = w) = matrix.default_value) = 214;
    ASSERT_EQ(matrix[100][100], 214);
    ((matrix[100][100] = matrix.default_value) = 4) = matrix.default_value;
    ASSERT_EQ(matrix[100][100], matrix.default_value);
}

TEST(Matrix, Clear) {
    Matrix<int, -1> matrix;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            matrix[i][j] = i * j;
        }
    }
    ASSERT_EQ(matrix.size(), 100);
    matrix.clear();
    ASSERT_EQ(matrix.size(), 0);
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            ASSERT_EQ(matrix[i][j], matrix.default_value);
        }
    }
}

TEST(Matrix, Matrix3D) {
    Matrix<int, 0, 3> matrix;
    size_t size = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                matrix[i][j][k] = i * j * k;
                size += (i * j * k) > 0;
                ASSERT_EQ(matrix.size(), size);
            }
        }
    }
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                ASSERT_EQ(matrix[i][j][k], i * j * k);
                matrix[i][j][k].setDefault();
            }
        }
    }
    ASSERT_EQ(matrix.size(), 0);
}

TEST(Matrix, BigIndexes) {
    constexpr size_t multiplier = 1'000'000;
    Matrix<int, -1> matrix;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            matrix[i * multiplier][j * multiplier] = i * j + 1;
        }
    }
    ASSERT_EQ(matrix.size(), 100);
    constexpr size_t largestIndex = std::numeric_limits<size_t>::max();
    ASSERT_EQ(matrix[largestIndex][0], matrix.default_value);
    ASSERT_EQ(matrix[0][largestIndex], matrix.default_value);
    ASSERT_EQ(matrix[largestIndex][largestIndex], matrix.default_value);
    matrix[largestIndex][largestIndex] = matrix.default_value;
    ASSERT_EQ(matrix.size(), 100);
}

TEST(Matrix, RangeBasedFor) {
    Matrix<int, -1> matrix;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix[i][j] = i * j;
        }
    }
    std::vector<std::tuple<size_t, size_t, int>> elements;
    for (auto cell : matrix) {
        std::array<size_t, 2> indexes{};
        size_t row, column;
        int value;
        std::tie(indexes, value) = cell;
        std::tie(row, column) = indexes;
        elements.emplace_back(row, column, value);
    }
    std::sort(elements.begin(), elements.end());
    ASSERT_EQ(elements.size(), 9);
    size_t curIndex = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(i, std::get<0>(elements[curIndex]));
            ASSERT_EQ(j, std::get<1>(elements[curIndex]));
            ASSERT_EQ(i * j, std::get<2>(elements[curIndex]));
            ++curIndex;
        }
    }
}

TEST(Matrix, DifferentTypeCasting) {
    Matrix<int, -1> matrix;
    ASSERT_EQ(matrix[0][0], -1.0f);
    matrix[0][0] = 'a';
    ASSERT_EQ(matrix[0][0], 97);
    matrix[0][1] = true;
    matrix[1][1] = 1u;
    matrix[2][2] = 4LL;
    long long resLL = matrix[2][2];
    ASSERT_EQ(resLL, 4);
    double doubleValue = 7.5;
    matrix[3][3] = doubleValue;
    ASSERT_NE(matrix[3][3], doubleValue);
    ASSERT_EQ(matrix[3][3], 7);
}

TEST(Matrix, IntermediateCell) {
    Matrix<int, -1> matrix;
    auto row = matrix[0];
    ASSERT_THROW(row.setDefault(), std::out_of_range);
    ASSERT_THROW(__attribute__((unused)) int val = row, std::out_of_range);
    ASSERT_THROW(matrix[0][0][0], std::out_of_range);
    ASSERT_THROW(matrix[0] = 1, std::out_of_range);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
