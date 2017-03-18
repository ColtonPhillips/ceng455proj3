/* ###################################################################
**     Filename    : rtos_main_task.c
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
**         main_task - void main_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file rtos_main_task.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup rtos_main_task_module rtos_main_task module documentation
**  @{
*/         
/* MODULE rtos_main_task */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
#include "GPIO.h"
#include "RGB.h"
#include "BTN.h"
#include "MessagePool.h"
#include "helper_function.h"
/* Initialization of Processor Expert components function prototype */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
extern void PEX_components_init(void);
#endif 

/*
** ===================================================================
**     Callback    : main_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/

void init_main_task() {
	init_GPIO(); // init the input and output gpios
	init_buttons(); // init the 2 buttons
	init_RGB_light(); // blue light indicates the light system is on.
	init_message_pool(); // Initializes the TASK_LIST message pool and DD q
}

/*
 * Priority Order:
 * 8 MAINTASK_TASK ???
 * 9  DD_GENERATOR_TASK
 * 10 DD_SCHEDULER_TASK
 * 20 DD_USER_TASK
 * 30 DD_IDLE_TASK
 * 40 DD_MONITOR_TASK
*/

void main_task(os_task_param_t task_init_data)
{
  /* Initialization of Processor Expert components (when some RTOS is active). DON'T REMOVE THIS CODE!!! */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
  PEX_components_init(); 
#endif 
  	printf("\x1B[H\x1B[J");
  	println("Main Task Begins:");
  	init_main_task();
  	out_kill_lights();
  	_task_create(0,DD_GENERATOR_TASK,0);
  	_task_create(0,DD_SCHEDULER_TASK,0);
 	 println("Main Task Ends:");
  	_task_abort(_task_get_id());
}

/* END rtos_main_task */

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
