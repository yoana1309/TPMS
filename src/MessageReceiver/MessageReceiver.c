/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "MessageReceiver.h"

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/
static returnType MessageReceiver_CheckMessage();
static returnType MessageReceiver_ValidateMessageData();
static void MessageReceiver_ExtractData();
static void MessageReceiver_AlertDriver();

/***********************************************************************************************************************
Variables
***********************************************************************************************************************/
MessageReceiverDataType MessageReceiverData;

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/
void MessageReceiver_Init()
{
    MessageReceiverData.Initialized = MESSAGE_RECEIVER_INITIALIZED;
    MessageReceiverData.TireID = 0;
    MessageReceiverData.MessageType = 0;
    MessageReceiverData.Data = 0;
}

void MessageReceiver_Run()
{
    if( MESSAGE_RECEIVER_INITIALIZED == MessageReceiverData.Initialized )
    {
        if( E_OK == MessageReceiver_ReadMessage( &MessageReceiverData.Data ) )
        {
            if( E_OK == MessageReceiver_CheckMessage() )
            {
                MessageReceiver_ExtractData();
                MessageReceiver_AlertDriver();
            }
            else
            {
                MessageReceiver_AlertTPMSSystemError( MessageReceiverData.TireID );
            }
        }
        else
        {
            MessageReceiver_AlertTPMSSystemError( MessageReceiverData.TireID );
        }
    }
    else
    {
        MessageReceiver_AlertTPMSSystemError( MessageReceiverData.TireID );
    }
}

static returnType MessageReceiver_CheckMessage()
{
    returnType ret;
    E2E_P01ConfigType E2EConfigType;
    E2E_P01CheckStateType CheckState;

    ret = E2E_P01Check( &E2EConfigType, &CheckState, &MessageReceiverData.Data );

    return ret;
}

static void MessageReceiver_ExtractData()
{
    MessageReceiverData.MessageType = MessageReceiverData.Data&MESSAGE_TYPE_MASK;
    MessageReceiverData.TireID = MessageReceiverData.Data >> 4;
}

static void MessageReceiver_AlertDriver()
{
    switch( MessageReceiverData.MessageType )
    {
        case 0:
            MessageReceiver_AlertTPMSOK( MessageReceiverData.TireID );
            break;
        
        case 1:
            MessageReceiver_AlertTPMSWarning( MessageReceiverData.TireID );
            break;
        
        case 2:
            MessageReceiver_AlertTPMSSystemError( MessageReceiverData.TireID );
            break;

        default:
            break;
    }
}

