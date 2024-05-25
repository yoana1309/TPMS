#include "Sensors.h"

SensorsDataType SensorsData[ Sensors_Count ];

void Sensors_Init( uint8 index )
{
    SensorsData[ index ].tireID = Sensors_Count;
    SensorsData[ index ].signalType = Sensors_Signal_Count;
    SensorsData[ index ].reading = 0;   
}

returnType Sensors_Run()
{
    MCUWakeUpStatesType wakeUpState;
    returnType ret = E_NOT_OK;

    if( E_OK == MCUWakeUp_GetState( &wakeUpState ) )
    {
        if( MCUWakeUp_State_Active == wakeUpState )
        {
            TPMS_Run();
            if( E_OK == MCUWakeUp_RequestLowPower() )
            {
                ret = E_OK;
            }
            else 
            {
                Sensors_SignalError( Sensors_Count, SENSORS_MCUWAKEUP_REQLOWPOWER_ERROR );
                ret = E_NOT_OK;
            }
        }
    }
    else 
    {
        Sensors_SignalError( Sensors_Count, SENSORS_MCUWAKEUP_GETSTATE_ERROR );
        ret = E_NOT_OK;
    }
}


//API
returnType Sensors_GetSignal( uint8 index, SensorsSignalType signalType, uint16* reading )
{
    uint8 ret = E_NOT_OK;
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
    }

    return ret;
}


