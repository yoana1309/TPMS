#ifndef TIMER_H
#define TIMER_H

#include "C:\diplomna\TPMS\src\common\Types.h"

typedef struct Timer_TimerStruct
{
    uint16 start;
    uint16 value;
    uint16 inverseStart;
    uint16 inverseValue;
} Timer_TimerType;

typedef enum Timer_TimerStateEnum
{
    Timer_Uninitialized = 0u,
    Timer_Expired,
    Timer_Running
}Timer_TimerStateType;

/**
 * Init Timer component
 *
 * Shall be called before using the component
 *
 * @return standard return
 *
 */
returnType Timer_Init( void );

/**
 * Set timer
 *
 * Set timer variable with current timestamp and value. Subsequent polling with CheckTimer
 *
 * @param[out] timer pointer to timer variable.
 * @param[in]  valueMs value in ms. Range [1..65535].
 * @param[in]  mode DelayMode - Set timer based on the current time.
 *                  RateMode - Set timer based on pervious timer set point.
 * @return standard return
 *            E_NOT_OK - when uninitialized component 
 *            E_NOT_OK - in case of null pointer to timer
 *            E_NOT_OK - in case of out of range of valueMs
 *
 */
returnType Timer_SetTimer( Timer_TimerType *timer, const uint16 valueMs );

/**
 * Check timer
 *
 * Calculates is timer state
 *
 * @param[in]  timer pointer to timer variable.
 * @param[out] state output of the current state
 * @return standard return
 *              E_NOT_OK - when uninitialized component
 *              E_NOT_OK - in case of null pointer to timer
 *              E_NOT_OK - in case of null pointer to state
 *              E_NOT_OK - in case of uninitialized timer
 */
returnType Timer_CheckTimer( Timer_TimerType *timer, Timer_TimerStateType *state );

/**
 * Tick input
 *
 * Shall be called on 1 ms. Increments internal software counter by one.
 *
 * @return standard return
 *         E_NOT_OK - when uninitialized component
 *
 */
returnType Timer_Tick( void );

#endif /* TIMER_H */