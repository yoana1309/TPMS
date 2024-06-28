#ifndef RESOURCES_H
#define RESOURCES_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "C:\diplomna\TPMS\src\common\Types.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define VALID_SENSOR_DATA_LEN 5u
//#define INVALID_SENSOR_READINGS_LEN 10u

extern uint16 invalidSensorData;

extern uint16 validPressureData[ VALID_SENSOR_DATA_LEN ];

extern uint16 validTemperatureData[ VALID_SENSOR_DATA_LEN ];

extern uint16 validSensorID;

extern uint8 messageBuffer[ VALID_SENSOR_DATA_LEN ];

#endif /* RESOURCES_H */

