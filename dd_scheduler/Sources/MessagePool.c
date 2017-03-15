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

TASK_LIST_PTR taskListFactory(unsigned int tid, unsigned int deadline, unsigned int task_type, unsigned int creation_time) {
	TASK_LIST *tl, task;
	tl = &task;
	tl->tid = tid;
	tl->deadline = deadline;
	tl->task_type = task_type;
	tl->creation_time = creation_time;
	return tl;
}

void msgpop(MESSAGE_PTR msg_ptr, _queue_id src, _queue_id target, TASK_LIST_PTR task_ptr) {
	msg_ptr->HEADER.SOURCE_QID = 0;
	msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, DD_QUEUE);
	msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + sizeof(TASK_LIST);
	msg_ptr->TASK_DATA = *task_ptr;
}

void msgsend(MESSAGE_PTR msg_ptr) {
	if (!_msgq_send(msg_ptr)) {
		printf("\nCould not send a message\n");
		_task_block();
	}
}

// no need to return message pointer because the memory is not to be freed
void msgpush(int QTYPE, TASK_LIST_PTR task_ptr) {

	// Allocate a message
	MESSAGE_PTR msg_ptr = msgalloc();

	// Populate a message
	msgpop(msg_ptr, 0 ,QTYPE, task_ptr);

	// Send the message
	msgsend(msg_ptr);
}

MESSAGE_PTR msgreceive(int QTYPE) {
	MESSAGE_PTR msg_ptr = _msgq_receive(QTYPE, 0);
	if (msg_ptr == NULL) {
		printf("\nCould not receive an DD Scheduler message\n");
		_task_block();
	}
	return msg_ptr;
}

bool msgtarget_equals_q(MESSAGE_PTR msg_ptr, int QTYPE) {
	return (msg_ptr->HEADER.TARGET_QID == _msgq_get_id(0, QTYPE));
}

bool msgsrc_equals_q(MESSAGE_PTR msg_ptr, int QTYPE) {
	return (msg_ptr->HEADER.SOURCE_QID == _msgq_get_id(0, QTYPE));
}
