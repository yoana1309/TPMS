/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "Monitor.h"

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/
static returnType Monitor_CheckErrors();

/***********************************************************************************************************************
Variables
***********************************************************************************************************************/
MonitorDataType MonitorData;

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/
void Monitor_Init()
{
    int i;
    for(i = 0; i < ERROR_BUFFER_LEN; i++)
    {
        MonitorData.ErrorBuffer[ i ] = 0;
    }
}

void Monitor_Run()
{
    if ( E_OK == Monitor_CheckErrors() )
    {
        for(int i = 0; i < ERROR_BUFFER_LEN; i++)
        {
            if( MonitorData.ErrorBuffer[ i ] != 0 )
            {
                if( E_OK == MessageSender_TriggerSafeState() )
                {
                    break;
                }
                else
                {
                    MonitorData.ErrorBuffer[ MONITOR_ERROR ] = MONITOR_ERR_TRIGGER_SAFE_STATE_ERROR;
                }
            }
        }
    }
}

static returnType Monitor_CheckErrors()
{
    returnType ret;
    SignalProcessorErrorType SignalProcessorError;
    MessageSenderErrorType MessageSenderError;

    ret = SignalProcessor_GetError( &SignalProcessorError );
    if( E_OK == ret )
    {
        if( SignalProcessorError != 0 )
        {
            MonitorData.ErrorBuffer[ SIGNAL_PROCESSOR_ERROR ] = SignalProcessorError;
        }

        ret = MessageSender_GetError( &MessageSenderError );
        if( E_OK == ret )
        {
            if( MessageSenderError != 0 )
            {
                MonitorData.ErrorBuffer[ MESSAGE_SENDER_ERROR ] = MessageSenderError;
            }
        }
    }

    ret = Monitor_SaveErrors( MonitorData.ErrorBuffer );

    return ret;
}
