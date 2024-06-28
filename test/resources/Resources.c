/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "Resources.h"

/***********************************************************************************************************************
Variables
***********************************************************************************************************************/
uint16 invalidSensorData = 2000;

uint16 validPressureData[ VALID_SENSOR_DATA_LEN ] = 
{
    0u, 20u, 555u, 1000u, 1023u
};

uint16 validTemperatureData[ VALID_SENSOR_DATA_LEN ] = 
{
    0u, 20u, 555u, 1000u, 1023u
};

uint16 validSensorID = 4;

// to be filled with messages to be sent by sensor module 
// and to be read by indicator module
uint8 messageBuffer[ VALID_SENSOR_DATA_LEN ] =
{
    65u, 65u, 64u, 65u, 65u
};