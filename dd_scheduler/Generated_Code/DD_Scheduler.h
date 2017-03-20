/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : DD_Scheduler.h
**     Project     : dd_scheduler
**     Processor   : MK64FN1M0VLL12
**     Component   : OS_Task
**     Version     : Component 1.3.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.3.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-19, 15:26, # CodeGen: 23
**     Contents    :
**         Init - osa_status_t DD_Scheduler_Init(void);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file DD_Scheduler.h
** @version 01.00
*/         
/*!
**  @addtogroup DD_Scheduler_module DD_Scheduler module documentation
**  @{
*/         

#ifndef __DD_Scheduler_H
#define __DD_Scheduler_H

/* MODULE DD_Scheduler. */

/* Include inherited beans */
#include "osa1.h"

#include "Cpu.h"

/* DD_Scheduler task name */  
#define DD_SCHEDULER_TASK_NAME         "dd_scheduler"
/* DD_Scheduler task stack size */  
#define DD_SCHEDULER_TASK_STACK_SIZE   10000U
/* DD_Scheduler task priority value */  
#define DD_SCHEDULER_TASK_PRIORITY     11U
/*
** ===================================================================
**     Method      :  Init (component OS_Task)
**
**     Description :
**         The method creates and starts task defined by OS_Task 
**         component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
osa_status_t DD_Scheduler_Init(void);


/* END DD_Scheduler. */

#endif 
/* ifndef __DD_Scheduler_H */
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
