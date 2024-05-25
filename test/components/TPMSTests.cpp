#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C"
{
#include "C:\diplomna\TPMS\src\TPMS\TPMS.h"
}

using ::testing::_;
using ::testing::Args;
using ::testing::ElementsAre;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::InSequence;
using ::testing::DoAll;
using ::testing::SetArgPointee;

class TPMSMock
{
public:
    virtual ~TPMSMock() {}

    /* mock api */
    MOCK_METHOD( void, TPMS_SignalError, ( uint8 index, TPMSErrorType error ) );
};

TPMSMock *TPMSMockPtr = nullptr;

extern "C"
{
    void TPMS_SignalError( uint8 index, TPMSErrorType error )
    {
        TPMSMockPtr->TPMS_SignalError( index, error );
    }
}

TEST(TPMS, DummyPositive)
{
    returnType ret = E_OK;
    
    EXPECT_EQ(ret, E_OK);
}