#include "MCUWakeUp.h"

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/

// void    MCUWakeUp_CheckWakeUpSignal();
// boolean WakeUpCircuitTriggered     ();

static void MCUWakeUp_StateHandler_Undefined();
static void MCUWakeUp_StateHandler_Active();
static void MCUWakeUp_StateHandler_LowPower();

static void MCUWakeUp_StateTrigger_Active();
static void MCUWakeUp_StateTrigger_LowPower();

/***********************************************************************************************************************
Variables
***********************************************************************************************************************/
extern MCUWakeUpDataType MCUWakeUpData;

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/
void MCUWakeUpInit()
{
    MCUWakeUpData.Initialized = MCU_WAKE_UP_INITIALIZED;
    MCUWakeUpData.State = MCUWakeUp_State_Undefined;
    MCUWakeUpData.RequestLowPower = TRUE;
}

void MCUWakeUp_Run() 
{
    switch ( MCUWakeUpData.State )
    {
        case MCUWakeUp_State_Undefined:
            MCUWakeUp_StateHandler_Undefined();
            break;
        case MCUWakeUp_State_Active:
            MCUWakeUp_StateHandler_Active();
            break;   
        case MCUWakeUp_State_LowPower:
            MCUWakeUp_StateHandler_LowPower();
            break;

        default:
            //TODO: Exception/error
            break;
    }
}

// boolean WakeUpCircuitTriggered() 
// {
//     // Code to check wake-up circuit signal (callback; can return returnType)
// }

static void MCUWakeUp_StateHandler_Undefined()
{
    if( MCU_WAKE_UP_INITIALIZED == MCUWakeUpData.Initialized ) //i ako veche imame izprateni danni ot senzorite
    {
        MCUWakeUp_StateTrigger_Active();
    }
    else if( MCU_WAKE_UP_INITIALIZED == MCUWakeUpData.Initialized )
    {
        MCUWakeUp_StateTrigger_LowPower();
    }
    else
    {
        MCUWakeUp_SignalError( MCUWAKEUP_ERR_UNINITIALIZED );
    }
}

static void MCUWakeUp_StateHandler_Active()
{
    if( TRUE == MCUWakeUpData.RequestLowPower )
    {
        MCUWakeUp_StateTrigger_LowPower();
        MCUWakeUpData.RequestLowPower = FALSE;
    }  
}
static void MCUWakeUp_StateHandler_LowPower()
{
    Timer_TimerStateType timerState;

    if( E_OK == Timer_CheckTimer( &MCUWakeUpData.LowPowerTimer, &timerState ) )
    {
        if( Timer_Expired == timerState )
        {
            MCUWakeUp_StateTrigger_Active();
        }
    }
    else
    {
        MCUWakeUp_SignalError( MCUWAKEUP_ERR_TIMER_ERROR );
    }
}

static void MCUWakeUp_StateTrigger_Active()
{
    if( E_OK == Timer_SetTimer( &MCUWakeUpData.LowPowerTimer, 500 ) )
    {
        MCUWakeUpData.State = MCUWakeUp_State_Active;
    }
    else 
    {
        MCUWakeUp_SignalError( MCUWAKEUP_ERR_TIMER_ERROR );
    }
}

static void MCUWakeUp_StateTrigger_LowPower()
{
    MCUWakeUpData.State = MCUWakeUp_State_LowPower;
}

//API
returnType MCUWakeUp_RequestLowPower()
{
    MCUWakeUpData.RequestLowPower = TRUE;
}