#include "gtest/gtest.h"

TEST(version, basic_test) {
    ASSERT_GT(PATCH_VERSION, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
