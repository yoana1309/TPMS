#ifndef TPMS_H
#define TPMS_H

#include "C:\diplomna\TPMS\src\common\Types.h"

#define MIN_PRESSURE_THRESHOLD 20u //TODO: define metrics; define min threshold
#define MAX_PRESSURE_THRESHOLD 30u //TODO: define metrics; define min threshold

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef enum
{
    TPMS_FrontLeft_Tire = 0,
    TPMS_FrontRight_Tire,
    TPMS_RearLeft_Tire,
    TPMS_RearRight_Tire
} TPMSTireType;

typedef struct
{
    uint8 LastMeasuredPressure;
    uint8 LastMeasuredTemperature;
}TPMSDataType;

typedef struct
{
    TPMSTireType TireType;
}TPMSParametersType;


/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/

extern void TPMS_Init      ( uint8 index );


#endif /* TPMS_H */