/*
 * access_functions.c
 *
 *  Created on: Mar 14, 2017
 *      Author: coltonp
 */

#include "access_functions.h"

#define ACCESS_ERROR 0
#define ACCESS_FAIL 0
#define ACCESS_PASS 1

#define __NOPRINT

// Mutex is initialized by the main task
void init_mutex(){
	//Initialize mutex attributes:
		if (_mutatr_init(&mutexattr) != MQX_OK) {
			printf("mutex attr init failed");
			_mqx_exit(0);
		}
		// Initialize Mutex
		if (_mutex_init(&accessmutex,&mutexattr) != MQX_OK) {
			printf("Mutex failed to init.");
			_mqx_exit(0);
		}
}

// Generator creates tasks as does the monitor refresh itself
_task_id dd_tcreate(
		unsigned int template_index,
		unsigned int elapsed_execution,// in  ms
		unsigned int elapsed_deadline) // in  ms
{
	lock(&accessmutex);

	// Open a message queue and create a task
	_queue_id creator_qid  = qopen(TASK_CREATOR_QUEUE);
	_task_id this_task_id = _task_create(0,template_index,elapsed_execution);

#ifndef __NOPRINT

	printTD("\nTASK CREATED",this_task_id);
	printf("E: %u\n",elapsed_execution);
	printf("D: %u\n\n",elapsed_deadline);

#endif

	// Allocate, populate and send a msg
	msgpushtask(
			TASK_CREATOR_QUEUE, // src
			DD_QUEUE,			// target
			taskNodeFactory(	// the new task
				this_task_id,
				elapsed_deadline, // relative
				template_index,
				currentTime()), // creation time
			(unsigned char *)"CR\n"); // The Data

	// Wait for reply at the q above
	MESSAGE_PTR pMsg = msgreceive(TASK_CREATOR_QUEUE);

	// Destroy the Q
	_msgq_close(creator_qid);

	// Check return code status
	bool taskAdded = strsame(pMsg->DATA,TaskCreatedString);

	// free the message
	_msg_free(pMsg);

	unlock(&accessmutex);

	//Returns taskID of created task if task actually added to DD scheduler otherwise error.
 	if (!taskAdded) return ACCESS_ERROR;
	return this_task_id;
}

// User Task, Monitor Task delete themselves using this function
bool dd_delete(unsigned int task_id) {

	lock(&accessmutex);

	// Open a message queue
	_queue_id deletor_qid  = qopen(TASK_DELETOR_QUEUE);

	// Allocate, populate and send a msg (with some unimportant message data)
	msgpushtask(
			TASK_DELETOR_QUEUE, // src
			DD_QUEUE,			// target
			taskNodeFactory(	// the new task
				task_id,
				0,
				0,
				0),
			(unsigned char *)"DEL\n"); // The Data

	// Wait for reply at the q above
	MESSAGE_PTR pMsg = msgreceive(TASK_DELETOR_QUEUE);

	// Destroy the Q
	_msgq_close(deletor_qid);

	// Check return code status
	bool taskDeleted = strsame(pMsg->DATA, TaskDeletedString);

	// free the message
	_msg_free(pMsg);

	unlock(&accessmutex);

	//Returns true or false if the task actually deleted from the DD scheduler
 	if (!taskDeleted) return ACCESS_ERROR;
	return ACCESS_PASS;
}

// The monitor task is interested in the # of running tasks
unsigned int dd_return_active_list(TASK_NODE ** pActive_tasks_head, unsigned int * size) {
	lock(&accessmutex);

	// Open a message queue
	_queue_id active_list_qid  = qopen(ACTIVE_LIST_QUEUE);

	// Allocate, populate and send a msg
	msgpushtask(
				ACTIVE_LIST_QUEUE, // src
				DD_QUEUE,			// target
				taskNodeFactory(	// the new task
					0,
					0,
					0,
					0),
				(unsigned char *)"ACTIVELIST?\n"); // The Data

	// Wait for reply at the q above
	MONITOR_MESSAGE_PTR pMonMsg = monitormsgreceive(ACTIVE_LIST_QUEUE);

	// Destroy the Q
	_msgq_close(active_list_qid);

	// use the message to get the active list and set to the paramater above
	* pActive_tasks_head = pMonMsg->monitor_data.task_list_head;
	* size = pMonMsg->monitor_data.task_list_size;

	// free the message
	_msg_free(pMonMsg);

	unlock(&accessmutex);

	//Returns error or no error
	return ACCESS_PASS;
}

unsigned int dd_return_overdue_list(TASK_NODE ** overdue_tasks_head_ptr, unsigned int * size) {

	return ACCESS_FAIL;
/*
	// Open a message queue
	_queue_id overdue_list_qid  = qopen(OVERDUE_LIST_QUEUE);
	// Allocate, populate and send a msg
	//msgpush(DD_QUEUE, taskListFactory(DD_QUEUE, this_task_id, deadline, template_index, _time_get_hwticks()));

	// Wait for reply at the q above
	MESSAGE_PTR msg_ptr = msgreceive(OVERDUE_LIST_QUEUE);

	// Destroy the Q
	_msgq_close(overdue_list_qid);

	// use the message to get the active list and set to the paramater above

	// free the message
	_msg_free(msg_ptr);

	//Returns error or no error
	return 1;*/
}
