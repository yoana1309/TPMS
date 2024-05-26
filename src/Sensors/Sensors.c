#include "Sensors.h"

static returnType ValidateInput(uint8 index, SensorsSignalType signalType );

SensorsDataType SensorsData[ Sensors_Count ];

void Sensors_Init()
{
    uint8 index = 0;
    for( index; index < 4; index++ )
    {
        SensorsData[ index ].Initialized = SENSORS_INITIALIZED;
        SensorsData[ index ].tireID = index;
        SensorsData[ index ].signalType = Sensors_Signal_Pressure;
        SensorsData[ index ].reading = 0;
    } 
    for( index; index < Sensors_Count; index++ )
    {
        SensorsData[ index ].Initialized = SENSORS_INITIALIZED;
        SensorsData[ index ].tireID = (index)%2; 
        SensorsData[ index ].signalType = Sensors_Signal_Temperature;
        SensorsData[ index ].reading = 0;
    } 
}

returnType Sensors_Run()
{
    MCUWakeUpStatesType wakeUpState;
    returnType ret;

    ret = MCUWakeUp_GetState( &wakeUpState );
    if( ( E_OK == ret ) && ( MCUWakeUp_State_Active == wakeUpState ) )
    {
        ret = TPMS_Run();
        if( E_OK == ret )
        {
            ret = MCUWakeUp_RequestLowPower();
            if( E_OK == ret )
            {
                ret = E_OK;
            }
            else 
            {
                Sensors_SignalError( Sensors_Count, SENSORS_MCUWAKEUP_REQLOWPOWER_ERROR );
                ret = E_NOT_OK;
            }                
        }
        else
        {
            Sensors_SignalError( Sensors_Count, SENSORS_TPMS_RUN_ERROR );   
        }
    }
    else 
    {
        Sensors_SignalError( Sensors_Count, SENSORS_MCUWAKEUP_GETSTATE_ERROR );
        ret = E_NOT_OK;
    }
}

static returnType ValidateInput (uint8 index, SensorsSignalType signalType )
{ 
    returnType ret;

    if( ( ( index >= 0 ) && ( index < Sensors_Count ) ) &&
        ( ( signalType >= 0 ) && ( signalType < Sensors_Signal_Count ) ) )
    {
        ret = E_OK;
    }
    else
    {
        Sensors_SignalError(Sensors_Count, SENSORS_INVALID_INPUT_ERROR);
        ret = E_NOT_OK;
    }

    return ret;
}


//API
returnType Sensors_GetSignal( uint8 index, SensorsSignalType signalType, uint16* reading )
{
    returnType ret = ValidateInput( index, signalType );

    if( E_OK == ret )
    {
        if( SENSORS_INITIALIZED == SensorsData[ index ].Initialized )
        {
            if( E_OK == Sensors_ReadSignal( index, signalType, reading ) )
            {
                if( *reading <= 1024 )
                {
                    ret = E_OK;
                }
                else
                {
                    Sensors_SignalError( index, SENSORS_INVALID_READING_ERROR );
                    ret = E_NOT_OK;
                }
            }
            else
            {
                Sensors_SignalError( index, SENSORS_READ_SIGNAL_ERROR );
                ret = E_NOT_OK;
            }
        }
        else
        {
            Sensors_SignalError( index, SENSORS_INVALID_INIT_FLAG_ERROR );
            ret = E_NOT_OK;
        }
    }

    return ret;
}


