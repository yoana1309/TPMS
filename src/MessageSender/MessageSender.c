/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "MessageSender.h"

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/
static void MessageSender_FormMessage();
static returnType MessageSender_ProtectMessage();

/***********************************************************************************************************************
Variables
***********************************************************************************************************************/
MessageSenderDataType MessageSenderData;

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/
void MessageSender_Init()
{
    MessageSenderData.Initialized = MESSAGE_SENDER_INITIALIZED;
    MessageSenderData.TireID = 0;
    MessageSenderData.MessageType = MessageSender_Message_OK;
    MessageSenderData.Data = 0;
    MessageSenderData.SafeState = FALSE;
    MessageSenderData.Error = MESSAGE_SENDER_NO_ERROR;
}

void MessageSender_Run()
{
    if( MESSAGE_SENDER_INITIALIZED == MessageSenderData.Initialized )
    {
        MessageSender_FormMessage();

        if( MESSAGE_SENDER_NO_ERROR == MessageSenderData.Error )
        {
            if( E_OK == MessageSender_ProtectMessage() )
            {
                if( E_OK != MessageSender_SendMessage() )
                {
                    MessageSenderData.Error = MESSAGE_SENDER_ERR_SEND_MESSAGE_ERROR;
                }
            }
            else
            {
                MessageSenderData.Error = MESSAGE_SENDER_ERR_E2E_PROTECTION_ERROR;
            }
        }
    }
    else
    {
        MessageSenderData.Error = MESSAGE_SENDER_ERR_UNINITIALIZED;
    }
}

static void MessageSender_FormMessage()
{
    SignalProcessorOutputMessageType Msg;

    if( E_OK == SignalProcessor_GetOutputMessage( &Msg, &MessageSenderData.TireID ))
    {
        if( FALSE == MessageSenderData.SafeState )
        {
            if( SIGNAL_PROCESSOR_TPMS_OK == Msg )
            {
                MessageSenderData.MessageType = MessageSender_Message_OK;
            }
            else if( SIGNAL_PROCESSOR_TPMS_WARNING == Msg )
            {
                MessageSenderData.MessageType = MessageSender_Message_TPMSWarning;
            }
        }
        else
        {
            MessageSenderData.MessageType = MessageSender_Message_SystemError;
        }
        MessageSenderData.Data = MessageSenderData.TireID;
        MessageSenderData.Data <<= 4;
        MessageSenderData.Data += MessageSenderData.MessageType;
        MessageSenderData.Error = MESSAGE_SENDER_NO_ERROR;       
    }
    else
    {
        MessageSenderData.Error = MESSAGE_SENDER_ERR_GET_SIGNAL_PROCESSOR_OUTPUT_ERROR;
    }
}

static returnType MessageSender_ProtectMessage()
{
    returnType ret;
    E2E_P01ConfigType E2EConfigType;
    E2E_P01ProtectStateType ProtectState;

    ret = E2E_P01Protect( &E2EConfigType, &ProtectState, &MessageSenderData.Data );

    return ret;
}

returnType MessageSender_TriggerSafeState()
{
    returnType ret = E_NOT_OK;
    
    if( MESSAGE_SENDER_INITIALIZED == MessageSenderData.Initialized )
    {
        MessageSenderData.SafeState = TRUE;
        ret = E_OK;
    }

    return ret;
}

returnType MessageSender_GetError( MessageSenderErrorType* err )
{
    returnType ret = E_NOT_OK;

    if( MESSAGE_SENDER_INITIALIZED == MessageSenderData.Initialized )
    {
        *err = MessageSenderData.Error;
        ret = E_OK;
    }

    return ret;
}