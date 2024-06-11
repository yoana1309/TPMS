#ifndef MONITOR_H
#define MONITOR_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "C:\diplomna\TPMS\src\common\Types.h"
#include "C:\diplomna\TPMS\src\MessageSender\MessageSender.h"
#include "C:\diplomna\TPMS\src\SignalProcessor\SignalProcessor.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define ERROR_BUFFER_LEN 3u// equal to the number of components monitored + monitor itself

/***********************************************************************************************************************
Enum definitions
***********************************************************************************************************************/
typedef enum
{
    NO_ERRORS = 0,
    SIGNAL_PROCESSOR_UNINITIALIZED,
    SIGNAL_PROCESSOR_INVALID_SENSOR_DATA,
    SIGNAL_PROCESSOR_INVALID_SENSOR_ID,
    SIGNAL_PROCESSOR_READ_PRESSURE_ERROR,
    SIGNAL_PROCESSOR_READ_TEMPERATURE_ERROR,
    MESSAGE_SENDER_UNINITIALIZED,
    MESSAGE_SENDER_SEND_MESSAGE_ERROR,
    MESSAGE_SENDER_GET_SIGNAL_PROCESSOR_OUTPUT_ERROR,
    MESSAGE_SENDER_E2E_PROTECTION_ERROR,
    MONITOR_ERROR_TRIGGER_SAFE_STATE
}MonitorErrorCodesType;

typedef enum
{
    MONITOR_NO_ERROR = 0,
    MONITOR_ERR_TRIGGER_SAFE_STATE_ERROR
}MonitorErrorType;

typedef enum
{
    SIGNAL_PROCESSOR_ERROR = 0,
    MESSAGE_SENDER_ERROR,
    MONITOR_ERROR
}MonitorErrorBufferPositionsType;

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef struct
{
    MonitorErrorCodesType ErrorBuffer[ ERROR_BUFFER_LEN ];
}MonitorDataType;

/***********************************************************************************************************************
Public variables
***********************************************************************************************************************/
extern MonitorDataType MonitorData;

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/
extern void Monitor_Init();
extern void Monitor_Run();

extern returnType Monitor_SaveErrors( MonitorErrorCodesType ErrorBuffer[ ERROR_BUFFER_LEN ] );

#endif /* MONITOR_H */