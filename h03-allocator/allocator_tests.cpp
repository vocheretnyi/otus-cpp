#include <gtest/gtest.h>
#include <map>

#include "allocator.h"
#include "container.h"

template<size_t AllocChunkSize>
void MapWithAllocatorTestHelper() {
    std::map<int, int, std::less<>, MyAllocator<std::pair<const int, int>, AllocChunkSize> > m;
    ASSERT_EQ(m.size(), 0);

    int factorial = 1;
    for (int i = 0; i <= 9; ++i) {
        m[i] = factorial;
        factorial *= (i + 1);
        ASSERT_EQ(m.size(), i + 1);
    }

    factorial = 1;
    int key = 0;
    for (const auto&[keyMap, valueMap] : m) {
        ASSERT_EQ(keyMap, key);
        ASSERT_EQ(valueMap, factorial);
        key++;
        factorial *= key;
    }
}

TEST(Allocator, MapWithAllocator) {
    MapWithAllocatorTestHelper<3>();
    MapWithAllocatorTestHelper<1>();
    MapWithAllocatorTestHelper<10>();
}

TEST(Container, BasicTest) {
    MyList<int> list;

    ASSERT_EQ(list.size(), 0);
    ASSERT_EQ(list.empty(), true);
    ASSERT_TRUE(list.begin() == nullptr);
    ASSERT_TRUE(list.end() == nullptr);

    list.push(1);
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list.empty(), false);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 1);
    ASSERT_TRUE(list.begin() != nullptr);
    ASSERT_TRUE(list.end() == nullptr);

    list.push(2);
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.empty(), false);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 2);
    ASSERT_TRUE(list.begin() != nullptr);
    ASSERT_TRUE(list.end() == nullptr);
}

TEST(Container, TestIterators) {
    MyList<int> list;
    list.push(1);
    list.push(2);
    list.push(3);
    ASSERT_TRUE(list.begin() != nullptr);
    ASSERT_TRUE(list.end() == nullptr);

    {
        int val = 1;
        for (int x : list) {
            ASSERT_EQ(x, val);
            ++val;
        }
    }

    {
        int val = 1;
        for (auto it = list.begin(); it != list.end(); ++it, ++val) {
            ASSERT_EQ(*it, val);
        }
    }
}

TEST(Container, ContainerWithCustomAllocator) {
    constexpr size_t N = 100000;
    MyList<int, MyAllocator<int, 150>> list;
    for (size_t i = 0; i < N; ++i) {
        list.push(i);
    }
    ASSERT_EQ(list.size(), N);

    size_t counter = 0;
    for (const auto& val : list) {
        ASSERT_EQ(val, counter);
        ++counter;
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
