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
#include "Tasks.h"

// message pool is for task messages
// monitor message pools is for monitor messages
_pool_id message_pool;
_pool_id monitor_message_pool;

_queue_id qopen(_queue_number QNUMBER);
MESSAGE_PTR msgalloc();
MONITOR_MESSAGE_PTR monitormsgalloc();
TASK_NODE taskNodeFactory(unsigned int taskid, unsigned int deadline, unsigned int task_type, unsigned int creation_time);
void init_message_pools();
void msgpop(MESSAGE_PTR msg_ptr, _queue_number sourceNumber, _queue_number targetNumber, TASK_NODE task_ptr, unsigned char * data);
void monitormsgpop(MONITOR_MESSAGE_PTR msg_ptr, _queue_number qNumberSource, _queue_number qNumberTarget, TASK_NODE_PTR taskHead_ptr, unsigned int size, unsigned char * data);
void msgsend(MESSAGE_PTR msg_ptr);
void monitormsgsend(MONITOR_MESSAGE_PTR mon_msg_ptr);
void msgpushtask(_queue_number qidSource, _queue_number qidTarget, TASK_NODE task, unsigned char * data);
void msgpushdata(_queue_number qidSource, _queue_number qidTarget, unsigned char * data);
void monitormsgpush(_queue_number qNumberSource, _queue_number qNumberTarget, TASK_NODE_PTR task_node_ptr, unsigned int size, unsigned char * data);
MESSAGE_PTR msgreceive(_queue_number QNUMBER);
MONITOR_MESSAGE_PTR monitormsgreceive(_queue_number QNUMBER);
MESSAGE_PTR msgreceivetimeout(_queue_number QNUMBER, unsigned int timeout);
bool msgtarget_equals_q(MESSAGE_PTR msg_ptr, _queue_number QNUMBER);
bool msgsrc_equals_q(MESSAGE_PTR msg_ptr, _queue_number QNUMBER);

#endif /* SOURCES_MESSAGEPOOL_H_ */
