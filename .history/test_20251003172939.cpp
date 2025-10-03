/*#include <iostream>
#include <gtest/gtest.h>

using namespace std;

int main(int argc, char **arvg)
{
    testing::InitGoogleTest(&argc,arvg);

    return RUN_ALL_TESTS();
}
*/
#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

TEST(AdditionTest, BasicAssertions) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_NE(add(1, 1), 2);
}
TEST(lol, lol2)
{
    EXPECT_EQ(add(2,8),10);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
