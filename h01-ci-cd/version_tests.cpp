#include "gtest/gtest.h"

#include <iostream>

TEST(version, basic_test) {
    ASSERT_TRUE(PATCH_VERSION > 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}