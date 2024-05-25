#include "TPMS.h"
#include "C:\diplomna\TPMS\src\COM\COM.h"

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/
static returnType TPMS_EvaluatePressure  ( uint8 index );
static returnType TPMS_EvaluateTemperature  ( uint8 index );

static returnType TPMS_ReceivePressureSignal( uint8 index );
static returnType TPMS_ReceiveTemperatureSignal( uint8 index );



/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/

TPMSDataType TPMSData[ TPMS_Tire_Count ]; //In test resources (.c)

void TPMS_Init( uint8 index )
{
    TPMSData[ index ].LastMeasuredPressure = 0;
    TPMSData[ index ].LastMeasuredTemperature = 0;
}

void TPMS_Run()
{
    int tireNumber = 0;
    for(tireNumber; tireNumber < TPMS_Tire_Count; tireNumber++)
    {
        TPMS_ReceivePressureSignal( tireNumber );
        TPMS_ReceiveTemperatureSignal( tireNumber );
        TPMS_EvaluatePressure( tireNumber );
        TPMS_EvaluateTemperature( tireNumber );
    }
}

//Static functions

static returnType TPMS_EvaluatePressure( uint8 index )
{
    uint8 ret = E_NOT_OK;

    if ( TPMSData[ index ].LastMeasuredPressure < MIN_PRESSURE_THRESHOLD ) 
    {
        if( E_OK == COM_TriggerWarning( index, TPMS_WARNING_LOW_PRESSURE ) )
        {
            ret = E_OK;
        }
        else
        {
            TPMS_SignalError( index, TPMS_ERR_COM_TRIGGERWARNING_ERROR );
            ret = E_NOT_OK;
        }
    }
    else if( TPMSData[ index ].LastMeasuredPressure > MAX_PRESSURE_THRESHOLD )
    {
        COM_TriggerWarning( index, TPMS_WARNING_HIGH_PRESSURE ); 
    }

    return ret;
}

static returnType TPMS_EvaluateTemperature( uint8 index )
{
    uint8 ret = E_NOT_OK;

    if ( TPMSData[ index ].LastMeasuredTemperature > MAX_TEMPERATURE_THRESHOLD ) 
    {
        if( E_OK == COM_TriggerWarning( index, TPMS_WARNING_HIGH_TEMPERATURE ) )
        {
            ret = E_OK;
        }
        else
        {
            TPMS_SignalError( index, TPMS_ERR_COM_TRIGGERWARNING_ERROR );
            ret = E_NOT_OK;
        }
    }
    return ret;
}

static returnType TPMS_ReceivePressureSignal( uint8 index )
{
    returnType ret = E_NOT_OK;
    uint16 rawSignal = 0;


    if( E_OK == Sensors_GetSignal( index, Sensors_Signal_Pressure, &rawSignal ) )
    {
        TPMSData[ index ].LastMeasuredPressure = rawSignal; //TODO: formula to transform the raw signal; maybe use another function
        ret = E_OK;
    }
    else
    {
        TPMS_SignalError( index, TPMS_ERR_PRESSURE_READING );
        ret = E_NOT_OK;
    }

    return ret;
}

static returnType TPMS_ReceiveTemperatureSignal( uint8 index )
{
    returnType ret = E_NOT_OK;
    uint16 rawSignal = 0;

    if( E_OK == Sensors_GetSignal( index, Sensors_Signal_Temperature, &rawSignal ) )
    {
        TPMSData[ index ].LastMeasuredTemperature = rawSignal; //TODO: formula to transform the raw signal
        ret = E_OK;
    }
    else
    {
        TPMS_SignalError( index, TPMS_ERR_TEMPERATURE_READING );
        ret = E_NOT_OK;
    }

    return ret;
}


