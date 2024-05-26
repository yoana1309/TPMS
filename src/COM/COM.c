#include "COM.h"

//API

returnType COM_TriggerWarning( uint8 tireIndex, TPMSWarningType warning )
{
    //TODO: proverka za validnost na argumentite?

    returnType ret = E_NOT_OK;

    if( E_OK == COM_RequestLEDOn( warning ) )
    {
        if( E_OK == COM_WarnDiag( tireIndex, warning ) )
        {
            ret = E_OK;
        }
        else
        {
            COM_SignalError( COM_ERR_DIAG_WARNING );
            ret = E_NOT_OK;
        }
    }
    else
    {
        COM_SignalError( COM_ERR_LED_ON_REQUEST );
        ret = E_NOT_OK;
    }

    return ret;
}