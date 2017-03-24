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

// IS CALLED WHEN TIMER EXPIRES
void timer_callback(_timer_id t, void* dataptr, unsigned int seconds, unsigned int miliseconds){
    (*(bool*)dataptr) = false;
}

// CREATE BUSY-WAIT DELAY FOR A GIVEN DURATION
// NOT CURRENTLY USED
void synthetic_compute_ticks(unsigned int ticks){
	bool flag = true;
	MQX_TICK_STRUCT Ticks;
	_time_init_ticks(&Ticks, ticks);
	_timer_start_oneshot_after_ticks((TIMER_NOTIFICATION_TICK_FPTR)timer_callback, (void *) &flag, TIMER_ELAPSED_TIME_MODE, &Ticks);
	// TIMER ELAPSED MODE ENSURES THE TIMER ONLY OCCURS WHEN THE USER TASK IS ACTUALLY RUNNING
	while (flag){}
}

// CREATE BUSY-WAIT DELAY FOR A GIVEN DURATION
void synthetic_compute_ms(unsigned int ms){
	bool flag = true;
	MQX_TICK_STRUCT Ticks;
	_time_init_ticks(&Ticks, 0);
	_time_add_msec_to_ticks(&Ticks, ms);
	_timer_start_oneshot_after_ticks((TIMER_NOTIFICATION_TICK_FPTR)timer_callback, (void *) &flag, TIMER_ELAPSED_TIME_MODE, &Ticks);
	// TIMER ELAPSED MODE ENSURES THE TIMER ONLY OCCURS WHEN THE USER TASK IS ACTUALLY RUNNING
	while (flag){
		//putchar('*');
	}
}

#define TASK_NODE_ARRAY_SIZE 16
#define NO_TASK 0
int numOfRunningTasks = 0;
// TODO: getActiveList() , getOverDueList()

// Set the entire array of structs' deadlines to zero. (therefore they are not active tasks)
void zeroTaskNodeArray(TASK_NODE_PTR task_node_array) {
	int i;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		task_node_array[i].deadline = NO_TASK;
	}
}

// Insert into the first spot with deadline == NO_TASK 		O(n)
bool insertIntoTaskList(TASK_NODE_PTR task_list_array, TASK_NODE insertedTask) {
	// todo: keep track of length of list being used.
	int i = 0;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		if (task_list_array[i].deadline == NO_TASK) {
			task_list_array[i] = insertedTask;
			numOfRunningTasks++;
			return true;
		}
	}
	return false;
}

// Delete a running task (deadline != NO_TASK) if the tid matches 		O(n)
bool deleteFromTaskList(TASK_NODE_PTR task_list_array, TASK_NODE Task) {
	int i = 0;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		if (task_list_array[i].tid == Task.tid && task_list_array[i].deadline != NO_TASK) {
			task_list_array[i].deadline = NO_TASK;
			task_list_array[i].tid = NO_TASK;
			numOfRunningTasks--;
			return true;
		}
	}
	return false;
}

// Find the earliest Deadline within the whole array and set the pointer in argument 0		O(n)
void refreshEarliestDeadlineTask(TASK_NODE_PTR * ppEDF, TASK_NODE_PTR task_list_array) {
	unsigned int DL_min = 999999999; // sufficiently large enough number
	*ppEDF = NULL; // return null if the task list is empty
	int i = 0;
	//printf("\n");
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		if (task_list_array[i].deadline < DL_min && task_list_array[i].deadline != NO_TASK) {
			DL_min = task_list_array[i].deadline;
			*ppEDF = (TASK_NODE_PTR) &(task_list_array[i]);
//			printf("%d ", DL_min);
		}
	}
//	printf("\nWORK?:%d\n",(*ppEDF)->deadline);
}

// This is called during scheduling points on tasks to update creation time to now and deadline to be smaller than before.
void updateNodeWithRespectToTime(TASK_NODE_PTR tn_ptr) {
	// TODO: I feel like this is buggy if deadline becomes a negative or zero in some corner cases such as when two tasks are running
	unsigned int ct = currentTime();
	unsigned int delta = ct - tn_ptr->creation_time; // how much time has passed since last time it was updated
	// Deadline is reduced by the amount of time elapsed since the time it was last 'created' (or updated)
	tn_ptr->deadline = tn_ptr->deadline - delta;
	// we just reuse creation time to make things simple for us.
	tn_ptr->creation_time = ct;
}

TASK_NODE_PTR getActiveTaskHeadPtr(TASK_NODE_PTR task_node_array) {
	TASK_NODE_PTR active_task_node_array = _mem_alloc(numOfRunningTasks * sizeof(TASK_NODE));
	int current_active_task_node = 0;
	int i;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		if (task_node_array->deadline != NO_TASK) {
			active_task_node_array[current_active_task_node] = task_node_array[i];
			current_active_task_node++;
		}
	}
	return active_task_node_array;
}

void printActiveTasksPriorites(TASK_NODE_PTR task_node_array) {
	int i;
	for (i = 0; i < TASK_NODE_ARRAY_SIZE; i++) {
		if (task_node_array[i].deadline != NO_TASK) {
			_mqx_uint priority;
			_task_get_priority(task_node_array[i].tid, &priority);
			printf("T %d   D %d  Actual_DL %d    ",(int)task_node_array[i].tid, (int)task_node_array[i].deadline,(int) priority);
		}
	}
	printf("\n");
}

#define LOW_USER_TASK_PRIORITY 25
#define SCHEDULED_USER_TASK_PRIORITY 18
void dd_scheduler_task(os_task_param_t task_init_data)
{
   // Open a Q for the DDscheduler to use, then lower it's priority (from 11) to 15
	_queue_id dd_qid = qopen(DD_QUEUE);
///	printTDNoBlock("SCHEDULER BEGIN",_task_get_id());
	priorityset(15); // PRIORITY IS NOW BELOW THE GENERATOR

	// BLOCKS UNTIL GENERATOR BLOCKS WHEN IT SENDS A MESSAGE
//	printTDNoBlock("SCHEDULER START",_task_get_id());

	// This starts out with an infinite timeout until it gets it's first task!
	unsigned int deadlineTimeout = 0;

	// Allocate the Task Node Array buffer of a fixed size and set it all to zero (deadline) aka no active task in list
	TASK_NODE_PTR task_node_array = _mem_alloc(TASK_NODE_ARRAY_SIZE * sizeof(TASK_NODE));
	zeroTaskNodeArray(task_node_array);

	// The currently executing task is stored here as a pointer and is NULL if there is no EDF (aka set deadlineTimeout to 0)
	TASK_NODE_PTR earliestDeadlineTask = NULL;


	while (1) {
		// Wait for deadlineTimeout ms for a message (forever if =0)
		if (earliestDeadlineTask != NULL) {
			printf("RUNNING TASKS:%d\n", numOfRunningTasks);
			printf("DEADLINE:%d\n",(int) deadlineTimeout);
		} else {
		}

		MESSAGE_PTR msg_ptr = msgreceivetimeout(DD_QUEUE, deadlineTimeout);

		// v Handle the message v

		// > Message pointer is not a timeout, and there exists an already running task:
		if (msg_ptr != NULL && earliestDeadlineTask != NULL) {
			// Update EDF so that it's deadline is fewer, and it's 'creation' is later
			updateNodeWithRespectToTime(earliestDeadlineTask);
		}

		// > Handle the deadline timeout case
		if (msg_ptr == NULL) {
			// TODO: add to overdue task

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
				updateNodeWithRespectToTime(earliestDeadlineTask);
				deadlineTimeout = earliestDeadlineTask->deadline;
			}
		}

		// > Handle the dd_tcreate message:
		else if (msgsrc_equals_q(msg_ptr,TASK_CREATOR_QUEUE)) {

			// put the sent task into the task list
			bool success = insertIntoTaskList(task_node_array, msg_ptr->TASK_DATA);

			// We are done using the message now
			_msg_free(msg_ptr);

			// Make a copy reference to the value of EDF (Null if no active task) before EDF is recalculated
			TASK_NODE_PTR prevEDFTask = earliestDeadlineTask;

			// Since a task is added we have to refresh the EDF pointer in case the new task is high priority

			refreshEarliestDeadlineTask(&earliestDeadlineTask, task_node_array);
			TASK_NODE tnp0 = task_node_array[0];
			TASK_NODE tnp1 = task_node_array[1];
			TASK_NODE tnp2 = task_node_array[2];
			TASK_NODE tnp3 = task_node_array[3];
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
				updateNodeWithRespectToTime(earliestDeadlineTask);
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
		else if (msgsrc_equals_q(msg_ptr,TASK_DELETOR_QUEUE)) {

			// Delete the sent task from the active running tasks
			bool success = deleteFromTaskList(task_node_array, msg_ptr->TASK_DATA);

			// We are done using the message now
			_msg_free(msg_ptr);

			// Since a task is removed from the array, we refresh the EDF (EDF = NULL if no active tasks exist)
			refreshEarliestDeadlineTask(&earliestDeadlineTask, task_node_array);

			// If successful deletion and an EDF now exists, set it to priority 18
			if (success && earliestDeadlineTask != NULL) {
				prioritysettask(earliestDeadlineTask->tid,SCHEDULED_USER_TASK_PRIORITY);
//				println("DEL RESCHEDULE");
			}

			// If an active task is available, update it's deadline and creation time, and set deadlineTimeout to its deadline
			if (earliestDeadlineTask != NULL) {
				updateNodeWithRespectToTime(earliestDeadlineTask);
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

		else if (msgsrc_equals_q(msg_ptr,ACTIVE_LIST_QUEUE)) {

//			printf("%s", msg_ptr->DATA);

			// We are done using the message now
			_msg_free(msg_ptr);

			// get new memory of the for the list of tasks running:
			unsigned int activeTaskListSize = numOfRunningTasks;
			TASK_NODE_PTR activeTaskListHead = getActiveTaskHeadPtr(task_node_array);

			// the EDF is surely the monitor task: update the deadlineTimeout
			updateNodeWithRespectToTime(earliestDeadlineTask);
			deadlineTimeout = earliestDeadlineTask->deadline;
			refreshEarliestDeadlineTask(&earliestDeadlineTask, task_node_array);

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

		else if (msgsrc_equals_q(msg_ptr,OVERDUE_LIST_QUEUE)) {
			//Copy a new chunk of memory the size of the active array
			//
		}


		printActiveTasksPriorites(task_node_array);
	}
	_msgq_close(dd_qid);
//	println("DE");
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
	//println("Periodic");

	dd_tcreate(DD_USER_TASK, PERIODIC_EXECUTION, PERIODIC_PERIOD);

	_time_init_ticks(tick_ptr,0);
	_time_add_msec_to_ticks(tick_ptr,PERIODIC_PERIOD);
	*(_timer_id * )data_ptr = _timer_start_periodic_every_ticks(PeriodicallyAddTask,0,TIMER_KERNEL_TIME_MODE, tick_ptr);
}

#define MONITORING_PERIOD 5000
/*
typedef struct my_periodic_task {
	unsigned int execution;
	unsigned int deadline;
} PERIODIC_TASK, * PERIODIC_TASK;

*/

void generator_task(os_task_param_t task_init_data)
{
	// TODO: Create a timer for each periodic task instead of just one periodic task
	//printTDNoBlock("GENERATOR BEGIN",_task_get_id());

	// Start off with some aperiodic user tasks
	t1 = dd_tcreate(DD_USER_TASK, 100, 60000);
	t2 = dd_tcreate(DD_USER_TASK, 500, 50000);
	t3 = dd_tcreate(DD_USER_TASK, 500, 30000);

	// Create a monitor task.
	//dd_tcreate(DD_MONITOR_TASK,0,MONITORING_PERIOD);


#ifdef WILL_CREATE_PERIODIC_TASKS
//	struct task_def c[10] = {{1,2,4,5}, {1,3,5}}

	// Create a Timer(s) for periodic tasks
	MQX_TICK_STRUCT tick1;
	_timer_id periodic_task_timer;
	_timer_create_component(TIMER_TASK_PRIORITY, TIMER_STACK_SIZE);
	_time_init_ticks(&tick1, 0);
	_time_add_msec_to_ticks(&tick1, PERIODIC_PERIOD);
	periodic_task_timer = _timer_start_periodic_every_ticks(PeriodicallyAddTask,&periodic_task_timer,TIMER_KERNEL_TIME_MODE, &tick1);
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
	println("MB");

	// Get Overdue and Active Tasks
	TASK_NODE_PTR active_tasks_head_ptr  = NULL;
	TASK_NODE_PTR overdue_tasks_head_ptr = NULL;
	unsigned int active_size = 0;
	unsigned int overdue_size = 0;
	dd_return_active_list(&active_tasks_head_ptr, &active_size);
	dd_return_overdue_list(&overdue_tasks_head_ptr, &overdue_size);



	// Set Red Light if any tasks overdue
	if (overdue_tasks_head_ptr != NULL) {
		red_light();
	}

	if (active_tasks_head_ptr != NULL) {
		blue_light();
	}

	if (active_size == 4) {
		green_light();
	}

	// Free the memory my dude.
	int i;
	for (i = 0; i < active_size; i++) {
		_mem_free(&(active_tasks_head_ptr[i]));
	}
	for (i = 0; i < overdue_size; i++) {
		_mem_free(&(overdue_tasks_head_ptr[i]));
	}

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
	//printTD("IDLE BEGIN",_task_get_id());

	// Spin forever
	while(1) {
		//putchar('.');
	}

	//printTD("IDLE ENDED",_task_get_id());
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
