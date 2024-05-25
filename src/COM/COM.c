#include "COM.h"
#include "C:\diplomna\TPMS\src\TPMS\TPMS.h"

//API

returnType COM_TriggerWarning( uint8 tireIndex, TPMSWarningType warning )
{
    returnType ret = E_NOT_OK;
    
    if( E_OK == COM_WarnDiag( tireIndex, warning ) )
    {
        ret = E_OK;
    }
    else
    {
        COM_SignalError( COM_ERR_DIAG_WARNING );
    }

    if( E_OK == COM_RequestLEDOn( warning ) )
    {
        ret = E_OK;
    }
    else
    {
        COM_SignalError( COM_ERR_LED_ON_REQUEST );
    }

    return ret;
}