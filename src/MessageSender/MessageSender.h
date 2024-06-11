#ifndef MESSAGE_SENDER_H
#define MESSAGE_SENDER_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "C:\diplomna\TPMS\src\common\Types.h"
#include "C:\diplomna\TPMS\src\SignalProcessor\SignalProcessor.h"
#include "C:\diplomna\TPMS\src\E2E\E2E.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define MESSAGE_SENDER_INITIALIZED 1u
#define MESSAGE_SENDER_ERROR_CODE_INIT 255u

/***********************************************************************************************************************
Enum definitions
***********************************************************************************************************************/
typedef enum
{
    MESSAGE_SENDER_NO_ERROR = 0, 
    MESSAGE_SENDER_ERR_UNINITIALIZED,
    MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR,
    MESSAGE_SENDER_ERR_GET_SIGNAL_PROCESSOR_OUTPUT_ERROR,
    MESSAGE_SENDER_ERR_E2E_PROTECTION_ERROR
}MessageSenderErrorType;

typedef enum
{
    MessageSender_Message_OK = 0,
    MessageSender_Message_TPMSWarning,
    MessageSender_Message_SystemError
}MessageSenderMessageType;

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef struct
{
    uint8 Initialized;
    uint8 TireID;
    MessageSenderMessageType MessageType;
    uint8 Data;
    boolean SafeState;
    MessageSenderErrorType Error;
}MessageSenderDataType;

/***********************************************************************************************************************
Public variables
***********************************************************************************************************************/
extern MessageSenderDataType MessageSenderData;

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/
extern void MessageSender_Init();
extern void MessageSender_Run();

extern returnType MessageSender_TriggerSafeState();
extern returnType MessageSender_ResetSafeState();
extern returnType MessageSender_GetError( MessageSenderErrorType* err );

extern returnType MessageSender_SendMessage();

#endif /* MESSAGE_SENDER_H */