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

const int TASK_CREATOR_QUEUE = 0;
const int DD_QUEUE = 1;

typedef struct task_list {
	unsigned int tid;
	unsigned int deadline;
	unsigned int task_type;
	unsigned int creation_time;
} TASK_LIST, * TASK_LIST_PTR;

typedef struct my_messsage
{
	MESSAGE_HEADER_STRUCT HEADER;
	TASK_LIST TASK_DATA;
} MESSAGE, * MESSAGE_PTR;

_pool_id message_pool;
_queue_id dd_qid;
_queue_id qopen();
MESSAGE_PTR msgalloc();
TASK_LIST_PTR taskListFactory();
void init_message_pool();
void msgpop(MESSAGE_PTR msg_ptr, _queue_id src, _queue_id target, TASK_LIST_PTR task_ptr);
void msgsend(MESSAGE_PTR msg_ptr);
void msgpush(int qid, TASK_LIST_PTR task_ptr);
MESSAGE_PTR msgreceive(int QTYPE);
bool msgtarget_equals_q(MESSAGE_PTR msg_ptr, int QTYPE);
bool msgsrc_equals_q(MESSAGE_PTR msg_ptr, int QTYPE);

#endif /* SOURCES_MESSAGEPOOL_H_ */
