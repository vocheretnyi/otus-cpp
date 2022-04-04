#pragma once

#include <cstdlib>
#include <tuple>
#include <unordered_map>
#include <map>
#include <array>

#include <boost/container_hash/hash.hpp>

namespace std {
    template<typename T, size_t N>
    struct hash<std::array<T, N>> {
        std::size_t operator()(const std::array<T, N>& arr) const {
            return boost::hash_range(arr.begin(), arr.end());
        }
    };
}

template<typename T, T DefaultValue = T{}, size_t Dimension = 2>
class Matrix {
 private:
    class ProxyCellAccessor;

 public:
    static constexpr T default_value = DefaultValue;
    using index_type = size_t;

    Matrix() : proxyCell(*this) {}

    ProxyCellAccessor& operator[](index_type firstDimensionIndex) {
        proxyCell.resetCell();
        return proxyCell[firstDimensionIndex];
    }

    std::size_t size() const {
        return cells.size();
    }

    auto begin() {
        return cells.begin();
    }

    auto end() {
        return cells.end();
    }

    void clear() {
        proxyCell.resetCell();
        cells.clear();
    }

 private:
    ProxyCellAccessor proxyCell;
    std::unordered_map<std::array<index_type, Dimension>, T> cells;
};

template<typename T, T DefaultValue, size_t Dimension>
class Matrix<T, DefaultValue, Dimension>::ProxyCellAccessor {
    friend Matrix;

 public:
    operator T() const noexcept(false) {
        if (depth != Dimension) {
            throw std::out_of_range("You can work with intermediate dimension.");
        }

        if (auto it = matrix.cells.find(currentPath); it != matrix.cells.end()) {
            return it->second;
        } else {
            return matrix.default_value;
        }
    }

    ProxyCellAccessor& operator=(const T& other) noexcept(false) {
        if (depth != Dimension) {
            throw std::out_of_range("You can work with intermediate dimension.");
        }

        if (other == default_value) {
            if (auto it = matrix.cells.find(currentPath); it != matrix.cells.end()) {
                matrix.cells.erase(it);
            }
        } else {
            matrix.cells[currentPath] = other;
        }
        return *this;
    }

    void setDefault() noexcept(false) {
        if (depth != Dimension) {
            throw std::out_of_range("You can work with intermediate dimension.");
        }
        this->operator=(default_value);
    }

    ProxyCellAccessor& operator[](index_type index) noexcept(false) {
        if (depth >= Dimension) {
            throw std::out_of_range("You've used more dimensions than expected");
        }
        currentPath[depth] = index;
        ++depth;
        return *this;
    }

 private:
    ProxyCellAccessor(Matrix& matrix_) : depth(0), matrix(matrix_) {}

    void resetCell() {
        depth = 0;
    }

    std::array<index_type, Dimension> currentPath;
    size_t depth;
    Matrix& matrix;
};
