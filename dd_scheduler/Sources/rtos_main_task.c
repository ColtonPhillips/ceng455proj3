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
#include "ACCEL.h"
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
}

void have_fun() {
	//printf("%d %d \n",GPIO_DRV_ReadPinInput(ACCEL_2),GPIO_DRV_ReadPinInput(ACCEL_3));
	if (in_left_button()) {
		out_red_light();
		printf("0");
	}
	if (in_right_button()) {
		out_green_light();
		printf("1");
	}
	if (in_no_button()) {
		out_blue_light();
		printf("2");
	}
	int r = in_right_button();
	int l = in_left_button();
	printf("  %d , %d : \n",l,r);
}

/*
// Priority+Order:
 *
 *
#define MAINTASK_TASK        1U	// P:16
#define DD_IDLE_TASK         4U // P:24
#define DD_SCHEDULER_TASK    2U // P:16
#define DD_MONITOR_TASK      5U // P:16
#define DD_GENERATOR_TASK    6U // P:16
#define DD_USER_TASK         3U // P:24
*/

void main_task(os_task_param_t task_init_data)
{
  /* Initialization of Processor Expert components (when some RTOS is active). DON'T REMOVE THIS CODE!!! */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
  PEX_components_init(); 
#endif 

  	init_main_task();
/*
// Spawn Tasks:
//      DD_IDLE_TASK:   ###
//      DD_SCHEDULER:      ###
//   DD_MONITOR_TASK:         ###
// DD_GENERATOR_TASK:            ###
// DD_USER_TASK( by generator)      ###   ###       ###### ... etc.
//time->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->
 * */
  	_task_create(0,DD_IDLE_TASK,0);
  	_task_create(0,DD_SCHEDULER_TASK,0);
  	_task_create(0,DD_MONITOR_TASK,0);
  	_task_create(0,DD_GENERATOR_TASK,0);

	while (1) {
		if (0) {party_lights();}else{out_kill_lights();}
		//have_fun();
		OSA_TimeDelay(500);
	}
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