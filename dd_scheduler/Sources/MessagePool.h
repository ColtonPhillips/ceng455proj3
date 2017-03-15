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
TASK_LIST taskListFactory();
void init_message_pool();



#endif /* SOURCES_MESSAGEPOOL_H_ */
