/*
 * access_functions.c
 *
 *  Created on: Mar 14, 2017
 *      Author: coltonp
 */

#include "access_functions.h"
#include "MessagePool.h"

_task_id dd_tcreate(unsigned int template_index, unsigned int deadline) {
    // Open an message queue
	_queue_id creator_qid  = qopen(TASK_CREATOR_QUEUE);

	// Create A task and set it to min priority
	_task_id this_task_id = _task_create(0,template_index,deadline);
	unsigned int priority = _sched_get_min_priority(0);
	_task_set_priority(this_task_id,0,&priority);

	// Allocate a message
	MESSAGE_PTR create_task_msg_ptr = msgalloc();
	// Populate a message
	create_task_msg_ptr->HEADER.SOURCE_QID = 0;
	create_task_msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, DD_QUEUE);
	create_task_msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + sizeof(TASK_LIST);
	create_task_msg_ptr->TASK_DATA = taskListFactory(this_task_id, deadline, template_index, _time_get_hwticks());

	// Send the message
	if (!_msgq_send(create_task_msg_ptr)) {
		printf("\nCould not send a message\n");
		_task_block();
	}

	// Wait for reply at the q above
	MESSAGE_PTR msg_ptr = _msgq_receive(TASK_CREATOR_QUEUE, 0);
	if (msg_ptr == NULL) {
		printf("\nCould not receive an DD Scheduler message\n");
		_task_block();
	}
	// free the message
	_msg_free(msg_ptr);

	// Destroy the Q
	_msgq_close(creator_qid);

	//Returns to the invoking task
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
