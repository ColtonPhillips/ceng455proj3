/*
 * access_functions.h
 *
 *  Created on: Mar 14, 2017
 *      Author: coltonp
 */

#ifndef SOURCES_ACCESS_FUNCTIONS_H_
#define SOURCES_ACCESS_FUNCTIONS_H_

#include "helper_function.h"
#include "MessagePool.h"
#include <mutex.h>
#include "Tasks.h"
MUTEX_STRUCT accessmutex;
MUTEX_ATTR_STRUCT mutexattr;
struct TASK_NODE;

void init_mutex();


// The following is copied from the Lab 3 Documentation:


// The DD Scheduler uses one data structure to store task lists

/*
 * When a user task is created using dd_tcreate the scheduler must put the task id (and other information)
into the task list and then sort the list.
This primitive, creates a deadline driven scheduled task. It follows the steps outlined below
1. Opens a queue
2. Creates the task specified and assigns it the minimum priority possible
3. Composes a create_task_message and sends it to the DD-scheduler
4. Waits for a reply at the queue it created above
5. Once the reply is received, it obtains it
6. Destroys the queue
7. Returns to the invoking task
template_index is the template index of the task to be created.
deadline is the number of clock ticks to the task's deadline.
It returns the task_id of the created task, or an error. The error is either an MQX task creation error or a
DD-scheduler specific error (to be determined).
 *
 * */
_task_id dd_tcreate(unsigned int template_index, unsigned int execution, unsigned int deadline);

/*
 * When the function dd_delete is called, the specified task is deleted.
This primitive deletes the task specified. It parallels the structure of the dd_tcreate as outlined above.
 *
 * */
bool dd_delete(unsigned int task_id);

/*
 * This primitive requests the DD-scheduler for the list of active tasks and returns this information to the
requestor.
The designer must decide whether the list should be copied and sent to the requestor, or simply a
pointer pointing to the start of the list could suffice. Analyze the alternatives and justify your
implementation choice.
 *
 * */
unsigned int dd_return_active_list(TASK_NODE ** active_tasks_head_ptr, unsigned int * size);

/*
 * This primitive requests the DD-scheduler for the list of overdue tasks and returns this information to the
requestor. Similar in structure as the dd_return_active_list primitive.
 *
 * */
unsigned int dd_return_overdue_list(TASK_NODE ** overdue_tasks_head_ptr , unsigned int * size);

#endif /* SOURCES_ACCESS_FUNCTIONS_H_ */
