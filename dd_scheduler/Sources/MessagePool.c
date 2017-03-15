/*
 * MessagePool.c
 *
 *  Created on: Mar 14, 2017
 *      Author: coltonp
 */
#include "MessagePool.h"
#include <stdio.h>

_queue_id qopen(int QTYPE) {
	_queue_id qid  = _msgq_open(QTYPE, 0);
	if (qid == 0) {
	  printf("\nCould not open a message queue\n");
	  _task_block();
	}
	return qid;
}

MESSAGE_PTR msgalloc() {
	MESSAGE_PTR msg_ptr = (MESSAGE_PTR)_msg_alloc(message_pool);
	if (msg_ptr == NULL) {
		 printf("\nCould not allocate a message\n");
		 _task_block();
	}
	return msg_ptr;
}

void init_message_pool() {
	/* create a message pool */
	   message_pool = _msgpool_create(sizeof(MESSAGE),
		  80, 0, 0);
	   if (message_pool == MSGPOOL_NULL_POOL_ID) {
		  printf("\nCould not create a handler message pool\n");
		  _task_block();
	   }

	   // Open a Q for the DD scheduler to use
	   	_queue_id dd_qid = qopen(DD_QUEUE);
}

TASK_LIST taskListFactory(unsigned int tid, unsigned int deadline, unsigned int task_type, unsigned int creation_time) {
	TASK_LIST tl;
	tl.tid = tid;
	tl.deadline = deadline;
	tl.task_type = task_type;
	tl.creation_time = creation_time;
	return tl;
}
