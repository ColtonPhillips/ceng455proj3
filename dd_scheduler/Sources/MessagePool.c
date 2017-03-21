/*
 * MessagePool.c
 *
 *  Created on: Mar 14, 2017
 *      Author: coltonp
 */
#include "MessagePool.h"
#include <stdio.h>

// helper function to open a Q
_queue_id qopen(_queue_number QNUMBER) {
	_queue_id qid  = _msgq_open(QNUMBER, 0);
	if (qid == 0) {
	  printf("\nCould not open a private message queue\n");
	  printf("%d\n",(int)_task_get_error());
	}
	return qid;
}

// helper function to allocate a message
MESSAGE_PTR msgalloc() {
	MESSAGE_PTR msg_ptr = (MESSAGE_PTR)_msg_alloc(message_pool);
	if (msg_ptr == NULL) {
		 printf("\nCould not allocate a message\n");
		 _task_block();
	}
	return msg_ptr;
}

/* helper function: create a message pool */
void init_message_pool() {
	   message_pool = _msgpool_create(sizeof(MESSAGE),
		  80, 0, 0);
	   if (message_pool == MSGPOOL_NULL_POOL_ID) {
		  printf("\nCould not create a message pool\n");
		  _task_block();
	   }
}

// helper function to populate a task list in fewer keystrokes.
TASK_NODE taskNodeFactory(
		unsigned int taskid,
		unsigned int deadline,
		unsigned int task_type,
		unsigned int creation_time)
{
	TASK_NODE tl;
	tl.tid = taskid;
	tl.deadline = deadline;
	tl.task_type = task_type;
	tl.creation_time = creation_time;
	return tl;
}

// helper function to send message which has been set up with a target prior
void msgsend(MESSAGE_PTR msg_ptr) {
	if (!_msgq_send(msg_ptr)) {
		printf("\nCould not send a message\n");
		printf("%d\n",(int)_task_get_error());
		_task_block();
	}
}

// helper function to populate a message in fewer keystrokes (source QNumber, target QNumber, task list, and string msg data)
void msgpop(
		MESSAGE_PTR msg_ptr,
		_queue_number qNumberSource,
		_queue_number qNumberTarget,
		TASK_NODE task,
		unsigned char * data)
{
	msg_ptr->HEADER.SOURCE_QID = _msgq_get_id(0, qNumberSource);
	msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, qNumberTarget);
	msg_ptr->HEADER.SIZE = sizeof(MESSAGE);
	msg_ptr->TASK_DATA = task;
	strcpy(msg_ptr->DATA, (UCHAR_PTR)data);
}

// Push a message to the target qid that includes an actually important and populated task (usually to DD or Monitor)
void msgpushtask(
		_queue_number qNumberSource,
		_queue_number qNumberTarget,
		TASK_NODE task_node,
		unsigned char * data)
{
	// Allocate a message
	MESSAGE_PTR msg_ptr = msgalloc();

	// Populate a message
	msgpop(msg_ptr, qNumberSource, qNumberTarget, task_node, data);

	// Send the message
	msgsend(msg_ptr);
	// no need to return message pointer because
	//the memory is not to be freed by the creator of msg
}

// Just fill the tasklist with anything. It's not important for some messages, like the ones DD sends to access functions.
void msgpushdata(_queue_number qNumberSource, _queue_number qNumberTarget, unsigned char * data) {
	msgpushtask(
		qNumberSource,
		qNumberTarget,
		taskNodeFactory(0, 0, 0, currentTime()),
		data);
}

// helper function to receive message of type QTYPE
MESSAGE_PTR msgreceive(_queue_number QNUMBER) {
	MESSAGE_PTR msg_ptr = _msgq_receive(_msgq_get_id(0,QNUMBER), 0);
	if (msg_ptr == NULL) {
		printf("\nCould not receive a message\n");
		printf("%d\n",(int)_task_get_error());
		_task_block();
	}
	return msg_ptr;
}

// Receive with a ms value as a timeout argument
MESSAGE_PTR msgreceivetimeout(_queue_number QNUMBER, unsigned int timeout){
	MESSAGE_PTR msg_ptr = _msgq_receive(_msgq_get_id(0,QNUMBER), timeout);
	if (msg_ptr == NULL) {
		// TODO: Check if the NULL is an error or a timeout. We're assuming its a timeout. :/
		println("MSGTIMEOUT");
	}
	return msg_ptr;
}

// helper function to see if a msg ptr has same target qid as QTYPE
bool msgtarget_equals_q(MESSAGE_PTR msg_ptr, _queue_number QNUMBER) {
	return (msg_ptr->HEADER.TARGET_QID == _msgq_get_id(0, QNUMBER));
}

// helper function to see if a msg ptr has same source qid as QTYPE
bool msgsrc_equals_q(MESSAGE_PTR msg_ptr, _queue_number QNUMBER) {
	return (msg_ptr->HEADER.SOURCE_QID == _msgq_get_id(0, QNUMBER));
}
