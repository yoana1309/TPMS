#include <gtest/gtest.h>

extern "C"
{
#include "C:\diplomna\TPMS\src\TPMS\TPMS.h"
}

TEST(TPMS, DummyPositive)
{
    returnType ret = E_OK;
    
    EXPECT_EQ(ret, E_OK);
}