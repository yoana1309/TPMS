/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "SignalProcessor.h"

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/
static returnType SignalProcessor_ValidateSensorData( uint16 data, uint8 id);
static void SignalProcessor_ProcessPressure();
static void SignalProcessor_ProcessTemperature();
static void SignalProcessor_EvaluateData();

/***********************************************************************************************************************
Variables
***********************************************************************************************************************/
SignalProcessorDataType SignalProcessorData;

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/
void SignalProcessor_Init()
{
    SignalProcessorData.Initialized = SIGNAL_PROCESSOR_INITIALIZED;
    SignalProcessorData.TireID = 0;
    SignalProcessorData.PressureReading = 0;
    SignalProcessorData.TemperatureReading = 0;
    SignalProcessorData.PressureKPa = 0;
    SignalProcessorData.TemperatureCelsius = 0;
    SignalProcessorData.OutputMessage = 0;
    SignalProcessorData.Error = SIGNAL_PROCESSOR_NO_ERROR;
}

void SignalProcessor_Run()
{
    if( SIGNAL_PROCESSOR_INITIALIZED == SignalProcessorData.Initialized )
    {
        if( E_OK == SignalProcessor_ReadPressureSensorData( &SignalProcessorData.PressureReading, &SignalProcessorData.TireID ) )
        {
            SignalProcessor_ProcessPressure();

            if( E_OK == SignalProcessor_ReadTemperatureSensorData( &SignalProcessorData.TemperatureReading, &SignalProcessorData.TireID ) )
            {
                SignalProcessor_ProcessTemperature();
                SignalProcessor_EvaluateData();
            }
            else
            {
                SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_READ_TEMPERATURE_ERROR;
            }
        }
        else
        {
            SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_READ_PRESSURE_ERROR;
        }
    }
    else
    {
        SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_UNINITIALIZED;
    }
}


static returnType SignalProcessor_ValidateSensorData( uint16 data, uint8 id)
{
    returnType ret; 
    
    if( ( id > 0 ) && ( id <= 4 ) )
    {
        if( ( data <= ADC_CONVERSION_MAX_RANGE ) && 
            ( data >= ADC_CONVERSION_MIN_RANGE) )
        {
            ret = E_OK;
        }
        else
        {
            SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_INVALID_SENSOR_DATA;
            ret = E_NOT_OK;
        }
    }
    else
    {
        SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_INVALID_SENSOR_ID;
        ret = E_NOT_OK;
    }

    return ret;
}

static void SignalProcessor_ProcessPressure()
{
    if( E_OK == SignalProcessor_ValidateSensorData( SignalProcessorData.PressureReading, SignalProcessorData.TireID ) )
    {
        SignalProcessorData.PressureKPa = 100 + (uint16)(SignalProcessorData.PressureReading*((float)400/1024));
    }
}

static void SignalProcessor_ProcessTemperature()
{
    if( E_OK == SignalProcessor_ValidateSensorData( SignalProcessorData.TemperatureReading, SignalProcessorData.TireID ) )
    {
        SignalProcessorData.TemperatureCelsius = -40 + (uint16)(SignalProcessorData.TemperatureReading*((float)165/1024));
    }
}

static void SignalProcessor_EvaluateData()
{
    if ( ( SignalProcessorData.PressureKPa < MIN_PRESSURE_THRESHOLD ) ||
         ( SignalProcessorData.PressureKPa > MAX_PRESSURE_THRESHOLD ) ||
         ( SignalProcessorData.TemperatureCelsius > MAX_TEMPERATURE_THRESHOLD ) ) 
    {
        SignalProcessorData.OutputMessage = SIGNAL_PROCESSOR_TPMS_WARNING;
    }
    else
    {
        SignalProcessorData.OutputMessage = SIGNAL_PROCESSOR_TPMS_OK;
    }
}

returnType SignalProcessor_GetOutputMessage( SignalProcessorOutputMessageType* outputMsg, uint8* tireID )
{
    returnType ret;

    if( SIGNAL_PROCESSOR_INITIALIZED == SignalProcessorData.Initialized )
    {
        *outputMsg = SignalProcessorData.OutputMessage;
        *tireID = SignalProcessorData.TireID;
        ret = E_OK;
    }
    else
    {
        SignalProcessorData.Error = SIGNAL_PROCESSOR_ERR_UNINITIALIZED;
        ret = E_NOT_OK;
    }

    return ret;
}

returnType SignalProcessor_GetError( SignalProcessorErrorType* err )
{
    returnType ret = E_NOT_OK;

    if( SIGNAL_PROCESSOR_INITIALIZED == SignalProcessorData.Initialized )
    {
        *err = SignalProcessorData.Error;
        ret = E_OK;
    }

    return ret;
}







