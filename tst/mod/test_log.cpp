
#include <limits>

#include <gtest/gtest.h>
#include <AeroKernel/log.hpp>

TEST( LogTest, pass )
{
    EXPECT_EQ( true, true );
}

TEST( LogTest, pass2 )
{
    EXPECT_EQ( AeroKernel::someLogThing(std::numeric_limits<uint32_t>::max()), 200 );
    //EXPECT_EQ( AeroKerne)
}