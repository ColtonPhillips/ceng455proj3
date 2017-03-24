/* ###################################################################
**     Filename    : os_tasks.c
**     Project     : dd_scheduler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-08, 14:24, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         dd_scheduler_task - void dd_scheduler_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         
/* MODULE os_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif 



/* User includes (#include below this line is not maintained by Processor Expert) */
#include "helper_function.h"

/*
** ===================================================================
**     Callback    : dd_scheduler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/



#define LOW_USER_TASK_PRIORITY 25
#define SCHEDULER_PRIORITY 15
#define SCHEDULED_USER_TASK_PRIORITY 18
int numOfRunningTasks = 0;
void dd_scheduler_task(os_task_param_t task_init_data)
{
   // Open a Q for the DDscheduler to use, then lower it's priority (from 11) to 15
	_queue_id dd_qid = qopen(DD_QUEUE);
	priorityset(SCHEDULER_PRIORITY); // PRIORITY IS NOW BELOW THE GENERATOR

	// BLOCKS UNTIL GENERATOR BLOCKS WHEN IT SENDS A MESSAGE

	// This starts out with an infinite timeout until it gets it's first task!
	unsigned int deadlineTimeout = 0;

	// Allocate the Task Node Array buffer of a fixed size and set it all to zero (deadline) aka no active task in list
	TASK_NODE_PTR task_node_array = zeroDeadlineTaskNodeArrayFactory();

	// The currently executing task is stored here as a pointer and is NULL if there is no EDF (aka set deadlineTimeout to 0)
	TASK_NODE_PTR earliestDeadlineTask = NULL;

	while (1) {
		// Wait for deadlineTimeout ms for a message (forever if =0)
		if (earliestDeadlineTask != NULL) {
			printf("RUNNING TASKS:%d\n", numOfRunningTasks);
			printf("DEADLINE:%d\n",(int) deadlineTimeout);
		} else {
		}

		MESSAGE_PTR pMsg = msgreceivetimeout(DD_QUEUE, deadlineTimeout);

		// v Handle the message v

		// > Message pointer is not a timeout, and there exists an already running task:
		if (pMsg != NULL && earliestDeadlineTask != NULL) {
			// Update EDF so that it's deadline is fewer, and it's 'creation' is later
			UpdateEveryNodeWithRespectToTime(task_node_array);
		}

		// > Handle the deadline timeout case
		if (pMsg == NULL) {
			// Turn the task off. abort the task. lower active task count
			earliestDeadlineTask->deadline = NO_TASK;
			numOfRunningTasks--;
			_task_abort(earliestDeadlineTask->tid);

			// 1. Refresh the active task.
			// 2. 	If an active task still exists: update his Node and set the deadlineTimeout to his deadline
			// 		else If there are now no active tasks, set deadlineTimeout to zero
			refreshEarliestDeadlineTask(&earliestDeadlineTask, task_node_array);
			if (earliestDeadlineTask == NULL) {
				deadlineTimeout = 0;
			}
			else {
				deadlineTimeout = earliestDeadlineTask->deadline;
			}
		}

		// > Handle the dd_tcreate message:
		else if (msgsrc_equals_q(pMsg,TASK_CREATOR_QUEUE)) {

			// put the sent task into the task list
			bool success = insertIntoTaskList(task_node_array, pMsg->TASK_DATA, &numOfRunningTasks);

			// We are done using the message now
			_msg_free(pMsg);

			// Make a copy reference to the value of EDF (Null if no active task) before EDF is recalculated
			TASK_NODE_PTR prevEDFTask = earliestDeadlineTask;

			// Since a task is added we have to refresh the EDF pointer in case the new task is high priority

			refreshEarliestDeadlineTask(&earliestDeadlineTask, task_node_array);
			// If new task is added successfully:
			// 		-> raise the EDF task priority to 18
			//		If lastEDF existed (was running), and isn't the same as the new task
			// 			-> lower it's priority to 25
			if (success) {
				prioritysettask(earliestDeadlineTask->tid,SCHEDULED_USER_TASK_PRIORITY);
				if (prevEDFTask != NULL && prevEDFTask != earliestDeadlineTask) {
					prioritysettask(prevEDFTask->tid,LOW_USER_TASK_PRIORITY);
				}
			}

			// If an EDF exists, it needs to update it's deadline and creation time, and the deadlineTimeout.
			if (earliestDeadlineTask != NULL) {
				deadlineTimeout = earliestDeadlineTask->deadline;
			}
			// If there is no EDFs (no success) the deadline should wait forever again
			else {
				deadlineTimeout = 0;
			}

			// Allocate a return data message, populate it, and send
			UCHAR_PTR returnData;
			if (success) {returnData = TaskCreatedString;} else {returnData = TaskCreatedFailedString;}
			msgpushdata(
				DD_QUEUE, 			// src
				TASK_CREATOR_QUEUE, // target
				returnData); 		// data
		}
		// > Handle dd_delete message
		else if (msgsrc_equals_q(pMsg,TASK_DELETOR_QUEUE)) {

			// Delete the sent task from the active running tasks
			bool success = deleteFromTaskList(task_node_array, pMsg->TASK_DATA, &numOfRunningTasks);

			// We are done using the message now
			_msg_free(pMsg);

			// Since a task is removed from the array, we refresh the EDF (EDF = NULL if no active tasks exist)
			refreshEarliestDeadlineTask(&earliestDeadlineTask, task_node_array);

			// If successful deletion and an EDF now exists, set it to priority 18
			if (success && earliestDeadlineTask != NULL) {
				prioritysettask(earliestDeadlineTask->tid,SCHEDULED_USER_TASK_PRIORITY);
//				println("DEL RESCHEDULE");
			}

			// If an active task is available, update it's deadline and creation time, and set deadlineTimeout to its deadline
			if (earliestDeadlineTask != NULL) {
				deadlineTimeout = earliestDeadlineTask->deadline;
			}
			// No active jobs, so we have to wait!
			else {
				deadlineTimeout = 0;
			}

			// Allocate a return data message, populate it, and send (preempting function)
			UCHAR_PTR returnData;
			if (success) {returnData = TaskDeletedString;} else {returnData = TaskDeletedFailedString;}
			msgpushdata(
				DD_QUEUE, 			// src
				TASK_DELETOR_QUEUE, // target
				returnData); 		// data
		}

		else if (msgsrc_equals_q(pMsg,ACTIVE_LIST_QUEUE)) {

			// We are done using the message now
			_msg_free(pMsg);

			// get new memory of the for the list of tasks running:
			unsigned int activeTaskListSize = numOfRunningTasks;
			TASK_NODE_PTR activeTaskListHead = getActiveTaskHeadPtr(task_node_array, numOfRunningTasks);

			// the EDF is surely the monitor task: update the deadlineTimeout
			deadlineTimeout = earliestDeadlineTask->deadline;

			// Allocate a return data message, populate it, and send (preempting function)
			UCHAR_PTR returnData;
			if (1) {returnData = ActiveListPassedString;} else {returnData = ActiveListFailedString;}
			monitormsgpush(
				DD_QUEUE, 			// src
				TASK_DELETOR_QUEUE, // target
				activeTaskListHead, // Task list
				activeTaskListSize, // num of tasks running
				returnData); 		// data
		}

		else if (msgsrc_equals_q(pMsg,OVERDUE_LIST_QUEUE)) {
			//Copy a new chunk of memory the size of the active array
			//
		}


		printActiveTasksPriorites(task_node_array);
	}
	_msgq_close(dd_qid);
	abortme();
}

/*
** ===================================================================
**     Callback    : generator_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
#define TIMER_TASK_PRIORITY 2
#define TIMER_STACK_SIZE 2000

typedef struct my_periodic_task {
	unsigned int execution;
	unsigned int deadline;
	unsigned int period;
} PERIODIC_TASK, * PERIODIC_TASK_PTR;

static void PeriodicallyAddTask(
		_timer_id id,
		void * data_ptr,
		MQX_TICK_STRUCT_PTR tick_ptr) {

	PERIODIC_TASK_PTR pPeriodicTask = (PERIODIC_TASK_PTR) data_ptr;
	dd_tcreate(DD_USER_TASK, pPeriodicTask->execution, pPeriodicTask->deadline);

	_time_init_ticks(tick_ptr,0);
	_time_add_msec_to_ticks(tick_ptr,pPeriodicTask->period);
	_timer_start_periodic_every_ticks(PeriodicallyAddTask,pPeriodicTask,TIMER_KERNEL_TIME_MODE, tick_ptr);
}


#define NUM_OF_PERIODIC_TASKS 3
MQX_TICK_STRUCT periodticks[NUM_OF_PERIODIC_TASKS];
_timer_id periodtimes[NUM_OF_PERIODIC_TASKS];
void PeriodTaskInit(PERIODIC_TASK_PTR pPeriodicTasks) {
	_timer_create_component(TIMER_TASK_PRIORITY, TIMER_STACK_SIZE);
	int i;
	for (i = 0; i < NUM_OF_PERIODIC_TASKS; i++) {
		MQX_TICK_STRUCT_PTR pTicks = &periodticks[i];
		_timer_id * pTimer = &periodtimes[i];
		_time_init_ticks(pTicks, 0);
		_time_add_msec_to_ticks(pTicks, pPeriodicTasks[i].period);
		*pTimer = _timer_start_periodic_every_ticks(PeriodicallyAddTask,&pPeriodicTasks[i],TIMER_KERNEL_TIME_MODE, pTicks);
	}
}

void printPeriodTasks (PERIODIC_TASK_PTR pPeriodicTasks) {
	int i;
	for (i = 0; i < NUM_OF_PERIODIC_TASKS; i++) {
		printf(" DL%d = %d  ", i, pPeriodicTasks[i].deadline);
	}
	printf("\n");
}

//TESTS
//#define SIMPLE_TEST
#define PERIODIC_TEST

// The task generator creates and tests tasks for the DD scheduler
#define MONITORING_PERIOD 40000
void generator_task(os_task_param_t task_init_data)
{
#ifdef SIMPLE_TEST
	// These aperiodic tasks should definitely be created and definitely be overdue
	asrt(dd_tcreate(DD_USER_TASK, 100, 1500));
	asrt(dd_tcreate(DD_USER_TASK, 200, 1500));
	asrt(dd_tcreate(DD_USER_TASK, 100, 1500));
	asrt(dd_tcreate(DD_MONITOR_TASK, 0, MONITORING_PERIOD));
#endif

#ifdef PERIODIC_TEST
	// Create Timer(s) for periodic tasks
	PERIODIC_TASK periods[NUM_OF_PERIODIC_TASKS] = {{100,4000, 5000},{150,4000, 11000},{200,10000, 15000}};
	printPeriodTasks(periods);
	PeriodTaskInit(periods);
#endif

	while (1) {
		_task_block();
	}
}

/*
** ===================================================================
**     Callback    : monitor_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void monitor_task(os_task_param_t task_init_data)
{
	synthetic_compute_ms(1300);

	// Get Overdue and Active Tasks
	TASK_NODE_PTR active_tasks_head_ptr  = NULL;
	TASK_NODE_PTR overdue_tasks_head_ptr = NULL;
	unsigned int active_size = 0;
	unsigned int overdue_size = 0;
	//dd_return_active_list(&active_tasks_head_ptr, &active_size);
	//dd_return_overdue_list(&overdue_tasks_head_ptr, &overdue_size);

	// Set Red Light if any tasks overdue
	out_kill_lights();
	if (overdue_tasks_head_ptr == NULL) {
		red_light();
	}

	if (active_tasks_head_ptr == NULL) {
		blue_light();
	}

	if (in_left_button()) {
		red_light();
	}

	if (in_right_button()) {
		dd_tcreate(DD_USER_TASK,10000,23456);
	}

	// Free the memory my dude.
	/*int i;
	for (i = 0; i < active_size; i++) {
		_mem_free(&(active_tasks_head_ptr[i]));
	}
	for (i = 0; i < overdue_size; i++) {
		_mem_free(&(overdue_tasks_head_ptr[i]));
	}
*/
	// Delete this task from DD and schedule another one! :)
	bool b = dd_delete(_task_get_id());
	if (!b) {println("MONITOR DELETE FAILED");}
	dd_tcreate(DD_MONITOR_TASK,0,MONITORING_PERIOD);
	abortme();
}

/*
** ===================================================================
**     Callback    : idle_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void idle_task(os_task_param_t task_init_data)
{

	// Spin forever
	while(1) {
		//putchar('.');
	}

	abortme();
}

/*
** ===================================================================
**     Callback    : user_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void user_task(os_task_param_t task_init_data)
{
	unsigned int executionTime = (unsigned int) task_init_data;
	synthetic_compute_ms(executionTime);
	bool b = dd_delete(_task_get_id());
	if (!b) {println("DELETE FAILED");}
	abortme();
}

/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
