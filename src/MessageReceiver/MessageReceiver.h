#ifndef MESSAGE_RECEIVER_H
#define MESSAGE_RECEIVER_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "C:\diplomna\TPMS\src\common\Types.h"
#include "C:\diplomna\TPMS\src\E2E\E2E.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define MESSAGE_RECEIVER_INITIALIZED 1u
#define MESSAGE_TYPE_MASK 15u //decimal value for 00001111

/***********************************************************************************************************************
Enum definitions
***********************************************************************************************************************/
typedef enum
{
    MESSAGE_RECEIVER_TIRE_FRONT_LEFT = 1,
    MESSAGE_RECEIVER_TIRE_FRONT_RIGHT,
    MESSAGE_RECEIVER_TIRE_REAR_LEFT,
    MESSAGE_RECEIVER_TIRE_REAR_RIGHT
}MessageReceiverTireIDType;

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef struct
{
    uint8 Initialized;
    MessageReceiverTireIDType TireID;
    uint8 MessageType;
    uint8 Data;
}MessageReceiverDataType;

/***********************************************************************************************************************
Public variables
***********************************************************************************************************************/
extern MessageReceiverDataType MessageReceiverData;

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/
extern void MessageReceiver_Init();
extern void MessageReceiver_Run();

extern returnType MessageReceiver_ReadMessage( uint8* msg );
extern returnType MessageReceiver_AlertTPMSOK( uint8 tireID ); //green light
extern returnType MessageReceiver_AlertTPMSWarning( uint8 tireID ); //red light
extern returnType MessageReceiver_AlertTPMSSystemError( uint8 tireID ); //blue light

#endif /* MESSAGE_RECEIVER_H */