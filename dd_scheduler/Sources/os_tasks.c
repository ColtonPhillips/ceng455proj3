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
#include "RGB.h"
/*
** ===================================================================
**     Callback    : dd_scheduler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
const int TASK_ARRAY_SIZE = 1024;
const int NO_TASK = 0;
void insertIntoTaskList(TASK_LIST_PTR * task_list_array, TASK_LIST_PTR insertedTask) {
	// todo: keep track of length of list being used.
	int i = 0;
	for (i = 0; i < TASK_ARRAY_SIZE; i++) {
		if (task_list_array[i]->deadline == NO_TASK) {
			// insert it here
			task_list_array[i] = insertedTask;
		}
	}
}
void dd_scheduler_task(os_task_param_t task_init_data)
{
	prntln("dd_scheduler_task");
	// Create an empty task List array
	TASK_LIST_PTR  task_list_array[TASK_ARRAY_SIZE];
	// Set the entire array of structs' deadlines to zero.
	int i = 0;
	for (i = 0; i < TASK_ARRAY_SIZE; i++) {
		task_list_array[i]->deadline = NO_TASK;
	}

	TASK_LIST_PTR runningTask = NULL;

	// Q is already available! dd_qid

	// Wait for Any Message
	MESSAGE_PTR msg_ptr = _msgq_receive(MSGQ_ANY_QUEUE, 0);
	if (msg_ptr == NULL) {
		printf("\nCould not receive an DD Scheduler message\n");
		_task_block();
	}

	if (msg_ptr->HEADER.TARGET_QID == _msgq_get_id(0, DD_QUEUE)) { // putline
		// put into the task list
		insertIntoTaskList(task_list_array, &msg_ptr->TASK_DATA);
		// pre-empt if possible


		// Allocate a message
		MESSAGE_PTR creator_reply_msg_ptr = (MESSAGE_PTR)_msg_alloc(message_pool);
		if (creator_reply_msg_ptr == NULL) {
		 printf("\nCould not allocate a message\n");
		 _task_block();
		}
		// Populate it
		creator_reply_msg_ptr->HEADER.SOURCE_QID = 0;
		creator_reply_msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, TASK_CREATOR_QUEUE);
		creator_reply_msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + sizeof(_task_id);
		creator_reply_msg_ptr->TASK_DATA = msg_ptr->TASK_DATA;

		// Reply to Task Creator w message
		if (!_msgq_send(creator_reply_msg_ptr)) {
			printf("\nCould not send a message\n");
			_task_block();
		}
		// Free msg_ptr
		_msg_free(msg_ptr);
	}
	while (1) {
	}

	_task_block();
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
void generator_task(os_task_param_t task_init_data)
{
	prntln("generator_task");
	printf("%s", "coltong");
	// Create a Active task-list and an Overdue task list (empty)
	TASK_LIST_PTR active_tasks_head_ptr  = NULL;
	TASK_LIST_PTR overdue_tasks_head_ptr = NULL;
	dd_tcreate(DD_USER_TASK, 200);
	_task_block();
	while (1) {

	}

	_task_block();
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
	prntln("monitor_task");
	while (1) {
	}

	_task_block();
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
	prntln("idle_task");
	while (1) {

	}
	_task_block();
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
	prntln("user_task");
	out_red_light();
	OSA_TimeDelay(400);
	out_green_light();
	while (1) {

	}

	_task_block();
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
