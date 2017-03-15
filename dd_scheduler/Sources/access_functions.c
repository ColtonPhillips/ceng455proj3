/*
 * access_functions.c
 *
 *  Created on: Mar 14, 2017
 *      Author: coltonp
 */

#include "access_functions.h"
#include "MessagePool.h"

_task_id dd_tcreate(unsigned int template_index, unsigned int deadline) {

	// Open a message queue
	_queue_id creator_qid  = qopen(TASK_CREATOR_QUEUE);

	// Create a new task of the index and deadline
	_task_id this_task_id = _task_create(0,template_index,deadline);

	// Set task priority to that of the minimum (25?) (24?)
	_mqx_uint priority = _task_get_priority(this_task_id,&priority);
	unsigned int newpriority = _sched_get_min_priority(0);
	_task_set_priority(this_task_id,newpriority,&priority);

	// Allocate, populate and send a msg
	msgpush(DD_QUEUE, taskListFactory(DD_QUEUE, this_task_id, deadline, template_index, _time_get_hwticks()));

	// Wait for reply at the q above
	MESSAGE_PTR msg_ptr = msgreceive(TASK_CREATOR_QUEUE);

	// Destroy the Q
	_msgq_close(creator_qid);

	// use the message (check for error?)

	// free the message
	_msg_free(msg_ptr);

	//Returns to the invoking task (todo errors)
	return this_task_id;
}

unsigned int dd_delete(unsigned int task_id) {
	return 1;
}

unsigned int dd_return_active_list(TASK_LIST ** active_tasks_head_ptr) {
	return 1;
}

unsigned int dd_return_overdue_list(TASK_LIST ** overdue_tasks_head_ptr) {
	return 1;
}