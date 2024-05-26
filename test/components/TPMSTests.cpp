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

TEST(Demo, Test_Demo)
{
    returnType ret;
}

TEST(COM, Test_TriggerWarning_Negative)
{
    returnType ret;
    uint8 tireIndex = TPMS_Tire_FrontLeft; //initialize
    TPMSWarningType warning = TPMS_WARNING_LOW_PRESSURE; //initialize 

    PRETEST();

    {
        InSequence s;

        //Case 1: COM_RequestLEDOn returning E_NOT_OK
        EXPECT_CALL(*MockPtr, COM_RequestLEDOn(warning)).WillOnce(Return(E_NOT_OK));
        EXPECT_CALL(*MockPtr, COM_SignalError(COM_ERR_LED_ON_REQUEST)).WillOnce(Return());

        //Case 2: COM_WarnDiag returning E_NOT_OK
        EXPECT_CALL(*MockPtr, COM_RequestLEDOn(warning)).WillOnce(Return(E_OK));
        EXPECT_CALL(*MockPtr, COM_WarnDiag(tireIndex, warning)).WillOnce(Return(E_NOT_OK));
        EXPECT_CALL(*MockPtr, COM_SignalError(COM_ERR_DIAG_WARNING)).WillOnce(Return());
    }

    //Case 1: COM_RequestLEDOn returning E_NOT_OK
    ret = COM_TriggerWarning( tireIndex, warning );
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 2: COM_WarnDiag returning E_NOT_OK
    ret = COM_TriggerWarning( tireIndex, warning );
    EXPECT_EQ(ret, E_NOT_OK);

    POSTTEST();
}

TEST(COM, Test_TriggerWarning_Positive)
{
    returnType ret;
    uint8 tireIndex = TPMS_Tire_FrontLeft;
    TPMSWarningType warning = TPMS_WARNING_LOW_PRESSURE;

    PRETEST();

    {
        InSequence s;

        EXPECT_CALL(*MockPtr, COM_RequestLEDOn(warning)).WillOnce(Return(E_OK));
        EXPECT_CALL(*MockPtr, COM_WarnDiag(tireIndex, warning)).WillOnce(Return(E_OK));
    }

    ret = COM_TriggerWarning( tireIndex, warning );
    EXPECT_EQ(ret, E_OK);

    POSTTEST();
}

TEST(MCUWakeUp, Test_Init_Positive)
{
    PRETEST();

    MCUWakeUp_Init();
    EXPECT_EQ(MCUWakeUpData.Initialized, MCU_WAKE_UP_INITIALIZED);
    EXPECT_EQ(MCUWakeUpData.State, MCUWakeUp_State_Undefined);
    EXPECT_EQ(MCUWakeUpData.RequestLowPower, FALSE);

    POSTTEST();
}

TEST(MCUWakeUp, Test_Uninitialized)
{
    returnType ret;
    MCUWakeUpStatesType state;

    PRETEST();

    //Running API functions without initializing the component

    ret = MCUWakeUp_GetState( &state );
    EXPECT_EQ(ret, E_NOT_OK);

    ret = MCUWakeUp_RequestLowPower();
    EXPECT_EQ(ret, E_NOT_OK);

    POSTTEST();
}

TEST(MCUWakeUp, Test_Run_Negative)
{
    returnType ret;

    PRETEST();

    {
        InSequence s;
        EXPECT_CALL(*MockPtr, MCUWakeUp_SignalError(MCUWAKEUP_ERR_UNHANDLED_STATE));
    }

    //Case: Unhandled state
    MCUWakeUp_Init();
    MCUWakeUpData.State = MCUWakeUp_State_Count;
    MCUWakeUp_Run();

    POSTTEST();
}

TEST(MCUWakeUp, Test_Run_Positive)
{
    returnType ret;

    PRETEST();

    {
        InSequence s;
        EXPECT_CALL(*MockPtr, Timer_SetTimer(_, 500)).WillOnce(Return(E_OK));
        EXPECT_CALL(*MockPtr, Timer_CheckTimer(_,_)).WillOnce(DoAll(SetArgPointee<1>(Timer_Running),Return(E_OK)));
        EXPECT_CALL(*MockPtr, Timer_CheckTimer(_,_)).WillOnce(DoAll(SetArgPointee<1>(Timer_Expired),Return(E_OK)));
        EXPECT_CALL(*MockPtr, Timer_SetTimer(_, 500)).WillOnce(Return(E_OK));
    }

    MCUWakeUp_Init(); // MCUWakeUp_State_Undefined
    EXPECT_EQ(MCUWakeUpData.State, MCUWakeUp_State_Undefined);
    MCUWakeUp_Run(); // MCUWakeUp_State_Undefined -> MCUWakeUp_State_Active
    EXPECT_EQ(MCUWakeUpData.State, MCUWakeUp_State_Active);
    MCUWakeUp_Run(); // MCUWakeUp_State_Active -> MCUWakeUp_State_Active (Timer still running)
    EXPECT_EQ(MCUWakeUpData.State, MCUWakeUp_State_Active);
    ret = MCUWakeUp_RequestLowPower();
    EXPECT_EQ(ret, E_OK);
    MCUWakeUp_Run(); // MCUWakeUp_State_Active -> MCUWakeUp_State_LowPower
    EXPECT_EQ(MCUWakeUpData.State, MCUWakeUp_State_LowPower);
    MCUWakeUp_Run(); // MCUWakeUp_State_LowPower -> MCUWakeUp_State_LowPower
    EXPECT_EQ(MCUWakeUpData.State, MCUWakeUp_State_LowPower);
    MCUWakeUp_Run(); // MCUWakeUp_State_LowPower -> MCUWakeUp_State_Active
    EXPECT_EQ(MCUWakeUpData.State, MCUWakeUp_State_Active);

    POSTTEST();
}

TEST(MCUWakeUp, Test_GetState_Negative)
{
    returnType ret;
    MCUWakeUpStatesType state;

    PRETEST();

    {
        InSequence s;
        EXPECT_CALL(*MockPtr, MCUWakeUp_SignalError(MCUWAKEUP_ERR_UNINITIALIZED));
    }

    //Case 1: Invalid initialization flag
    MCUWakeUp_Init();
    MCUWakeUpData.Initialized = 0;
    ret = MCUWakeUp_GetState( &state );
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 2: Safe state???
    MCUWakeUp_Init();
    MCUWakeUpData.State = MCUWakeUp_State_SafeState;
    ret = MCUWakeUp_GetState( &state );
    EXPECT_EQ(ret, E_NOT_OK);

    POSTTEST();
}

TEST(MCUWakeUp, Test_GetState_Positive)
{
    returnType ret;
    MCUWakeUpStatesType state;

    PRETEST();

    MCUWakeUp_Init();
    ret = MCUWakeUp_GetState( &state );
    EXPECT_EQ(state, MCUWakeUp_State_Undefined);
    EXPECT_EQ(ret, E_OK);

    POSTTEST();
}

TEST(MCUWakeUp, Test_RequestLowPower_Negative)
{
    returnType ret;

    PRETEST();

    {
        InSequence s;
        EXPECT_CALL(*MockPtr, MCUWakeUp_SignalError(MCUWAKEUP_ERR_UNINITIALIZED));
    }

    //Case 1: Invalid initialization flag
    MCUWakeUp_Init();
    MCUWakeUpData.Initialized = 0;
    ret = MCUWakeUp_RequestLowPower();
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 2: Already in low power state
    MCUWakeUp_Init();
    MCUWakeUpData.State = MCUWakeUp_State_LowPower;
    ret = MCUWakeUp_RequestLowPower();
    EXPECT_EQ(ret, E_NOT_OK);

    POSTTEST();
}

TEST(MCUWakeUp, Test_RequestLowPower_Positive)
{
    returnType ret;

    PRETEST();

    MCUWakeUp_Init();
    ret = MCUWakeUp_RequestLowPower();
    EXPECT_EQ(MCUWakeUpData.RequestLowPower, TRUE);
    EXPECT_EQ(ret, E_OK);

    POSTTEST();
}

TEST(Sensors, Test_Init_Positive)
{
    uint8 index = 0;

    PRETEST();

    //Check if all sensors are initialized properly
    Sensors_Init();
    for( index; index < 4; index++ )
    {
        EXPECT_EQ(SensorsData[ index ].Initialized, SENSORS_INITIALIZED);
        EXPECT_EQ(SensorsData[ index ].tireID, index);
        EXPECT_EQ(SensorsData[ index ].signalType, Sensors_Signal_Pressure);
        EXPECT_EQ(SensorsData[ index ].reading, 0);
    }
    for( index; index < Sensors_Count; index++ )
    {
        EXPECT_EQ(SensorsData[ index ].Initialized, SENSORS_INITIALIZED);
        EXPECT_EQ(SensorsData[ index ].tireID, (index)%2);
        EXPECT_EQ(SensorsData[ index ].signalType, Sensors_Signal_Temperature);
        EXPECT_EQ(SensorsData[ index ].reading, 0);
    }

    POSTTEST();
}

TEST(Sensors, Test_Uninitialized)
{
    returnType ret;
    uint8 index = Sensors_Pressure_FL;
    SensorsSignalType signalType = Sensors_Signal_Pressure;
    uint16 reading;

    PRETEST();

    {
        InSequence s;
        EXPECT_CALL(*MockPtr, Sensors_SignalError(index, SENSORS_INVALID_INIT_FLAG_ERROR));
    }

    //Running API function without initializing the component
    ret = Sensors_GetSignal(index, signalType, &reading);
    EXPECT_EQ(ret, E_NOT_OK);
    
    POSTTEST();
}

TEST(Sensors, Test_Run_Negative)
{
    returnType ret;
    MCUWakeUpStatesType invalidWakeUpState = MCUWakeUp_State_Count;
    MCUWakeUpStatesType activeWakeUpState = MCUWakeUp_State_Active;

    PRETEST();

    {
        InSequence s;
        //Case 1: MCUWakeUp_GetState error
        EXPECT_CALL(*MockPtr, MCUWakeUp_SignalError(MCUWAKEUP_ERR_UNINITIALIZED));
        EXPECT_CALL(*MockPtr, Sensors_SignalError(Sensors_Count, SENSORS_MCUWAKEUP_GETSTATE_ERROR));
        //Case 2: wakeUpState is not active
        EXPECT_CALL(*MockPtr, Sensors_SignalError(Sensors_Count, SENSORS_MCUWAKEUP_GETSTATE_ERROR));
        //Case 3: TPMS_Run error
        EXPECT_CALL(*MockPtr, Sensors_SignalError(_, SENSORS_INVALID_INIT_FLAG_ERROR));
        EXPECT_CALL(*MockPtr, TPMS_SignalError(_, TPMS_ERR_PRESSURE_READING));
        EXPECT_CALL(*MockPtr, TPMS_SignalError(_, TPMS_ERR_RECEIVE_PRESSURE_SIGNAL));
        EXPECT_CALL(*MockPtr, Sensors_SignalError(Sensors_Count, SENSORS_TPMS_RUN_ERROR));
        //Case 4: MCUWakeUp_RequestLowPower error
        EXPECT_CALL(*MockPtr, Sensors_ReadSignal(_, _ , _)).Times(8).WillOnce(DoAll(SetArgPointee<2>(25),Return(E_OK)));
        EXPECT_CALL(*MockPtr, MCUWakeUp_SignalError(MCUWAKEUP_ERR_UNINITIALIZED));
        EXPECT_CALL(*MockPtr, Sensors_SignalError(Sensors_Count, SENSORS_MCUWAKEUP_REQLOWPOWER_ERROR));
    }

    //Case 1: MCUWakeUp_GetState error: e.g. uninitialized
    MCUWakeUp_Init();
    MCUWakeUpData.Initialized = 0;
    ret = Sensors_Run();
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 2: wakeUpState is not active
    MCUWakeUp_Init();
    MCUWakeUpData.State = invalidWakeUpState;
    ret = Sensors_Run();
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 3: TPMS_Run error: e.g. sensors not initialized
    MCUWakeUpData.State = activeWakeUpState;
    ret = Sensors_Run();
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 4: MCUWakeUp_RequestLowPower error: e.g. MCUWakeUp uninitialized
    Sensors_Init();
    TPMS_Init();
    MCUWakeUpData.State = activeWakeUpState;
    MCUWakeUpData.Initialized = 0;
    ret = Sensors_Run();
    EXPECT_EQ(ret, E_NOT_OK);
    
    POSTTEST();
}

// TEST(Sensors, Test_Run_Positive)
// {
//     returnType ret;
//     MCUWakeUpStatesType activeWakeUpState = MCUWakeUp_State_Active;

//     PRETEST();

//     {
//         InSequence s;
//         EXPECT_CALL(*MockPtr, MCUWakeUp_GetState(_)).WillOnce(DoAll(SetArgPointee<0>(activeWakeUpState),Return(E_OK)));
//         EXPECT_CALL(*MockPtr, TPMS_Run).WillOnce(Return(E_OK));
//         EXPECT_CALL(*MockPtr, MCUWakeUp_RequestLowPower).WillOnce(Return(E_OK));
//     }

//     ret = Sensors_Run();
//     EXPECT_EQ(ret, E_OK);
    
//     POSTTEST();
// }

TEST(Sensors, Test_GetSignal_Negative)
{
    returnType ret;
    uint8 index = Sensors_Pressure_FL;
    SensorsSignalType signalType = Sensors_Signal_Pressure;
    uint8 invalidIndex = Sensors_Count;
    SensorsSignalType invalidSignalType = Sensors_Signal_Count;
    uint16 reading;

    PRETEST();

    {
        InSequence s;
        //Case 1: invalid index; Case 2: invalid signal type
        EXPECT_CALL(*MockPtr, Sensors_SignalError(Sensors_Count, SENSORS_INVALID_INPUT_ERROR)).Times(2);
        //Case 3: invalid initialization flag
        EXPECT_CALL(*MockPtr, Sensors_SignalError(index, SENSORS_INVALID_INIT_FLAG_ERROR));
        //Case 4: ReadSignal error
        EXPECT_CALL(*MockPtr, Sensors_ReadSignal(index, signalType, &reading)).WillOnce(Return(E_NOT_OK));
        EXPECT_CALL(*MockPtr, Sensors_SignalError(index, SENSORS_READ_SIGNAL_ERROR));
        //Case 5: Invalid reading
        EXPECT_CALL(*MockPtr, Sensors_ReadSignal(index, signalType, _)).WillOnce(DoAll(SetArgPointee<2>(2000),Return(E_OK)));
        EXPECT_CALL(*MockPtr, Sensors_SignalError(index, SENSORS_INVALID_READING_ERROR));
    }

    //Case 1: invalid index
    Sensors_Init();
    ret = Sensors_GetSignal( invalidIndex, signalType, &reading);
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 2: invalid signal type
    ret = Sensors_GetSignal( index, invalidSignalType, &reading);
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 3: invalid initialization flag
    SensorsData[ index ].Initialized = 0;
    ret = Sensors_GetSignal( index, signalType, &reading);
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 4: ReadSignal error
    Sensors_Init();
    ret = Sensors_GetSignal( index, signalType, &reading);
    EXPECT_EQ(ret, E_NOT_OK);

    //Case 5: Invalid reading
    ret = Sensors_GetSignal( index, signalType, &reading);
    EXPECT_EQ(ret, E_NOT_OK);
    
    POSTTEST();
}

TEST(Sensors, Test_GetSignal_Positive)
{
    returnType ret;
    uint8 index = Sensors_Pressure_FL;
    SensorsSignalType signalType = Sensors_Signal_Pressure;
    uint16 reading;

    PRETEST();

    {
        InSequence s;
        EXPECT_CALL(*MockPtr, Sensors_ReadSignal(index, signalType, _)).WillOnce(DoAll(SetArgPointee<2>(500),Return(E_OK)));
    }

    Sensors_Init();
    ret = Sensors_GetSignal( index, signalType, &reading);
    EXPECT_EQ(ret, E_OK);
    
    POSTTEST();
}
