#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "C:\diplomna\TPMS\src\common\Types.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SIGNAL_PROCESSOR_INITIALIZED 1u

//MCU has internal 10-bit ADC (therefore ADC output can range from 0 to 1024)
#define ADC_CONVERSION_MIN_RANGE 0u 
#define ADC_CONVERSION_MAX_RANGE 1023

#define MIN_PRESSURE_THRESHOLD 138u //kPa
#define MAX_PRESSURE_THRESHOLD 345u //kPa 
#define MAX_TEMPERATURE_THRESHOLD 90u //degrees Celsius

/***********************************************************************************************************************
Enum definitions
***********************************************************************************************************************/
typedef enum
{
    SIGNAL_PROCESSOR_NO_ERROR = 0,
    SIGNAL_PROCESSOR_ERR_UNINITIALIZED,
    SIGNAL_PROCESSOR_ERR_INVALID_SENSOR_DATA,
    SIGNAL_PROCESSOR_ERR_INVALID_SENSOR_ID,
    SIGNAL_PROCESSOR_ERR_READ_PRESSURE_ERROR,
    SIGNAL_PROCESSOR_ERR_READ_TEMPERATURE_ERROR
}SignalProcessorErrorType;

typedef enum
{
    SIGNAL_PROCESSOR_TPMS_OK = 0,
    SIGNAL_PROCESSOR_TPMS_WARNING
}SignalProcessorOutputMessageType;

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef struct
{
    uint8 Initialized;
    uint8 TireID;
    uint16 PressureReading;
    uint16 TemperatureReading;
    uint16 PressureKPa;
    sint8 TemperatureCelsius;
    SignalProcessorOutputMessageType OutputMessage;
    SignalProcessorErrorType Error;
}SignalProcessorDataType;

/***********************************************************************************************************************
Public variables
***********************************************************************************************************************/
extern SignalProcessorDataType SignalProcessorData;

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/
extern void SignalProcessor_Init();
extern void SignalProcessor_Run();

extern returnType SignalProcessor_GetOutputMessage( SignalProcessorOutputMessageType* outputMsg, uint8* tireID );
extern returnType SignalProcessor_GetError( SignalProcessorErrorType* err );

extern returnType SignalProcessor_ReadPressureSensorData( uint16* reading, uint8* id );
extern returnType SignalProcessor_ReadTemperatureSensorData( uint16* reading, uint8* id );


#endif /* SIGNAL_PROCESSOR_H */