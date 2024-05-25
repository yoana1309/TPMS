#ifndef SENSORS_H
#define SENSORS_H

#include "C:\diplomna\TPMS\src\common\Types.h"
#include "C:\diplomna\TPMS\src\MCUWakeUp\MCUWakeUp.h"
#include "C:\diplomna\TPMS\src\TPMS\TPMS.h"

typedef enum
{
    Sensors_Pressure_FL = 0, //Front left tire pressure sensor
    Sensors_Pressure_FR,     //Front right tire pressure sensor
    Sensors_Pressure_RL,     //Rear left tire pressure sensor
    Sensors_Pressure_RR,     //Rear right tire pressure sensor
    Sensors_Temperature_FL,  //Front left tire temperature sensor
    Sensors_Temperature_FR,  //Front right tire temperature sensor
    Sensors_Temperature_RL,  //Rear left tire temperature sensor
    Sensors_Temperature_RR,  //Rear right tire temperature sensor

    Sensors_Count
} SensorsType;

typedef enum
{
    Sensors_Signal_Pressure = 0,
    Sensors_Signal_Temperature,

    Sensors_Signal_Count
} SensorsSignalType;

typedef enum
{
    SENSORS_NO_ERROR = 0,
    SENSORS_READ_SIGNAL_ERROR,
    SENSORS_INVALID_READING_ERROR,
    SENSORS_MCUWAKEUP_GETSTATE_ERROR,
    SENSORS_MCUWAKEUP_REQLOWPOWER_ERROR
} SensorsErrorType;

typedef struct
{
    uint8 tireID;
    SensorsSignalType signalType;
    uint16 reading;
}SensorsDataType;

extern void Sensors_Init( uint8 index );
extern returnType Sensors_Run();

//api
extern returnType Sensors_GetSignal( uint8 index, SensorsSignalType signalType, uint16* reading );

//callbacks
extern returnType Sensors_ReadSignal( uint8 index, SensorsSignalType signalType, uint16* reading );
extern void Sensors_SignalError( uint8 index, SensorsErrorType error );

#endif /* SENSORS_H */