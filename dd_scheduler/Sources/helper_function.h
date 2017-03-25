/*
 * helper_function.h
 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */

#ifndef SOURCES_HELPER_FUNCTION_H_
#define SOURCES_HELPER_FUNCTION_H_
#include "Constants.h"
#include <mqx.h>
#include <mutex.h>
#include <stdio.h>
#include <mqx.h>
#include <math.h>
#include <message.h>
#include <timer.h>
#include <stdbool.h>
#include "GPIO.h"
#include "RGB.h"
#include "BTN.h"
#include "Tasks.h"
#include "access_functions.h"
#include "Priority.h"
#include "MessagePool.h"
void printTD(char * msg,_task_id t);
void printlnUnsigned(unsigned char * str);
void println(char * str);
bool asrt(bool b);
bool asrtmsg(char * str, bool b);
void have_fun();
void spin(unsigned int n);
bool strsame(unsigned char * a, unsigned char * b);
void lock(MUTEX_STRUCT_PTR Paccessmutex);
void unlock(MUTEX_STRUCT_PTR Paccessmutex);
void timer_callback(_timer_id t, void* dataptr, unsigned int seconds, unsigned int miliseconds);
void synthetic_compute_ms(unsigned int ms);
unsigned int currentTime();
void timercreate();
_timer_id startUtilizationTimer(TIMER_NOTIFICATION_TIME_FPTR timerfunction);
#define UTILIZATION_TIMER_LENGTH 4
#endif /* SOURCES_HELPER_FUNCTION_H_ */
