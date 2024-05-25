#ifndef COM_H
#define COM_H

#include "C:\diplomna\TPMS\src\common\Types.h"
#include "C:\diplomna\TPMS\src\TPMS\TPMS.h"

typedef enum
{
    COM_NO_ERROR = 0,
    COM_ERR_DIAG_WARNING,
    COM_ERR_LED_ON_REQUEST
} COMErrorType;

//API
extern returnType COM_TriggerWarning( uint8 tireIndex, TPMSWarningType warning ); 

//callbacks
extern returnType COM_WarnDiag( uint8 tireIndex, TPMSWarningType warning );
extern returnType COM_RequestLEDOn( TPMSWarningType warning );
extern void COM_SignalError( COMErrorType error );

#endif /* COM_H */