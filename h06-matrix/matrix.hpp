#pragma once

#include <cstdlib>
#include <tuple>
#include <unordered_map>
#include <map>
#include <array>

#include <boost/container_hash/hash.hpp>

namespace std {
/**
 * @brief std::hash definition for the std::array
 * @details implemented using boost::hash_range from boost/container_hash/hash.hpp
 * @tparam T Array's elements type
 * @tparam N Size of the array
 * @param arr Array
 */
    template<typename T, size_t N>
    struct hash<std::array<T, N>> {
        std::size_t operator()(const std::array<T, N>& arr) const {
            return boost::hash_range(arr.begin(), arr.end());
        }
    };
}  // namespace std

/**
 * @brief Matrix class for sparse matrix case
 * @details Supports multidimensional usage
 * @tparam T Type of elements
 * @tparam DefaultValue Default value of elements, by default T{}
 * @tparam Dimension Dimension of the matrix, by default 2
 */
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

/**
 * @brief Returns iterator on first element which is not equal to default value
 * @details Working in range based for
 * @return std::unordered_map<std::array<index_type, Dimension>, T>::iterator
 */
    auto begin() {
        return cells.begin();
    }

/**
 * @brief Returns iterator on the element after last which is not equal to default value
 * @details Working in range based for
 * @return std::unordered_map<std::array<index_type, Dimension>, T>::iterator
 */
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

/**
 * @brief Proxy class which allows accessing to matrix cells
 * @details Have implicit cast operator to T
 * @tparam T Type of elements
 * @tparam DefaultValue Default value of elements, by default T{}
 * @tparam Dimension Dimension of the matrix, by default 2
 */
template<typename T, T DefaultValue, size_t Dimension>
class Matrix<T, DefaultValue, Dimension>::ProxyCellAccessor {
    friend Matrix;

 public:
/**
 * @brief Implicit typecasting operator, so we can use ProxyCellAccessor as T.
 * @throws std::out_of_range Thrown if number of accessed dimensions not equal to Dimension
 */
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

/**
 * @throws std::out_of_range Thrown if number of accessed dimensions not equal to Dimension
 */
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

/**
 * @brief Allows access to matrix
 * @details Implemented via recursion and populating std::array with indexes
 * @throws std::out_of_range Thrown if number of accessed dimensions more than Dimensions
 */
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
