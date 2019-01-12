#include <gtest/gtest.h>
#include "bisection.hpp"

TEST(testMath, myTest)
{
    EXPECT_EQ(1, 1);
}

TEST(testMath, myTest2)
{
    EXPECT_NE(1, 2);
}

TEST(testMath, myTest3)
{
    EXPECT_EQ(3, 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}