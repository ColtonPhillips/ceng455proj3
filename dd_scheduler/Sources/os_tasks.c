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
#include "access_functions.h"
#include "MessagePool.h"
#include "Priority.h"
#include "RGB.h"
#include <math.h>
#include <timer.h>
/*
** ===================================================================
**     Callback    : dd_scheduler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/

// IS CALLED WHEN TIMER EXPIRES
void timer_callback(_timer_id t, void* dataptr, unsigned int seconds, unsigned int miliseconds){
    (*(bool*)dataptr) = false;
}

// CREATE BUSY-WAIT DELAY FOR A GIVEN DURATION
void synthetic_compute_ticks(unsigned int ticks){
	bool flag = true;
	MQX_TICK_STRUCT Ticks;
	_time_init_ticks(&Ticks, ticks);
	_timer_start_oneshot_after_ticks(timer_callback, (void *) &flag, TIMER_ELAPSED_TIME_MODE, &Ticks);

	while (flag){}
}

// CREATE BUSY-WAIT DELAY FOR A GIVEN DURATION
void synthetic_compute_ms(unsigned int ms){
	bool flag = true;
	MQX_TICK_STRUCT Ticks;
	_time_init_ticks(&Ticks, 0);
	_time_add_msec_to_ticks(&Ticks, ms);
	_timer_start_oneshot_after_ticks(timer_callback, (void *) &flag, TIMER_ELAPSED_TIME_MODE, &Ticks);

	while (flag){}
}

#define TASK_NODE_ARRAY_SIZE 16
#define NO_TASK 0

// How many tasks are running O(n)
int numOfRunningTasks(TASK_NODE_PTR task_list_array) {
	int i = 0; int num = 0;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		if (task_list_array[i].deadline != NO_TASK) {
			num++;
		}
	}
	return num;
}

// Inserting is O(n) in worst case scenario
bool insertIntoTaskList(TASK_NODE_PTR task_list_array, TASK_NODE insertedTask) {
	// todo: keep track of length of list being used.
	int i = 0;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		if (task_list_array[i].deadline == NO_TASK) {
			task_list_array[i] = insertedTask;
			return true;
		}
	}
	return false;
}

// Deleting is O(n) in worst case scenario
bool deleteFromTaskList(TASK_NODE_PTR task_list_array, TASK_NODE Task) {
	int i = 0;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		if (task_list_array[i].tid == Task.tid && task_list_array[i].deadline != NO_TASK) {
			task_list_array[i].deadline = NO_TASK;
			task_list_array[i].tid = NO_TASK;
			return true;
		}
	}
	return false;
}

// Getting EDF task searches entire array therefore is O(n)
void refreshEarliestDeadlineTask(TASK_NODE_PTR * ppEDF, TASK_NODE_PTR task_list_array) {
	unsigned int DL_min = 999999999; // sufficiently large enough number
	*ppEDF = NULL; // return null if the task list is empty
	int i = 0;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		if (task_list_array[i].deadline < DL_min && task_list_array[i].deadline != NO_TASK) {
			DL_min = task_list_array[i].deadline;
			*ppEDF = (TASK_NODE_PTR) &(task_list_array[i]);
			println("This Happens");
		}
	}
	if (*ppEDF != NULL) {
		println((*ppEDF)->deadline);
	}
}

// Set the entire array of structs' deadlines to zero.
void zeroTaskNodeArray(TASK_NODE_PTR task_node_array) {
	int i;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		task_node_array[i].deadline = NO_TASK;
	}
}

void updateNodeWithRespectToTime(TASK_NODE_PTR tn_ptr) {
	unsigned int ct = currentTime();
	unsigned int delta = ct - tn_ptr->creation_time; // how much time has passed since last time it was updated
	// Deadline is reduced by the amount of time elapsed since the time it was last 'created' (or updated)
	tn_ptr->deadline = tn_ptr->deadline - delta;
	// we just reuse creation time to make things simple for us.
	tn_ptr->creation_time = ct;
}

void dd_scheduler_task(os_task_param_t task_init_data)
{
	println("DQ");

   // Open a Q for the DDscheduler to use, then lower it's priority (from 11) to 15
	_queue_id dd_qid = qopen(DD_QUEUE);
	priorityset(15);
	// BLOCKS UNTIL GENERATOR SENDS A CREATE MESSAGE
	println("DB");

	// This starts out with an infinite timeout until it gets it's first task!
	unsigned int deadlineTimeout = 0;

	// Allocate the Task Node buffer of a fixed size and set it all to zero
	TASK_NODE_PTR task_node_array = _mem_alloc(TASK_NODE_ARRAY_SIZE * sizeof(TASK_NODE));
	zeroTaskNodeArray(task_node_array);

	// The currently executing task is stored here as a pointer
	TASK_NODE_PTR earliestDeadlineTask = NULL;

	while (1) {
		// Wait for Any Message
		println("DSLEEP");
		MESSAGE_PTR msg_ptr = msgreceivetimeout(DD_QUEUE, deadlineTimeout);
		println("DWAKE");
		if (msg_ptr == NULL) {
			printf("NULLMSG\n");
		}

		// Switch on the message's source

		// Message pointer is not a timeout, and there exists an already running task:
		if (msg_ptr != NULL && earliestDeadlineTask != NULL) {
			println("UPD8");
			// Update EDF so that it's deadline is fewer, and it's 'creation' is later
			updateNodeWithRespectToTime(earliestDeadlineTask);
		}
		// if a timeout occurs
		else if (msg_ptr == NULL) {
			// Turn the task off. abort the task. refresh the EDF.
			// If EDF != null, update his deadline and 'creationtime' + set the deadlineTimeout
			// If EDF == null, set timeout to 0 (wait forever)
			// TODO: add to overdue task
			println("TIMEOUT");
			earliestDeadlineTask->deadline = NO_TASK;
			_task_abort(earliestDeadlineTask->tid);
			refreshEarliestDeadlineTask(&earliestDeadlineTask, task_node_array);
			if (earliestDeadlineTask == NULL) {
				deadlineTimeout = 0;
			}
			else {
				updateNodeWithRespectToTime(earliestDeadlineTask);
				deadlineTimeout = earliestDeadlineTask->deadline;
			}
		}
		else if (msgsrc_equals_q(msg_ptr,TASK_CREATOR_QUEUE)) {
			// Create a task if possible

			//printf((UCHAR_PTR)msg_ptr->DATA);

			// put into the task list
			bool success = insertIntoTaskList(task_node_array, msg_ptr->TASK_DATA);

			// Free msg_ptr
			_msg_free(msg_ptr);

			// Copy last task PTR
			TASK_NODE_PTR lastEDFTask = earliestDeadlineTask;

			// Refresh EDF task
			refreshEarliestDeadlineTask(&earliestDeadlineTask, task_node_array);


			// If it's added, lower the last task priority (to 25) (if there was one)
			// and then raise the new task priority (18)
			if (success) {
				println("BUMP UP PRIORITY");
				prioritysettask(earliestDeadlineTask->tid,18);
				if (lastEDFTask != NULL && lastEDFTask != earliestDeadlineTask) {
					prioritysettask(lastEDFTask->tid,25);
					println("BUMP DOWN PRIORITY");
				}
			}

			// Allocate a message, populate it, and send
			UCHAR_PTR returnData;
			if (success) {returnData = TaskCreatedString;} else {returnData = TaskCreatedFailedString;}
			msgpushdata(
				DD_QUEUE, 			// src
				TASK_CREATOR_QUEUE, // target
				returnData); 		// data

			// If an EDF exists, it needs to update it's deadline and stuff
			if (earliestDeadlineTask != NULL) {
				updateNodeWithRespectToTime(earliestDeadlineTask);
				deadlineTimeout = earliestDeadlineTask->deadline;
				out_kill_lights();
			}
			else {
				deadlineTimeout = 0;
				out_white_light();
			}
		}
		else if (msgsrc_equals_q(msg_ptr,TASK_DELETOR_QUEUE)) {
			// Delete a task
			printf((UCHAR_PTR)msg_ptr->DATA); // DELETE?

			// Delete from the task list
			bool success = deleteFromTaskList(task_node_array, msg_ptr->TASK_DATA);

			// Free msg_ptr
			_msg_free(msg_ptr);

			// Refresh EDF task
			refreshEarliestDeadlineTask(&earliestDeadlineTask, task_node_array);

			// If it's deleted and the new EDF is not NULL, raise it's priority (18)
			if (success && earliestDeadlineTask != NULL) {
				prioritysettask(earliestDeadlineTask->tid,18);
				println("DEL RESCHEDULE");
			}

			// If an EDF (still) exists, it needs to update it's deadline and stuff
			if (earliestDeadlineTask != NULL) {
				updateNodeWithRespectToTime(earliestDeadlineTask);
				deadlineTimeout = earliestDeadlineTask->deadline;
			}
			else {
				deadlineTimeout = 0;
			}

			// Allocate a message, populate it, and send
			UCHAR_PTR returnData;
			if (success) {returnData = TaskDeletedString;} else {returnData = TaskDeletedFailedString;}
			msgpushdata(
				DD_QUEUE, 			// src
				TASK_DELETOR_QUEUE, // target
				returnData); 		// data
		}
		// switch case ends
		printf("RUN:%d\n", numOfRunningTasks(task_node_array));
	}
	println("DE");
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

//#define WILL_CREATE_PERIODIC_TASKS
#define PERIODIC_PERIOD 800
#define PERIODIC_EXECUTION 400

static void PeriodicallyAddTask(
		_timer_id id,
		void * data_ptr,
		MQX_TICK_STRUCT_PTR tick_ptr) {
	println("Periodic");

	// Add a new task!
	dd_tcreate(DD_USER_TASK, PERIODIC_EXECUTION, PERIODIC_PERIOD);

	_time_init_ticks(&tick_ptr,0);
	_time_add_msec_to_ticks(&tick_ptr,PERIODIC_PERIOD);
	*(_timer_id * )data_ptr = _timer_start_periodic_every_ticks(PeriodicallyAddTask,0,TIMER_ELAPSED_TIME_MODE, &tick_ptr);
}

void generator_task(os_task_param_t task_init_data)
{
	// TODO: Don't naively generate tasks
	// TODO: Create a timer for each periodic task
	println("GB");
	// Create a Active task-list and an Overdue task list (empty)
	TASK_NODE_PTR active_tasks_head_ptr  = NULL;
	TASK_NODE_PTR overdue_tasks_head_ptr = NULL;

	// Start off with some aperiodic user tasks
	dd_tcreate(DD_USER_TASK, 1600, 400);
	//dd_tcreate(DD_USER_TASK, 500, 800);
	//dd_tcreate(DD_USER_TASK, 500, 800);


#ifdef WILL_CREATE_PERIODIC_TASKS
	// Create a Timer(s) for periodic tasks
	MQX_TICK_STRUCT tick1;
	_timer_id periodic_task_timer;
	_timer_create_component(TIMER_TASK_PRIORITY, TIMER_STACK_SIZE);
	_time_init_ticks(&tick1, 0);
	_time_add_msec_to_ticks(&tick1, PERIODIC_PERIOD);
	periodic_task_timer = _timer_start_periodic_every_ticks(PeriodicallyAddTask,&periodic_task_timer,TIMER_ELAPSED_TIME_MODE, &tick1);
#endif

	println("GE");
	abortme();
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
	println("MB");
	println("ME");
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
	println("IB");

	// Spin forever
	while(1) {}

	println("IE");
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
	println("UB");
	unsigned int executionTime = (unsigned int) task_init_data;
	synthetic_compute_ms(executionTime);
	println("UE");
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
