/* ###################################################################
**     Filename    : os_tasks.c
**     Project     : dd_scheduler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-08, 14:24, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         dd_scheduler_task - void dd_scheduler_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         
/* MODULE os_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif 



/* User includes (#include below this line is not maintained by Processor Expert) */
#include "helper_function.h"
/*
** ===================================================================
**     Callback    : dd_scheduler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void dd_scheduler_task(os_task_param_t task_init_data)
{
	prntln("dd_scheduler_task");
	// List of
	while (1) {
		//_task

	}

	_task_block();
}

/*
** ===================================================================
**     Callback    : generator_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void generator_task(os_task_param_t task_init_data)
{
	prntln("generator_task");
	while (1) {

	}

	_task_block();
}

/*
** ===================================================================
**     Callback    : monitor_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void monitor_task(os_task_param_t task_init_data)
{
	prntln("monitor_task");
	while (1) {

	}

	_task_block();
}

/*
** ===================================================================
**     Callback    : idle_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void idle_task(os_task_param_t task_init_data)
{
	prntln("idle_task");
	while (1) {

	}
	_task_block();
}

/*
** ===================================================================
**     Callback    : user_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void user_task(os_task_param_t task_init_data)
{
	prntln("user_task");
	while (1) {

	}

	_task_block();
}

/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
