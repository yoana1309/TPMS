#include "OS.h"
#include "C:\diplomna\TPMS\src\common\Types.h"

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/

TaskContextType* OSTaskList[ OS_TASK_LIST_SIZE + 1 ] = { NULL_PTR };

TaskSchedulingTableType OSSchedulingTable = SCHEDULING_TABLE_STATIC_INIT;

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/

static void UpdateProgressFlag( SchedulerProgressFlagType* progFlag );

//OSTaskScheduler 
static ExecutionModeType CheckTaskTimer     ( TaskContextType* ctx );
static ExecutionModeType CheckEmergencyState( TaskContextType* ctx );
static ExecutionModeType CheckRequestFlags  ( TaskContextType* ctx );
static void              SetExecutionState  ( TaskContextType* ctx, TaskExecutionStateType state );


/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/

static returnType TaskInit( TaskContextType* ctx )
{
    returnType retVal = E_OK;

    if( ctx->ExecState == TS_INIT )
    {
        ctx->Handle.InitHandler();

        if( ctx->ExecState != TS_EXCEPT )
        {
            ctx->ExecState = TS_IDLE;

            if( ctx->SchedulingInterval != TASK_SCHEDULING_INTERVAL_NONE )
            {
                retVal = Timer_SetTimer( &(ctx->Handle.Timer), ctx->SchedulingInterval );
            }
        }
    }
    else
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}

static returnType TaskRun( TaskContextType* ctx )
{
    returnType retVal = E_OK;

    // No need to check if task is in stable state (done by scheduler)
    ctx->Handle.RunHandler();

    return retVal;
}

static returnType TaskSafeRun( TaskContextType* ctx )
{
    returnType retVal = E_OK;

    if( TASK_STATE_VALID( ctx ) )
    {
        ctx->Handle.RunHandler();
    }

    return retVal;
}

static void UpdateProgressFlag( SchedulerProgressFlagType* progFlag )
{
    //TODO: Check maximum background task duration
    *progFlag = SPF_PROCEED;
}

returnType OS_Init( void )
{
    returnType retVal = E_OK;
    int i = 0;

    while( OSTaskList[ i ] != NULL_PTR )
    {
        retVal = ( retVal == E_OK ) ? ( TaskInit( OSTaskList[ i ] ) ) : ( retVal );
        i++;
    }

    return retVal;
}

returnType OS_RunOnce( void )
{
    returnType retVal = E_OK;
    int i = 0;
    ExecutionModeType execMode;
    SchedulerProgressFlagType progressFlag = SPF_PROCEED;

    OSTaskScheduler_ResetSchedulingTable( &OSSchedulingTable );

    while( OSTaskList[ i ] != NULL_PTR )
    {
        OSTaskScheduler_EvaluateTaskState( OSTaskList[ i ], &execMode, &progressFlag );

        if( execMode == EM_IMMEDIATE )
        {
            // Run the task directly without entering it in the scheduling list
            TaskRun( OSTaskList[ i ] );
        }
        else if( ( execMode == EM_SCHEDULED ) && ( progressFlag == SPF_PROCEED ) )
        {
            // Put the task in the scheduling table
            if( OSTaskScheduler_ScheduleTask( &OSSchedulingTable, OSTaskList[ i ] ) != E_OK )
            {
                retVal = E_NOT_OK;
            }
        }

        i++;
    }

    if( progressFlag != SPF_REWIND )
    {
        for( uint8 prio = 0; prio < PL_COUNT; prio++ )
        {
            for( uint8 taskIdx = 0; taskIdx < OSSchedulingTable.PriorityLists[ prio ].ItemCount; taskIdx++ )
            {
                UpdateProgressFlag( &progressFlag );

                if( progressFlag == SPF_REWIND )
                {
                    retVal = E_NOT_OK; // Not enough time to execute entire scheduling table
                    break;
                }

                TaskSafeRun( OSSchedulingTable.PriorityLists[ prio ].Items[ taskIdx ] );
            }
        }
    }

    return retVal;
}

void Run( void )
{
    while( TRUE )
    {
        OS_RunOnce();
    }
}


//Task scheduler implementation; the function prototypes are at the beginning

/***********************************************************************************************************************
Constants
***********************************************************************************************************************/

const TaskExecutionStateType Mapping_TaskPriority2ExecutionState[ TP_COUNT ] = 
{
    TS_EMERGENCY,   // TP_EMERGENCY
    TS_RT_EVENT,    // TP_REALTIME
    TS_EXEC_HIGH,   // TP_HIGH
    TS_EXEC_MEDIUM, // TP_MEDIUM
    TS_EXEC_LOW     // TP_LOW
};

const ExecutionModeType Mapping_TaskPriority2ExecutionMode[ TP_COUNT ] = 
{
    EM_IMMEDIATE, // TP_EMERGENCY
    EM_IMMEDIATE, // TP_REALTIME
    EM_SCHEDULED, // TP_HIGH
    EM_SCHEDULED, // TP_MEDIUM
    EM_SCHEDULED  // TP_LOW
};

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/

// Set to RT event if not yet set to a higher priority one
//TODO: TS_TIME_WAIT should be handled here
static ExecutionModeType CheckTaskTimer( TaskContextType* ctx )
{
    ExecutionModeType retVal = EM_PASS;
    Timer_TimerStateType timerState;

    if( Timer_CheckTimer( &(ctx->Handle.Timer), &timerState ) == E_OK )
    {
        if( timerState == TickTimer_Expired )
        {
            SetExecutionState( ctx, TS_RT_EVENT );
            retVal = EM_IMMEDIATE;

            if( ctx->SchedulingInterval != TASK_SCHEDULING_INTERVAL_NONE )
            {
                Timer_SetTimer( &(ctx->Handle.Timer), ctx->SchedulingInterval );
            }
        }
    }

    return retVal;
}

// Apply emergency flag if such request is set
static ExecutionModeType CheckEmergencyState( TaskContextType* ctx )
{
    ExecutionModeType retVal = EM_PASS;

    if( TASK_CHECK_PRIORITY_MASK(ctx, TP_EMERGENCY) )
    {
        SetExecutionState( ctx, TS_EMERGENCY );
        retVal = EM_IMMEDIATE;
    }

    return retVal;
}

// Set execution state according to task requests
static ExecutionModeType CheckRequestFlags( TaskContextType* ctx )
{
    ExecutionModeType retVal = EM_PASS;

    if( ctx->Handle.Requests != 0 )
    {
        // Iterate through TaskPriority enum values starting from TP_REALTIME, since TP_EMERGENCY is checked in advance
        for( TaskPriorityType prio = TP_REALTIME; prio < TP_COUNT; prio++ )
        {
            if( TASK_CHECK_PRIORITY_MASK( ctx, prio ) )
            {
                SetExecutionState( ctx, Mapping_TaskPriority2ExecutionState[ prio ] );
                retVal = Mapping_TaskPriority2ExecutionMode[ prio ];
                break;
            }
        }
    }

    return retVal;
}

// Set the execution state in a "watermark" manner - only higher than current priorities can be set
static void SetExecutionState( TaskContextType* ctx, TaskExecutionStateType state )
{
    if( state > ctx->ExecState )
    {
        ctx->ExecState = state;
    }
}

returnType OSTaskScheduler_EvaluateTaskState( TaskContextType* ctx, ExecutionModeType* execMode, SchedulerProgressFlagType* progressFlag )
{
    returnType retVal = E_OK;

    *execMode = EM_PASS;

    if( TASK_STATE_VALID( ctx ) )
    {
        //TODO: TS_TIME_WAIT should be handled before resetting to TS_IDLE
        ctx->ExecState = TS_IDLE; // Reset execution state before evaluation to restart watermarking

        *execMode = CheckEmergencyState( ctx );

        if( *execMode == EM_IMMEDIATE )
        {
            // In case of emergency, set the SPF_REWIND flag in order to execute only EM_IMMEDIATE-flagged tasks and skip the official schedule list
            *progressFlag = SPF_REWIND;
        }
        else
        {
            if( TASK_IS_TIMER_DRIVEN( ctx ) )
            {
                *execMode = CheckTaskTimer( ctx );
            }
            else
            {
                *execMode = CheckRequestFlags( ctx );
            }
        }
    }

    return retVal;
}

returnType OSTaskScheduler_ScheduleTask( TaskSchedulingTableType* schedulingTable, TaskContextType* ctx )
{
    returnType retVal = E_OK;

    TaskSchedulingListType* taskList = NULL_PTR;

    switch( ctx->ExecState )
    {
        case TS_EXEC_HIGH:   taskList = &(schedulingTable->PriorityLists[ PL_HIGH   ]); break;
        case TS_EXEC_MEDIUM: taskList = &(schedulingTable->PriorityLists[ PL_MEDIUM ]); break;
        case TS_EXEC_LOW:    taskList = &(schedulingTable->PriorityLists[ PL_LOW    ]); break;
        default:             retVal = E_NOT_OK; break; // Invalid priority index (task should not be scheduled)
    }        

    if( taskList != NULL_PTR )
    {
        if( taskList->ItemCount < TASK_LIST_MAX_LENGTH )
        {
            taskList->Items[ taskList->ItemCount ] = ctx;
            taskList->ItemCount++;
        }
        else
        {
            retVal = E_NOT_OK;  // Table is full
        }
    }

    return retVal;
}

returnType OSTaskScheduler_ResetSchedulingTable( TaskSchedulingTableType* schedulingTable )
{
    for( uint8 prio = 0; prio < PL_COUNT; prio++ )
    {
        schedulingTable->PriorityLists[ prio ].ItemCount = 0;
    }

    return E_OK;
}