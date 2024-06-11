#ifndef E2E_H
#define E2E_H

#include "C:\diplomna\TPMS\src\common\Types.h"

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef struct
{
    //E2E configuration of transmitted data
    //for each transmitted data, there is an instance of this typedef
}E2E_P01ConfigType;

typedef struct
{
    //state of the sender for a data protected with e2e profile 1
}E2E_P01ProtectStateType;

typedef struct
{
    //state of the receiver for a data protected with e2e profile 1
}E2E_P01CheckStateType;

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/
extern returnType E2E_P01Protect( const E2E_P01ConfigType* ConfigPtr, //pointer to static configuration
                                  E2E_P01ProtectStateType* StatePtr,  //pointer to port/data communication state
                                  uint8* DataPtr );                   //pointer to data to be sent


extern returnType E2E_P01Check( const E2E_P01ConfigType* Config, //pointer to static configuration
                                E2E_P01CheckStateType* State,    //pointer to port/data communication state
                                const uint8* Data );             //pointer to received data

#endif /* E2E_H */