#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C"
{
#include "C:\diplomna\TPMS\src\common\Types.h"
#include "C:\diplomna\TPMS\src\E2E\E2E.h"
#include "C:\diplomna\TPMS\src\MessageReceiver\MessageReceiver.h"
#include "C:\diplomna\TPMS\src\MessageSender\MessageSender.h"
#include "C:\diplomna\TPMS\src\Monitor\Monitor.h"
#include "C:\diplomna\TPMS\src\SignalProcessor\SignalProcessor.h"
}

#define PRETEST()                         \
    ASSERT_EQ(MockPtr, nullptr); \
    MockPtr = new Mock; \

#define POSTTEST()           \
    delete MockPtr; \

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::DoAll;
using ::testing::SetArgPointee;

class Mock
{
public:
    virtual ~Mock() {}

    /* mock api */
    MOCK_METHOD( returnType, E2E_P01Protect, ( const E2E_P01ConfigType* ConfigPtr, E2E_P01ProtectStateType* StatePtr, uint8* DataPtr ) );
    MOCK_METHOD( returnType, E2E_P01Check, ( const E2E_P01ConfigType* Config, E2E_P01CheckStateType* State, const uint8* Data ) );

    MOCK_METHOD( returnType, MessageReceiver_ReadMessage, ( uint8* msg ) );
    MOCK_METHOD( returnType, MessageReceiver_AlertTPMSOK, ( uint8 tireID ) );
    MOCK_METHOD( returnType, MessageReceiver_AlertTPMSWarning, ( uint8 tireID ) );
    MOCK_METHOD( returnType, MessageReceiver_AlertTPMSSystemError, ( uint8 tireID ) );

    MOCK_METHOD( returnType, MessageSender_SendMessage, () );
    
    MOCK_METHOD( returnType, Monitor_SaveErrors, ( MonitorErrorCodesType ErrorBuffer[ ERROR_BUFFER_LEN ] ) );
    
    MOCK_METHOD( returnType, SignalProcessor_ReadPressureSensorData, ( uint16* reading, uint8* id ) );
    MOCK_METHOD( returnType, SignalProcessor_ReadTemperatureSensorData, ( uint16* reading, uint8* id ) );
};

Mock *MockPtr = nullptr;

extern "C"
{
    returnType E2E_P01Protect( const E2E_P01ConfigType* ConfigPtr, E2E_P01ProtectStateType* StatePtr, uint8* DataPtr )
    {
        return MockPtr->E2E_P01Protect( ConfigPtr, StatePtr, DataPtr );
    }

    returnType E2E_P01Check( const E2E_P01ConfigType* Config, E2E_P01CheckStateType* State, const uint8* Data )
    {
        return MockPtr->E2E_P01Check( Config, State, Data );
    }

    returnType MessageReceiver_ReadMessage( uint8* msg )
    {
        return MockPtr->MessageReceiver_ReadMessage( msg );
    }

    returnType MessageReceiver_AlertTPMSOK( uint8 tireID )
    {
        return MockPtr->MessageReceiver_AlertTPMSOK( tireID );
    }

    returnType MessageReceiver_AlertTPMSWarning( uint8 tireID )
    {
        return MockPtr->MessageReceiver_AlertTPMSWarning( tireID );
    }

    returnType MessageReceiver_AlertTPMSSystemError( uint8 tireID )
    {
        return MockPtr->MessageReceiver_AlertTPMSSystemError( tireID );
    }

    returnType MessageSender_SendMessage()
    {
        return MockPtr->MessageSender_SendMessage();
    }

    returnType Monitor_SaveErrors( MonitorErrorCodesType ErrorBuffer[ ERROR_BUFFER_LEN ] )
    {
        return MockPtr->Monitor_SaveErrors( ErrorBuffer );
    }
    
    returnType SignalProcessor_ReadPressureSensorData( uint16* reading, uint8* id )
    {
        return MockPtr->SignalProcessor_ReadPressureSensorData( reading, id );
    }
    
    returnType SignalProcessor_ReadTemperatureSensorData( uint16* reading, uint8* id )
    {
        return MockPtr->SignalProcessor_ReadTemperatureSensorData( reading, id );
    }
}

TEST(MessageReceiver, Test_Init_Positive)
{
    PRETEST();

    MessageReceiver_Init();
    EXPECT_EQ(MessageReceiverData.Initialized, MESSAGE_RECEIVER_INITIALIZED);
    EXPECT_EQ(MessageReceiverData.TireID, 0);
    EXPECT_EQ(MessageReceiverData.MessageType, 0);
    EXPECT_EQ(MessageReceiverData.Data, 0);

    POSTTEST();
}

TEST(MessageReceiver, Test_Run_Negative)
{
    uint8 invalidInitFlag = 0;

    PRETEST();

    {
        InSequence s;

        //MessageReceiver is uninitialized
        EXPECT_CALL(*MockPtr, MessageReceiver_AlertTPMSSystemError).WillOnce(Return(E_OK));
        //MessageReceiver_ReadMessage returns E_NOT_OK
        EXPECT_CALL(*MockPtr, MessageReceiver_ReadMessage).WillOnce(Return(E_NOT_OK));
        EXPECT_CALL(*MockPtr, MessageReceiver_AlertTPMSSystemError).WillOnce(Return(E_OK));
        //MessageSender_CheckMessage returns E_NOT_OK
        EXPECT_CALL(*MockPtr, MessageReceiver_ReadMessage).WillOnce(Return(E_OK));
        EXPECT_CALL(*MockPtr, E2E_P01Check).WillOnce(Return(E_NOT_OK));
        EXPECT_CALL(*MockPtr, MessageReceiver_AlertTPMSSystemError).WillOnce(Return(E_OK));
    }

    MessageReceiver_Init();

    //MessageReceiver is uninitialized
    MessageReceiverData.Initialized = invalidInitFlag;
    MessageReceiver_Run();

    //MessageReceiver_ReadMessage returns E_NOT_OK
    MessageReceiver_Init();
    MessageReceiver_Run();

    //MessageSender_CheckMessage returns E_NOT_OK
    MessageReceiver_Run();

    POSTTEST();
}

TEST(MessageReceiver, Test_Run_Positive)
{
    uint8 OKdata = 32; //00100000 -> tire ID = 2; msg type - ok
    uint8 TPMSWarningData = 33; //00100001 -> tire ID = 2; msg type - tpms warning
    uint8 SysErrData = 34; //00100010 -> tire ID = 2; msg type - system error

    PRETEST();

    {
        InSequence s;

        EXPECT_CALL(*MockPtr, MessageReceiver_ReadMessage( &MessageReceiverData.Data )).WillOnce(DoAll(SetArgPointee<0>(OKdata),Return(E_OK)));
        EXPECT_CALL(*MockPtr, E2E_P01Check).WillOnce(Return(E_OK));
        EXPECT_CALL(*MockPtr, MessageReceiver_AlertTPMSOK).WillOnce(Return(E_OK));
        
        EXPECT_CALL(*MockPtr, MessageReceiver_ReadMessage( &MessageReceiverData.Data )).WillOnce(DoAll(SetArgPointee<0>(TPMSWarningData),Return(E_OK)));
        EXPECT_CALL(*MockPtr, E2E_P01Check).WillOnce(Return(E_OK));
        EXPECT_CALL(*MockPtr, MessageReceiver_AlertTPMSWarning).WillOnce(Return(E_OK));
        
        EXPECT_CALL(*MockPtr, MessageReceiver_ReadMessage( &MessageReceiverData.Data )).WillOnce(DoAll(SetArgPointee<0>(SysErrData),Return(E_OK)));
        EXPECT_CALL(*MockPtr, E2E_P01Check).WillOnce(Return(E_OK));
        EXPECT_CALL(*MockPtr, MessageReceiver_AlertTPMSSystemError).WillOnce(Return(E_OK));        
    }

    MessageReceiver_Init();
    MessageReceiver_Run();
    EXPECT_EQ(MessageReceiverData.TireID, 2);
    EXPECT_EQ(MessageReceiverData.MessageType, 0); //TPMS OK 
    
    MessageReceiver_Run();
    EXPECT_EQ(MessageReceiverData.TireID, 2);
    EXPECT_EQ(MessageReceiverData.MessageType, 1); //TPMS Warning
    
    MessageReceiver_Run();
    EXPECT_EQ(MessageReceiverData.TireID, 2);
    EXPECT_EQ(MessageReceiverData.MessageType, 2); //TPMS System error

    POSTTEST();
}

TEST(MessageSender, Test_Uninitialized)
{
    returnType ret;
    uint8 errorCode = 0;
    MessageSenderErrorType err;

    PRETEST();

    ret = MessageSender_TriggerSafeState();
    EXPECT_EQ(ret, E_NOT_OK);
       
    ret = MessageSender_GetError( &err );
    EXPECT_EQ(ret, E_NOT_OK); 

    POSTTEST();
}

TEST(MessageSender, Test_Init_Positive)
{
    PRETEST();

    MessageSender_Init();
    EXPECT_EQ(MessageSenderData.Initialized, MESSAGE_SENDER_INITIALIZED);
    EXPECT_EQ(MessageSenderData.TireID, 0);
    EXPECT_EQ(MessageSenderData.MessageType, MessageSender_Message_OK);
    EXPECT_EQ(MessageSenderData.Data, 0);
    EXPECT_EQ(MessageSenderData.SafeState, FALSE);
    EXPECT_EQ(MessageSenderData.Error, MESSAGE_SENDER_NO_ERROR);

    POSTTEST();
}

TEST(MessageSender, Test_Run_Negative)
{
    uint8 invalidInitFlag = 0;
    uint8 signalProcTireId = 1;

    PRETEST();

    {
        InSequence s;

        //MessageSender_ProtectMessage returns E_NOT_OK
        EXPECT_CALL(*MockPtr, E2E_P01Protect( _,_,_ )).WillOnce(Return(E_NOT_OK));
        //MessageSender_SendMessage returns E_NOT_OK
        EXPECT_CALL(*MockPtr, E2E_P01Protect( _,_,_ )).WillOnce(Return(E_OK));
        EXPECT_CALL(*MockPtr, MessageSender_SendMessage).WillOnce(Return(E_NOT_OK));
    }

    MessageSender_Init();
    SignalProcessor_Init();

    //MessageSender not initialized
    MessageSenderData.Initialized = invalidInitFlag;
    MessageSender_Run();
    EXPECT_EQ(MessageSenderData.Error, MESSAGE_SENDER_ERR_UNINITIALIZED);

    //SignalProcessor_GetOutputMessage returns E_NOT_OK
    MessageSender_Init();
    SignalProcessorData.Initialized = invalidInitFlag;
    MessageSender_Run();
    EXPECT_EQ(MessageSenderData.TireID, 0);
    EXPECT_EQ(MessageSenderData.Data, 0);
    EXPECT_EQ(MessageSenderData.Error, MESSAGE_SENDER_ERR_GET_SIGNAL_PROCESSOR_OUTPUT_ERROR);

    //MessageSender_ProtectMessage returns E_NOT_OK
    SignalProcessor_Init();
    SignalProcessorData.OutputMessage = SIGNAL_PROCESSOR_TPMS_WARNING;
    SignalProcessorData.TireID = signalProcTireId;
    MessageSender_Run();
    EXPECT_EQ(MessageSenderData.MessageType, MessageSender_Message_TPMSWarning);
    EXPECT_EQ(MessageSenderData.TireID, signalProcTireId);
    EXPECT_EQ(MessageSenderData.Error, MESSAGE_SENDER_ERR_E2E_PROTECTION_ERROR);

    //MessageSender_SendMessage returns E_NOT_OK
    MessageSender_Run();
    EXPECT_EQ(MessageSenderData.MessageType, MessageSender_Message_TPMSWarning);
    EXPECT_EQ(MessageSenderData.TireID, signalProcTireId);
    EXPECT_EQ(MessageSenderData.Error, MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR);

    POSTTEST();
}

TEST(MessageSender, Test_Run_Positive)
{
    uint8 signalProcTireId = 1;

    PRETEST();

    ON_CALL(*MockPtr, E2E_P01Protect).WillByDefault(Return(E_OK));
    ON_CALL(*MockPtr, MessageSender_SendMessage).WillByDefault(Return(E_OK));

    //MessageSender_Message_OK
    MessageSender_Init();
    SignalProcessor_Init();
    SignalProcessorData.OutputMessage = SIGNAL_PROCESSOR_TPMS_OK;
    SignalProcessorData.TireID = signalProcTireId;
    MessageSender_Run();
    EXPECT_EQ(MessageSenderData.MessageType, MessageSender_Message_OK);
    EXPECT_EQ(MessageSenderData.TireID, signalProcTireId);
    EXPECT_EQ(MessageSenderData.Data, 16);
    EXPECT_EQ(MessageSenderData.Error, MESSAGE_SENDER_NO_ERROR);

    //MessageSender_Message_TPMSWarning
    MessageSender_Init();
    SignalProcessor_Init();
    SignalProcessorData.OutputMessage = SIGNAL_PROCESSOR_TPMS_WARNING;
    SignalProcessorData.TireID = signalProcTireId;
    MessageSender_Run();
    EXPECT_EQ(MessageSenderData.MessageType, MessageSender_Message_TPMSWarning);
    EXPECT_EQ(MessageSenderData.TireID, signalProcTireId);
    EXPECT_EQ(MessageSenderData.Data, 17);
    EXPECT_EQ(MessageSenderData.Error, MESSAGE_SENDER_NO_ERROR);

    //MessageSender_Message_SystemError
    MessageSender_Init();
    SignalProcessor_Init();
    SignalProcessorData.OutputMessage = SIGNAL_PROCESSOR_TPMS_WARNING;
    SignalProcessorData.TireID = signalProcTireId;
    MessageSenderData.SafeState = TRUE;
    MessageSender_Run();
    EXPECT_EQ(MessageSenderData.MessageType, MessageSender_Message_SystemError);
    EXPECT_EQ(MessageSenderData.TireID, signalProcTireId);
    EXPECT_EQ(MessageSenderData.Data, 18);
    EXPECT_EQ(MessageSenderData.Error, MESSAGE_SENDER_NO_ERROR);

    POSTTEST();
}

TEST(MessageSender, Test_TriggerSafeState_Negative)
{
    returnType ret;
    uint8 invalidInitFlag = 0;

    PRETEST();

    MessageSender_Init();
    MessageSenderData.Initialized = invalidInitFlag;
    ret = MessageSender_TriggerSafeState();
    EXPECT_EQ(ret, E_NOT_OK);
    EXPECT_EQ(MessageSenderData.SafeState, FALSE);

    POSTTEST();
}

TEST(MessageSender, Test_TriggerSafeState_Positive)
{
    returnType ret;

    PRETEST();

    MessageSender_Init();
    ret = MessageSender_TriggerSafeState();
    EXPECT_EQ(ret, E_OK);
    EXPECT_EQ(MessageSenderData.SafeState, TRUE);

    POSTTEST();
}

TEST(MessageSender, Test_GetError_Negative)
{
    returnType ret;
    MessageSenderErrorType err = MESSAGE_SENDER_NO_ERROR;
    uint8 invalidInitFlag = 0;

    PRETEST();

    MessageSender_Init();
    MessageSenderData.Initialized = invalidInitFlag;
    MessageSenderData.Error = MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR;
    ret = MessageSender_GetError( &err );
    EXPECT_EQ(ret, E_NOT_OK);
    EXPECT_EQ(err, MESSAGE_SENDER_NO_ERROR);

    POSTTEST();
}

TEST(MessageSender, Test_GetError_Positive)
{
    returnType ret;
    MessageSenderErrorType err;

    PRETEST();

    MessageSender_Init();
    MessageSenderData.Error = MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR;
    ret = MessageSender_GetError( &err );
    EXPECT_EQ(ret, E_OK);
    EXPECT_EQ(err, MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR);

    POSTTEST();
}

TEST(Monitor, Test_Init_Positive)
{
    uint8 index = 0;
    PRETEST();

    Monitor_Init();
    for( index; index < ERROR_BUFFER_LEN; index++ )
    {
        EXPECT_EQ(MonitorData.ErrorBuffer[ index ], 0);
    }

    POSTTEST();
}

TEST(Monitor, Test_Run_Negative)
{
    uint8 invalidInitFlag = 0;

    PRETEST();

    {
        InSequence s;

        EXPECT_CALL(*MockPtr, Monitor_SaveErrors( MonitorData.ErrorBuffer )).Times(2).WillOnce(Return(E_OK));

        //Monitor_SaveErrors returns E_NOT_OK
        EXPECT_CALL(*MockPtr, Monitor_SaveErrors( MonitorData.ErrorBuffer )).WillOnce(Return(E_NOT_OK));
    
        EXPECT_CALL(*MockPtr, Monitor_SaveErrors( MonitorData.ErrorBuffer )).WillOnce(Return(E_OK));
    }

    Monitor_Init();
    SignalProcessor_Init();
    MessageSender_Init();

    //SignalProcessor_GetError returns E_NOT_OK
    SignalProcessorData.Initialized = invalidInitFlag;
    SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_UNINITIALIZED;
    MessageSenderData.Error = MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR;
    Monitor_Run();
    EXPECT_EQ(MonitorData.ErrorBuffer[ SIGNAL_PROCESSOR_ERROR ], NO_ERRORS);
    EXPECT_EQ(MonitorData.ErrorBuffer[ MESSAGE_SENDER_ERROR ], NO_ERRORS);
    EXPECT_EQ(MessageSenderData.SafeState, FALSE);

    //MessageSender_GetError returns E_NOT_OK
    Monitor_Init();
    SignalProcessor_Init();
    MessageSenderData.Initialized = invalidInitFlag;
    SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_UNINITIALIZED;
    Monitor_Run();
    EXPECT_EQ(MonitorData.ErrorBuffer[ SIGNAL_PROCESSOR_ERROR ], SIGNAL_PROCESSOR_ERR_UNINITIALIZED);
    EXPECT_EQ(MonitorData.ErrorBuffer[ MESSAGE_SENDER_ERROR ], NO_ERRORS);
    EXPECT_EQ(MessageSenderData.SafeState, FALSE);

    //Monitor_SaveErrors returns E_NOT_OK
    Monitor_Init();
    MessageSender_Init();
    MessageSenderData.Error = MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR;
    Monitor_Run();
    EXPECT_EQ(MonitorData.ErrorBuffer[ SIGNAL_PROCESSOR_ERROR ], SIGNAL_PROCESSOR_ERR_UNINITIALIZED);
    EXPECT_EQ(MonitorData.ErrorBuffer[ MESSAGE_SENDER_ERROR ], MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR);
    EXPECT_EQ(MessageSenderData.SafeState, FALSE);

    //MessageSender_TriggerSafeState returns E_NOT_OK
    Monitor_Init();
    MessageSenderData.Initialized = invalidInitFlag;
    Monitor_Run();
    EXPECT_EQ(MessageSenderData.SafeState, FALSE);

    POSTTEST();
}

TEST(Monitor, Test_Run_Positive)
{
    PRETEST();

    {
        InSequence s;

        EXPECT_CALL(*MockPtr, Monitor_SaveErrors( MonitorData.ErrorBuffer )).WillOnce(Return(E_OK));
    }

    Monitor_Init();
    SignalProcessor_Init();
    MessageSender_Init();

    //SignalProcessor_GetError returns E_NOT_OK
    SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_UNINITIALIZED;
    MessageSenderData.Error = MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR;
    Monitor_Run();
    EXPECT_EQ(MonitorData.ErrorBuffer[ SIGNAL_PROCESSOR_ERROR ], SIGNAL_PROCESSOR_ERR_UNINITIALIZED);
    EXPECT_EQ(MonitorData.ErrorBuffer[ MESSAGE_SENDER_ERROR ], MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR);
    EXPECT_EQ(MessageSenderData.SafeState, TRUE);

    POSTTEST();
}

TEST(SignalProcessor, Test_Uninitialized)
{
    returnType ret;
    SignalProcessorOutputMessageType outputMsg;
    uint8 tireID;
    SignalProcessorErrorType err;

    PRETEST();

    ret = SignalProcessor_GetOutputMessage( &outputMsg, &tireID );
    EXPECT_EQ(ret, E_NOT_OK);

    ret = SignalProcessor_GetError( &err );
    EXPECT_EQ(ret, E_NOT_OK);

    POSTTEST();
}

TEST(SignalProcessor, Test_Init_Positive)
{
    PRETEST();

    SignalProcessor_Init();
    EXPECT_EQ(SignalProcessorData.Initialized, SIGNAL_PROCESSOR_INITIALIZED);
    EXPECT_EQ(SignalProcessorData.TireID, 0);
    EXPECT_EQ(SignalProcessorData.PressureReading, 0);
    EXPECT_EQ(SignalProcessorData.TemperatureReading, 0);
    EXPECT_EQ(SignalProcessorData.PressureKPa, 0);
    EXPECT_EQ(SignalProcessorData.TemperatureCelsius, 0);
    EXPECT_EQ(SignalProcessorData.OutputMessage, 0);
    EXPECT_EQ(SignalProcessorData.Error, SIGNAL_PROCESSOR_NO_ERROR);

    POSTTEST();
}

TEST(SignalProcessor, Test_Run_Negative)
{
    uint8 invalidInitFlag = 0;
    uint16 pressureReading = 512; //expected kPa = 300
    uint16 invalidPressureReading = 2000;
    uint8 tireId = 1;
    uint8 invalidTireId = 0;
    uint16 temperatureReading = 512; 
    uint16 invalidTemperatureReading;

    PRETEST();

    {
        InSequence s;

        //SignalProcessor_ReadPressureSensorData returns E_NOT_OK
        EXPECT_CALL(*MockPtr, SignalProcessor_ReadPressureSensorData( &SignalProcessorData.PressureReading, &SignalProcessorData.TireID )).WillOnce(Return(E_NOT_OK));
        //SignalProcessor_ValidateSensorData for pressure returns E_NOT_OK
        EXPECT_CALL(*MockPtr, SignalProcessor_ReadPressureSensorData( &SignalProcessorData.PressureReading, &SignalProcessorData.TireID )).WillOnce(DoAll(SetArgPointee<0>(invalidPressureReading),SetArgPointee<1>(invalidTireId),Return(E_OK)));
        //SignalProcessor_ReadTemperatureSensorData returns E_NOT_OK
        EXPECT_CALL(*MockPtr, SignalProcessor_ReadTemperatureSensorData( &SignalProcessorData.TemperatureReading, &SignalProcessorData.TireID )).WillOnce(Return(E_NOT_OK));
        //SignalProcessor_ValidateSensorData for temperature returns E_NOT_OK
        EXPECT_CALL(*MockPtr, SignalProcessor_ReadPressureSensorData( &SignalProcessorData.PressureReading, &SignalProcessorData.TireID )).WillOnce(DoAll(SetArgPointee<0>(pressureReading),SetArgPointee<1>(tireId),Return(E_OK)));
        EXPECT_CALL(*MockPtr, SignalProcessor_ReadTemperatureSensorData( &SignalProcessorData.TemperatureReading, &SignalProcessorData.TireID )).WillOnce(DoAll(SetArgPointee<0>(invalidTemperatureReading),SetArgPointee<1>(invalidTireId),Return(E_OK)));
    }

    SignalProcessor_Init();

    //SignalProcessor not initialized
    SignalProcessorData.Initialized = invalidInitFlag;
    SignalProcessor_Run();
    EXPECT_EQ(SignalProcessorData.Error, SIGNAL_PROCESSOR_ERR_UNINITIALIZED);

    //SignalProcessor_ReadPressureSensorData returns E_NOT_OK
    SignalProcessor_Init();
    SignalProcessor_Run();
    EXPECT_EQ(SignalProcessorData.Error, SIGNAL_PROCESSOR_ERR_READ_PRESSURE_ERROR);

    //SignalProcessor_ValidateSensorData for pressure returns E_NOT_OK and
    //SignalProcessor_ReadTemperatureSensorData returns E_NOT_OK
    SignalProcessor_Run();
    EXPECT_EQ(SignalProcessorData.PressureKPa, 0);
    EXPECT_EQ(SignalProcessorData.PressureReading, invalidPressureReading);
    EXPECT_EQ(SignalProcessorData.TireID, invalidTireId);
    EXPECT_EQ(SignalProcessorData.Error, SIGNAL_PROCESSOR_ERR_READ_TEMPERATURE_ERROR);
    
    //SignalProcessor_ValidateSensorData for temperature returns E_NOT_OK
    SignalProcessor_Run();
    EXPECT_EQ(SignalProcessorData.PressureKPa, 300);
    EXPECT_EQ(SignalProcessorData.PressureReading, pressureReading);
    EXPECT_EQ(SignalProcessorData.TemperatureCelsius, 0);
    EXPECT_EQ(SignalProcessorData.TemperatureReading, invalidTemperatureReading);
    EXPECT_EQ(SignalProcessorData.TireID, invalidTireId);
    EXPECT_EQ(SignalProcessorData.Error, SIGNAL_PROCESSOR_NO_ERROR);
    EXPECT_EQ(SignalProcessorData.OutputMessage, SIGNAL_PROCESSOR_TPMS_OK);

    POSTTEST();
}

TEST(SignalProcessor, Test_Run_Positive)
{
    uint16 pressureReading = 512; //expected kPa = 300
    uint8 tireId = 1;
    uint16 temperatureReading = 512; //expected celsius = 42

    PRETEST();    
    
    {
        InSequence s;

        EXPECT_CALL(*MockPtr, SignalProcessor_ReadPressureSensorData( &SignalProcessorData.PressureReading, &SignalProcessorData.TireID )).WillOnce(DoAll(SetArgPointee<0>(pressureReading),SetArgPointee<1>(tireId),Return(E_OK)));
        EXPECT_CALL(*MockPtr, SignalProcessor_ReadTemperatureSensorData( &SignalProcessorData.TemperatureReading, &SignalProcessorData.TireID )).WillOnce(DoAll(SetArgPointee<0>(temperatureReading),SetArgPointee<1>(tireId),Return(E_OK)));
    }

    SignalProcessor_Init();
    SignalProcessor_Run();
    EXPECT_EQ(SignalProcessorData.PressureKPa, 300);
    EXPECT_EQ(SignalProcessorData.PressureReading, pressureReading);
    EXPECT_EQ(SignalProcessorData.TemperatureCelsius, 42);
    EXPECT_EQ(SignalProcessorData.TemperatureReading, temperatureReading);
    EXPECT_EQ(SignalProcessorData.TireID, tireId);
    EXPECT_EQ(SignalProcessorData.Error, SIGNAL_PROCESSOR_NO_ERROR);
    EXPECT_EQ(SignalProcessorData.OutputMessage, SIGNAL_PROCESSOR_TPMS_OK);

    POSTTEST();
}

TEST(SignalProcessor, Test_GetOutputMessage_Negative)
{
    returnType ret;
    uint8 invalidInitFlag;
    SignalProcessorOutputMessageType outputMsg = SIGNAL_PROCESSOR_TPMS_OK;
    uint8 tireID = 0;

    PRETEST();

    SignalProcessor_Init();
    SignalProcessorData.Initialized = invalidInitFlag;
    SignalProcessorData.OutputMessage = SIGNAL_PROCESSOR_TPMS_WARNING;
    SignalProcessorData.TireID = 3;
    ret = SignalProcessor_GetOutputMessage( &outputMsg, &tireID);
    EXPECT_EQ(ret, E_NOT_OK);
    EXPECT_EQ(SignalProcessorData.Error, SIGNAL_PROCESSOR_ERR_UNINITIALIZED);
    EXPECT_EQ(outputMsg, SIGNAL_PROCESSOR_TPMS_OK);
    EXPECT_EQ(tireID, 0);

    POSTTEST();
}

TEST(SignalProcessor, Test_GetOutputMessage_Positive)
{
    returnType ret;
    SignalProcessorOutputMessageType outputMsg = SIGNAL_PROCESSOR_TPMS_OK;
    uint8 tireID = 0;

    PRETEST();

    SignalProcessor_Init();
    SignalProcessorData.OutputMessage = SIGNAL_PROCESSOR_TPMS_WARNING;
    SignalProcessorData.TireID = 3;
    ret = SignalProcessor_GetOutputMessage( &outputMsg, &tireID);
    EXPECT_EQ(ret, E_OK);
    EXPECT_EQ(SignalProcessorData.Error, SIGNAL_PROCESSOR_NO_ERROR);
    EXPECT_EQ(outputMsg, SIGNAL_PROCESSOR_TPMS_WARNING);
    EXPECT_EQ(tireID, 3);

    POSTTEST();
}

TEST(SignalProcessor, Test_GetError_Negative)
{
    POSTTEST();


    returnType ret;
    uint8 invalidInitFlag = 0;
    SignalProcessorErrorType err = SIGNAL_PROCESSOR_NO_ERROR;

    PRETEST();

    SignalProcessor_Init();
    SignalProcessorData.Initialized = invalidInitFlag;
    SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_INVALID_SENSOR_DATA;
    ret = SignalProcessor_GetError( &err );
    EXPECT_EQ(ret, E_NOT_OK);
    EXPECT_EQ(err, SIGNAL_PROCESSOR_NO_ERROR);

    POSTTEST();
}

TEST(SignalProcessor, Test_GetError_Positive)
{
    returnType ret;
    SignalProcessorErrorType err;

    PRETEST();

    SignalProcessor_Init();
    SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_INVALID_SENSOR_DATA;
    ret = SignalProcessor_GetError( &err );
    EXPECT_EQ(ret, E_OK);
    EXPECT_EQ(err, SIGNAL_PROCESSOR_ERR_INVALID_SENSOR_DATA);

    POSTTEST();
}