#include "TPMS.h"

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/
void TPMS_MonitorPressure  ( uint8 index );
int  TPMS_ReadSignal          ( uint8 index ); //signal receiver func
void TPMS_TriggerAlert     ( uint8 index ); //com func
void TPMS_StorePressureData( uint8 index, int data );

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/
enum TPMS_Instances //in test resources (.h)
{
    TPMS_FRONT_LEFT_TIRE = 0,
    TPMS_FRONT_RIGHT_TIRE,
    TPMS_REAR_LEFT_TIRE,
    TPMS_REAR_RIGHT_TIRE,

    TPMS_COUNT
};

TPMSDataType TPMSData[ TPMS_COUNT ]; //In test resources (.c)



void TPMS_Init( uint8 index )
{
    TPMSData[ index ].LastMeasuredPressure = 0;
    TPMSData[ index ].LastMeasuredTemperature = 0;
}

void TPMS_EvaluatePressure( uint8 index )
{
    TPMSData[ index ].LastMeasuredPressure = TPMS_ReadSignal( index );

    if ( ( TPMSData[ index ].LastMeasuredPressure < MIN_PRESSURE_THRESHOLD ) || ( TPMSData[ index ].LastMeasuredPressure > MAX_PRESSURE_THRESHOLD) ) 
    {
        TPMS_TriggerAlert( index ); //This is a com function -> call a com func here
    }
    else 
    {
        TPMS_StorePressureData(index, TPMSData[ index ].LastMeasuredPressure); 
    }
}

