#include "Timer.h"

//if initialized timer will be synchronized with internal mcu oscillator by OS

static boolean Timer_initialized = FALSE;
static uint16 counter;

returnType Timer_Init( void )
{
    counter = 0;
    Timer_initialized = TRUE;
    return E_OK;
}

returnType Timer_SetTimer( Timer_TimerType *timer, const uint16 valueMs )
{
    returnType ret = E_NOT_OK;

    if ( NULL_PTR == timer )
    {
        return E_NOT_OK;
    }

    if ( 0 == valueMs )
    {
        return E_NOT_OK;
    }

    if ( TRUE == Timer_initialized )
    
    {
        timer->start = counter;
        timer->value = valueMs;
        ret = E_OK;
    }

    return ret;
}

returnType Timer_CheckTimer( Timer_TimerType *timer, Timer_TimerStateType *state )
{
    returnType ret = E_NOT_OK;

    if ( NULL_PTR == timer )
    {
        return E_NOT_OK;
    }
    if ( NULL_PTR == state )
    {
        return E_NOT_OK;
    }

    if ( TRUE == Timer_initialized )
    {
        if ( ( uint16 )( counter - timer->start ) >= timer->value )
        {
            *state = Timer_Expired;
        }
        else
        {
            *state = Timer_Running;
        }
        ret = E_OK;
    }

    return ret;
}

returnType Timer_Tick( void )
{
    returnType ret = E_NOT_OK;

    if ( TRUE == Timer_initialized )
    {
        counter += 1u;
        ret = E_OK;
    }

    return ret;
}