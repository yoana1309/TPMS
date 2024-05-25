#ifndef MCU_WAKE_UP_H
#define MCU_WAKE_UP_H

#include "C:\diplomna\TPMS\src\common\Types.h"
#include "C:\diplomna\TPMS\src\Timer\Timer.h"

#define MCU_WAKE_UP_INITIALIZED 1u

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/

//defs

typedef enum
{
    MCUWAKEUP_NO_ERROR = 0,
    MCUWAKEUP_ERR_UNINITIALIZED,
    MCUWAKEUP_ERR_TIMER_ERROR
} MCUWakeUpErrorType;

typedef enum
{
    MCUWakeUp_State_Undefined = 0,
    MCUWakeUp_State_Active,
    MCUWakeUp_State_LowPower,

    MCUWakeUp_State_Count
} MCUWakeUpStatesType;


//Data
typedef struct
{
    uint8 Initialized;
    MCUWakeUpStatesType State;
    Timer_TimerType LowPowerTimer;
    boolean RequestLowPower;
}MCUWakeUpDataType;

//cfg

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/
extern void MCUWakeUpRun();
extern void MCUWakeUpInit();

//api
extern returnType MCUWakeUp_GetState( MCUWakeUpStatesType *state );

extern returnType MCUWakeUp_RequestLowPower();

//callbacks
extern void MCUWakeUp_SignalError( MCUWakeUpErrorType error );

#endif /* MCU_WAKE_UP_H */