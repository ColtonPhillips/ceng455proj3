/*
 * MessagePool.h
 *
 *  Created on: Mar 14, 2017
 *      Author: coltonp
 */

#ifndef SOURCES_MESSAGEPOOL_H_
#define SOURCES_MESSAGEPOOL_H_
#include <mqx.h>
#include <message.h>

#define DD_QUEUE 9
#define  TASK_CREATOR_QUEUE 10
#define  TASK_DELETOR_QUEUE 11
#define  ACTIVE_LIST_QUEUE 12
#define  OVERDUE_LIST_QUEUE 13

unsigned int currentTime() {
	TIME_STRUCT ts;
	_time_get_elapsed(&ts);
	return ts.SECONDS * (1000) + ts.MILLISECONDS;
}

// const strings sent as Data
typedef unsigned char * UCHAR_PTR;
const UCHAR_PTR TaskCreatedString = (UCHAR_PTR) "TASK CREATED\n";
const UCHAR_PTR TaskCreatedFailedString = (UCHAR_PTR) "TASK CREATED FAILED\n";

const UCHAR_PTR TaskDeletedString = (UCHAR_PTR) "TASK DELETED\n";
const UCHAR_PTR TaskDeletedFailedString = (UCHAR_PTR) "TASK DELETED FAILED\n";

typedef struct task_list {
	unsigned int tid;
	unsigned int deadline;
	unsigned int task_type;
	unsigned int creation_time;
} TASK_NODE, * TASK_NODE_PTR;

#define DATA_BUFFER_SIZE 64
typedef struct my_messsage
{
	MESSAGE_HEADER_STRUCT HEADER;
	TASK_NODE TASK_DATA;
	unsigned char DATA[DATA_BUFFER_SIZE];
} MESSAGE, * MESSAGE_PTR;

_pool_id message_pool;

_queue_id qopen(_queue_number QNUMBER);
_queue_id qsysopen(_queue_number QNUMBER);
MESSAGE_PTR msgalloc();
TASK_NODE taskListFactory(unsigned int taskid, unsigned int deadline, unsigned int task_type, unsigned int creation_time);
void init_message_pool();
void msgpop(MESSAGE_PTR msg_ptr, _queue_number sourceNumber, _queue_number targetNumber, TASK_NODE task_ptr, unsigned char * data);
void msgsend(MESSAGE_PTR msg_ptr);
void msgpushtask(_queue_number qidSource, _queue_number qidTarget, TASK_NODE task, unsigned char * data);
void msgpushdata(_queue_number qidSource, _queue_number qidTarget, unsigned char * data);
MESSAGE_PTR msgreceive(_queue_number QNUMBER);
MESSAGE_PTR msgreceivetimeout(_queue_number QNUMBER, unsigned int timeout);
bool msgtarget_equals_q(MESSAGE_PTR msg_ptr, _queue_number QNUMBER);
bool msgsrc_equals_q(MESSAGE_PTR msg_ptr, _queue_number QNUMBER);

#endif /* SOURCES_MESSAGEPOOL_H_ */
