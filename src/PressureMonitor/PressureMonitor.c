#include "PressureMonitor.h"

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/

int  PressureMonitor_ReadADC          ();
void PressureMonitor_TriggerAlert     ();
void PressureMonitor_StorePressureData(int data);

/***********************************************************************************************************************
Variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/

void PressureMonitor_MonitorPressure()
{
    int pressureData = PressureMonitor_ReadADC();

    if ( ( pressureData < MIN_PRESSURE_THRESHOLD ) || ( pressureData > MAX_PRESSURE_THRESHOLD) ) 
    {
        PressureMonitor_TriggerAlert();
    }
    else 
    {
        PressureMonitor_StorePressureData(pressureData);
    }
}

int PressureMonitor_ReadADC() 
{
    // TODO: Code to read digital data from ADC (callback; can return returnType)
}

void PressureMonitor_TriggerAlert() 
{
    // TODO: Code to send alert via Communication Service (callback; can return returnType)
}

void PressureMonitor_StorePressureData(int data) 
{
    // TODO: Code to store or log pressure data (callback; can return returnType)
}