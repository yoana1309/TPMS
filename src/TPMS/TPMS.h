#ifndef TPMS_H
#define TPMS_H

#include "C:\diplomna\TPMS\src\common\Types.h"

#define MIN_PRESSURE_THRESHOLD 20u //TODO: define metrics; define min threshold
#define MAX_PRESSURE_THRESHOLD 30u //TODO: define metrics; define min threshold
#define MIN_TEMPERATURE_THRESHOLD 20u //TODO: define metrics; define min threshold
#define MAX_TEMPERATURE_THRESHOLD 30u //TODO: define metrics; define min threshold

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef enum
{
    TPMS_Tire_FrontLeft = 0,
    TPMS_Tire_FrontRight,
    TPMS_Tire_RearLeft,
    TPMS_Tire_RearRight,

    TPMS_Tire_Count
} TPMSTireType;

typedef enum
{
    TPMS_Signal_Pressure = 0,
    TPMS_Signal_Temperature,

    TPMS_Signal_Count
} TPMSSignalType;

typedef enum
{
    TPMS_NO_ERROR = 0,
    TPMS_ERR_PRESSURE_READING,
    TPMS_ERR_TEMPERATURE_READING
} TPMSErrorType;

typedef enum
{
    TPMS_WARNING_LOW_PRESSURE,
    TPMS_WARNING_HIGH_PRESSURE,
    TPMS_WARNING_HIGH_TEMPERATURE
} TPMSWarningType;

typedef struct
{
    uint16 LastMeasuredPressure;
    uint16 LastMeasuredTemperature;
}TPMSDataType;

typedef struct
{
    TPMSTireType TireType;
}TPMSParametersType;


/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/

extern void TPMS_Init( uint8 index );
extern void TPMS_Run( uint8 index );

//callbacks
extern returnType TPMS_ReadSignal( uint8 index, TPMSSignalType signalType, uint16* signal );
extern void TPMS_SignalError( uint8 index, TPMSErrorType error );

#endif /* TPMS_H */