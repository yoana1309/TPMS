#ifndef OS_H
#define OS_H

#include "C:\diplomna\TPMS\src\Timer\Timer.h"

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/

/**
 * Init OS component
 *
 * Shall be called before component usage.
 *
 * @return standard return
 *            E_NOT_OK - A task could not start up correctly fot one of the following reasons:
 *              1) Could not set rate-mode timer 
 *              2) Task not in TS_INIT state
 *
 */
extern returnType OS_Init( void );

/**
 * Run a single OS execution cycle
 *
 * Shall be called in a loop when combined with a parallel task/activity manager
 *
 * @return standard return
 *            E_NOT_OK - Full cycle could not be executed correctly for one of the following reasons:
 *              1) Could not add a task in the scheduling table
 *              2) Could not fit within maximum time interval for the execution of active tasks
 *
 */
extern returnType OS_RunOnce( void );

/**
 * Run the OS execution cycle in an infinite loop
 */
extern void OS_Run( void );

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/

/// @brief Indicates whether the scheduler should proceed with predefined task list or rewind to the beginning of che scheduling cycle
typedef enum
{
    SPF_PROCEED = 0,
    SPF_REWIND
} SchedulerProgressFlagType;

/// @brief Indicates whether a task should be skipped, ran immediately or scheduled for running after the end of the evaluation cycle
typedef enum
{
    EM_PASS = 0,
    EM_IMMEDIATE,
    EM_SCHEDULED
} ExecutionModeType;


//OSTaskDefs

/***********************************************************************************************************************
Enumerators
***********************************************************************************************************************/

typedef enum TaskPriority
{
    TP_EMERGENCY = 0,
    TP_REALTIME,
    TP_HIGH,
    TP_MEDIUM,
    TP_LOW,

    TP_COUNT
} TaskPriorityType;

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef enum
{
    // Idle states:
    TS_IDLE = 0,        // Idle:                       Not executed by scheduler
    TS_TIME_WAIT,       // Time wait:                  Not executed by scheduler until requested time interval is elapsed

    // Active states:
    TS_EXEC_LOW,        // Active, Low priority:       Tasks in this state are executed in the end of time slice
    TS_EXEC_MEDIUM,     // Active, Medium priority:    Tasks in this state are executed before low priority tasks
    TS_EXEC_HIGH,       // Active, High priority:      Tasks in this state are executed before medium priority tasks
    TS_RT_EVENT,        // Active, RT event:           Tasks in this state are executed before high priority tasks (in a RT manner)
    TS_EMERGENCY,       // Active, Emergency:          Tasks in this state are called in a loop until all emergency states are cleared

    // Initial/final/fault states:
    TS_INIT   = -1,     // Task is waiting to be initialized
    TS_DEINIT = -2,     // Task is deinitialized and is no longer called by the scheduler (shut down)
    TS_EXCEPT = -3      // Task is in exception state and is no longer called by the scheduler (suspended/zombie state)
} TaskExecutionStateType;

typedef enum
{
    TE_NO_ERROR = 0,    // No error, normal run
    TE_ERROR,           // Recoverable error, normal run
    TE_EXCEPTION        // Non-recoverable error, task suspended
} TaskErrorStateType;

typedef uint8 TaskFaultCodeType;

//typedef sint32 TaskTimerType;
typedef Timer_TimerType TaskTimerType;
typedef uint16 TaskSchedulingIntervalType;

//typedef uint16* TaskStructPtrType;

typedef uint16 TaskRequestsType;
typedef struct TaskRequestPriorityTableStruct{ TaskRequestsType Masks[TP_COUNT]; } TaskRequestPriorityTableType;

typedef void(*InitHandlerType)(void);
typedef void(*RunHandlerType)(void);
//typedef void(*DeinitHandlerType)(void);

typedef struct
{
    InitHandlerType    InitHandler;
    RunHandlerType     RunHandler;

    TaskRequestsType   Requests;
    TaskTimerType      Timer;
    TaskErrorStateType ErrState;
    TaskFaultCodeType  FaultCode;
} TaskHandleType;

typedef struct
{
    const TaskSchedulingIntervalType   SchedulingInterval;
    const TaskRequestPriorityTableType PriorityTable;
    TaskHandleType                     Handle;
    TaskExecutionStateType             ExecState;
} TaskContextType;

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define TASK_REQUEST_SET(ctx, req)   BITSET  (((ctx)->Handle.Requests), ((TaskRequestsType)req))
#define TASK_REQUEST_RESET(ctx, req) BITRESET(((ctx)->Handle.Requests), ((TaskRequestsType)req))
#define TASK_REQUEST_ISSET(ctx, req) BITISSET(((ctx)->Handle.Requests), ((TaskRequestsType)req))

#define TASK_ERROR(ctx, errCode)    do { (ctx)->Handle.ErrState = TE_ERROR; (ctx)->Handle.FaultCode = (TaskFaultCodeType)errCode; } while(0)
#define TASK_EXCEPTION(ctx, exCode) do { (ctx)->Handle.ErrState = TE_EXCEPTION; (ctx)->Handle.FaultCode = (TaskFaultCodeType)exCode; (ctx)->ExecState = TS_EXCEPT; return E_EXCEPT; } while(0)

#define TASK_SCHEDULING_INTERVAL_NONE 0

#define TASK_STATE_VALID(ctx) ((ctx)->ExecState >= 0)
#define TASK_STATE_INVALID(ctx) ((ctx)->ExecState < 0)
#define TASK_CHECK_PRIORITY_MASK(ctx, prio) ( ((ctx)->PriorityTable.Masks[prio] & (ctx)->Handle.Requests) > 0 )
#define TASK_IS_TIMER_DRIVEN(ctx) ( ((ctx)->SchedulingInterval != TASK_SCHEDULING_INTERVAL_NONE) || ((ctx)->ExecState == TS_TIME_WAIT) )

#define CONTEXT_INIT_STATIC(cmp) {\
        .ExecState = TS_INIT,\
        .Handle = \
        {\
            .Requests    = 0,\
            .ErrState    = TE_NO_ERROR,\
            .FaultCode   = 0,\
            .InitHandler = cmp##_Init,\
            .RunHandler  = cmp##_Run\
        },\
        .PriorityTable = cmp##_PriorityTable,\
        .SchedulingInterval = cmp##_SchedulingInterval\
    }

/*************************************
|  Default priority table            |
+-------------------+----------------+
|  TaskPriority     |  Bits          |
+-------------------+----------------+
|  TP_EMERGENCY     |  15, 14        |
|  TP_REALTIME      |  13, 12        |
|  TP_HIGH          |  11-8          |
|  TP_MEDIUM        |  7-4           |
|  TP_LOW           |  3-0           | 
*************************************/
#define TASK_DEFAULT_PRIORITY_TABLE {\
    .Masks =\
    {\
        0xC000,\
        0x3000,\
        0x0F00,\
        0x00F0,\
        0x000F \
    }\
}\

#define TASK_ALTERNATIVE_PRIORITY_TABLE( PRIO_TP_EMERGENCY, PRIO_TP_REALTIME, PRIO_TP_HIGH, PRIO_TP_MEDIUM, PRIO_TP_LOW ) {\
    .Masks =\
    {\
        PRIO_TP_EMERGENCY,\
        PRIO_TP_REALTIME,\
        PRIO_TP_HIGH,\
        PRIO_TP_MEDIUM,\
        PRIO_TP_LOW \
    }\
}\

//OSDefs
/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef struct
{
    // Maximum execution time of a single OS cycle
    const uint16 CycleMaxTimeMs;
} OSParametersType;

typedef struct
{
    // Main OS timer
    Timer_TimerType Timer;
} OSDataType;


//OSTestResources
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#define OS_TASK_LIST_SIZE 16


//OSTaskSchedulerDefs
/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#define TASK_LIST_MAX_LENGTH 16

#define SCHEDULING_TABLE_STATIC_INIT {\
    .PriorityLists =\
        {\
            {\
                .Items = { NULL_PTR },\
                .ItemCount = 0\
            }\
        }\
}

/// @brief List of priorities that are part of the scheduling table (RT and Emergency ones are executed immediately and should not be scheduled)
typedef enum PriorityListIndex
{
    PL_HIGH = 0,
    PL_MEDIUM,
    PL_LOW,

    PL_COUNT
} PriorityListIndexType;

/// @brief List of tasks belonging to one of the declared PriorityListIndexType members
typedef struct
{
    TaskContextType* Items[ TASK_LIST_MAX_LENGTH ];
    uint8 ItemCount;
} TaskSchedulingListType;

/// @brief Definition of the task scheduling table as an array of lists defined by PriorityListIndexType
typedef struct
{
    TaskSchedulingListType PriorityLists[ PL_COUNT ];
} TaskSchedulingTableType;



//OSTaskSheduler

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/

/**
 * Evaluate the Execution state of a task according to the data stored in its handler 
 *
 * This function is used as a first step in the Run routine of the OS component
 *
 * @param[in]  ctx Pointer to the task context
 * @param[out] execMode Execution mode flag calculated with respect of the ExecState field of the task handler
 * @param[out] progressFlag Scheduling table progression flag that depends on the calculated task priority
 *             (SPF_REWIND is returned in emergency state when scheduling table should be neglected)
 *
 * @return standard return
 *            E_OK - Evaluation phase is always successful 
 *
 */
extern returnType OSTaskScheduler_EvaluateTaskState( TaskContextType* ctx, ExecutionModeType* execMode, SchedulerProgressFlagType* progressFlag );

/**
 * Evaluate the Execution state of a task according to the data stored in its handler 
 *
 * This function is used as a first step in the Run routine of the OS component
 *
 * @param[in]  schedulingTable Pointer to the task table
 * @param[in]  ctx Pointer to the task context
 *
 * @return standard return
 *            E_NOT_OK - Task priority does not belong to the scheduling table
 *            E_NOT_OK - Table is full 
 *
 */
extern returnType OSTaskScheduler_ScheduleTask( TaskSchedulingTableType* schedulingTable, TaskContextType* ctx );

/**
 * Reset the scheduling table 
 *
 * This function is used as a first step in the Run routine of the OS component
 *
 * @param[in]  schedulingTable Pointer to the task table
 *
 * @return standard return
 *
 */
extern returnType OSTaskScheduler_ResetSchedulingTable( TaskSchedulingTableType* schedulingTable );


#endif /* OS_H */