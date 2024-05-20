#include "Timer.h"

static boolean TickTimer_initialized = FALSE;
static uint16 counter;

returnType TickTimer_Init( void )
{
    // #TODO init other variables
    counter = 0;
    TickTimer_initialized = TRUE;
    return E_OK;
}

returnType TickTimer_SetTimer( Timer_TimerType *timer, const uint16 valueMs )
{
    returnType ret = E_NOT_OK;

    if (NULL_PTR == timer)
    {
        return E_NOT_OK;
    }

    if (0 == valueMs)
    {
        return E_NOT_OK;
    }

    if (TRUE == TickTimer_initialized)
    {
        timer->startTs = counter;
        timer->value = valueMs;
        ret = E_OK;

        timer->inverseStartTs = timer->startTs ^ 0xffffu;
        timer->inverseValue = timer->value ^ 0xffffu;
    }

    return ret;
}

returnType TickTimer_CheckTimer( Timer_TimerType *timer, Timer_TimerStateType *state )
{
    returnType ret = E_NOT_OK;

    if (NULL_PTR == timer)
    {
        return E_NOT_OK;
    }
    if (NULL_PTR == state)
    {
        return E_NOT_OK;
    }

    if (TRUE == TickTimer_initialized)
    {
        if (((timer->startTs ^ timer->inverseStartTs) != 0xffffu) || ((timer->value ^ timer->inverseValue) != 0xffffu))
        {
            *state = TickTimer_Uninitialized;
            return E_NOT_OK;
        }
        //        if(timer->startTs+timer->value <= counter)
        if ((uint16)(counter - timer->startTs) >= timer->value)
        {
            *state = TickTimer_Expired;
        }
        else
        {
            *state = TickTimer_Running;
        }
        ret = E_OK;
    }

    return ret;
}

returnType TickTimer_Tick( void )
{
    returnType ret = E_NOT_OK;

    if (TRUE == TickTimer_initialized)
    {
        counter += 1u;
        ret = E_OK;
    }

    return ret;
}