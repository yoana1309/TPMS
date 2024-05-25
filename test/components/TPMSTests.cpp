#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C"
{
#include "C:\diplomna\TPMS\src\COM\COM.h"
#include "C:\diplomna\TPMS\src\MCUWakeUp\MCUWakeUp.h"
#include "C:\diplomna\TPMS\src\Sensors\Sensors.h"
#include "C:\diplomna\TPMS\src\Timer\Timer.h"
#include "C:\diplomna\TPMS\src\TPMS\TPMS.h"
}

#define PRETEST()                         \
    ASSERT_EQ(MockPtr, nullptr); \
    MockPtr = new Mock; \

#define POSTTEST()           \
    delete MockPtr; \

using ::testing::_;
using ::testing::Args;
using ::testing::ElementsAre;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::InSequence;
using ::testing::DoAll;
using ::testing::SetArgPointee;

class Mock
{
public:
    virtual ~Mock() {}

    /* mock api */
    MOCK_METHOD( returnType, COM_WarnDiag, ( uint8 tireIndex, TPMSWarningType warning ) );
    MOCK_METHOD( returnType, COM_RequestLEDOn, ( TPMSWarningType warning ) );    
    MOCK_METHOD( void, COM_SignalError, ( COMErrorType error ) );

    MOCK_METHOD( void, MCUWakeUp_SignalError, ( MCUWakeUpErrorType error ) );

    MOCK_METHOD( returnType, Sensors_ReadSignal, ( uint8 index, SensorsSignalType signalType, uint16* reading ) );
    MOCK_METHOD( void, Sensors_SignalError, ( uint8 index, SensorsErrorType error ) );
    
    MOCK_METHOD( returnType, Timer_Init, () );
    MOCK_METHOD( returnType, Timer_SetTimer, ( Timer_TimerType *timer, const uint16 valueMs ) );
    MOCK_METHOD( returnType, Timer_CheckTimer, ( Timer_TimerType *timer, Timer_TimerStateType *state ) );
    MOCK_METHOD( returnType, Timer_Tick, () );

    MOCK_METHOD( void, TPMS_SignalError, ( uint8 index, TPMSErrorType error ) );
};

Mock *MockPtr = nullptr;

extern "C"
{
    returnType COM_WarnDiag( uint8 tireIndex, TPMSWarningType warning )
    {
        return MockPtr->COM_WarnDiag( tireIndex, warning );
    }

    returnType COM_RequestLEDOn( TPMSWarningType warning )
    {
        return MockPtr->COM_RequestLEDOn( warning );
    }

    void COM_SignalError( COMErrorType error )
    {
        MockPtr->COM_SignalError( error );
    }

    void MCUWakeUp_SignalError( MCUWakeUpErrorType error )
    {
        MockPtr->MCUWakeUp_SignalError( error );
    }

    returnType Sensors_ReadSignal( uint8 index, SensorsSignalType signalType, uint16* reading )
    {
        return MockPtr->Sensors_ReadSignal( index, signalType, reading );
    }

    void Sensors_SignalError( uint8 index, SensorsErrorType error )
    {
        MockPtr->Sensors_SignalError( index, error );
    }

    returnType Timer_Init()
    {
        return MockPtr->Timer_Init();
    }

    returnType Timer_SetTimer( Timer_TimerType *timer, const uint16 valueMs )
    {
        return MockPtr->Timer_SetTimer( timer, valueMs );
    }

    returnType Timer_CheckTimer( Timer_TimerType *timer, Timer_TimerStateType *state )
    {
        return MockPtr->Timer_CheckTimer( timer, state );
    }

    returnType Timer_Tick()
    {
        return MockPtr->Timer_Tick();
    }

    void TPMS_SignalError( uint8 index, TPMSErrorType error )
    {
        MockPtr->TPMS_SignalError( index, error );
    }
}

TEST(TPMS, Test_Demo)
{
    returnType ret = E_OK;
    
    EXPECT_EQ(ret, E_NOT_OK);
}

TEST(TPMS, Test_COM_TriggerWarning_Negative)
{
    PRETEST();

    returnType ret = E_NOT_OK;
    EXPECT_EQ(ret, E_OK);

    POSTTEST();
}