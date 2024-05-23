#ifndef MCU_WAKE_UP_H
#define MCU_WAKE_UP_H

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef enum
{
    MCUWakeUpState_Enable = 0,
    MCUWakeUpState_Disable
} TPMSActivityStateType;

//defs
typedef enum
{
   MCU_WAKE_UP_REQ_ENABLE = 0x0000,
   MCU_WAKE_UP_REQ_DISABLE = 0x0000,

   MCU_WAKE_UP_REQ_ALL = 0xFFFF
} MCUWakeUpRequestsType;

typedef enum
{
    MCUWakeUp_State_Undefined = 0,
    MCUWakeUp_State_Active,
    MCUWakeUp_State_LowPower,

    MCUWakeUp_State_Count
} MCUWakeUpStatesType;

//Params
typedef struct 
{
    
}MCUWakeUpParametersType;

//Data
typedef struct
{
    uint8 Initialized;
    MCUWakeUpStatesType State;
}MCUWakeUpDataType;

//cfg

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/

extern void           MCUWakeUp_RequestSet             ( MCUWakeUpRequestsType   request );
extern void           MCUWakeUp_RequestReset           ( MCUWakeUpRequestsType   request );
extern boolean        MCUWakeUp_RequestIsSet           ( MCUWakeUpRequestsType   request );
// extern void           MCUWakeUp_Error                  ( MCUWakeUpErrorCodesType errCode );
// extern returnType     MCUWakeUp_CheckExternalConditions();

#endif /* MCU_WAKE_UP_H */