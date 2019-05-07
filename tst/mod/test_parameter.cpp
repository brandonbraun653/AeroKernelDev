#include <gtest/gtest.h>

TEST(Test1, pass)
{
    EXPECT_EQ( true, true );
}

TEST( Test1, fail )
{
    EXPECT_EQ( true, true );
}