#include "C:\diplomna\TPMS\src\common\Types.h"
#include "MCUWakeUp.h"

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/

// void    MCUWakeUp_CheckWakeUpSignal();
// boolean WakeUpCircuitTriggered     ();
// void    ActivateSystem             ();
// void    EnterLowPowerState         ();

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

//TODO: Fix this function - make a state machine
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

// void ActivateSystem() 
// {
//     // Code to transition system from low-power to active state
// }

// void EnterLowPowerState() 
// {
//     // Code to put system into low-power state (callback; can return returnType)
// }

static void MCUWakeUp_StateHandler_Undefined()
{
    if( MCUWakeUp_RequestIsSet( MCU_WAKE_UP_REQ_ENABLE ) ) //TODO: vmesto tova proverqvame taimer
    {
        MCUWakeUp_StateTrigger_Active();
    }
    else if( MCUWakeUp_RequestIsSet( MCU_WAKE_UP_REQ_DISABLE ) ) //TODO: vmesto tova proverqvame taimer
    {
        MCUWakeUp_StateTrigger_LowPower();
    }
}

static void MCUWakeUp_StateHandler_Active()
{
    //TODO: call TPMS API functions to measure pressure in each tire
    if( MCUWakeUp_RequestIsSet( MCU_WAKE_UP_REQ_ENABLE ) ) //TODO: vmesto tova proverqvame taimer
    {
        MCUWakeUp_StateTrigger_Active();
    }
}
static void MCUWakeUp_StateHandler_LowPower()
{
    //TODO: read from sensors and store info in nvm
    if( MCUWakeUp_RequestIsSet( MCU_WAKE_UP_REQ_DISABLE ) )
    {
        MCUWakeUp_StateTrigger_LowPower();
    }
}

static void MCUWakeUp_StateTrigger_Active()
{
    MCUWakeUpData.State = MCUWakeUp_State_Active;
}

static void MCUWakeUp_StateTrigger_LowPower()
{
    MCUWakeUpData.State = MCUWakeUp_State_LowPower;
}