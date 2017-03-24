/*
 * helper_function.c
 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */
#include "helper_function.h"
#include "mqx_ksdk.h"

//#define NOPRINT

// We have a system to print to the console, and shut it off at the "flip of a switch"

// Print an unsigned char * with a newline for debugging purposes
void printlnUnsigned(unsigned char * str) {
#ifndef NOPRINT
	printf("%s\n",str);
#endif
}

// Print a signed char * with a newline for debugging purposes
void println(char * str) {
#ifndef NOPRINT
	printf("%s\n",(unsigned char * )str);
#endif
}

// Print a TD_STRUCT associated to a task id
void printTD(char * msg,_task_id t) {
#ifndef NOPRINT
  	printf("%s\n",msg);
  	TD_STRUCT_PTR pTD = _task_get_td(t);
  	printf("ID: %d\n",(int)pTD->TASK_ID);
  	printf("TEMPLATE INDEX: %d\n",(int)pTD->TEMPLATE_INDEX);
	_mqx_uint priority;
	_task_get_priority(t, &priority);
  	printf("Priority: %d (%d)\n",(int)prioritygettask(t), (int)priority);
  	printf("\n");
#endif
}

// Simulate the assert function for the board and print a message if it fails
bool asrtmsg(char * str, bool b) {
	if (b) {
		return b;
	}
	else {
		println(str);
		_mqx_exit(1);
	}
	return false;
}

// In case we don't want to print a message
bool asrt(bool b) {
	return asrtmsg("No Good!",b);
}

// This is a function to install a fun video game with blinking lights :P
void have_fun() {
	if (in_left_button()) {
		out_red_light();
	}
	if (in_right_button()) {
		out_green_light();
	}
	if (in_no_button()) {
		out_blue_light();
	}
	int r = in_right_button();
	int l = in_left_button();
	printf("  %d , %d : \n",l,r);
}

// Can be used to waste time without blocking.
inline void spin(unsigned int n) {
	unsigned int i; char j = 1; char k = 2; char temp = 3;
	for (i = 0; i < n; i++) {
		temp = j;
		j = k;
		k = temp;
	}
}

// Handy function to test if two strings are identical
bool strsame(unsigned char * a, unsigned char * b) {
	return strcmp((char * ) a, (char * )b) == 0;
}

// handy functions for mutex locking/unlocking
void lock(MUTEX_STRUCT_PTR Paccessmutex) {
	if (_mutex_lock(Paccessmutex) != MQX_OK) {
		printf("Mutex failed to lock.\n");
		_mqx_exit(0);
	}
}
void unlock(MUTEX_STRUCT_PTR Paccessmutex) {
	if (_mutex_unlock(Paccessmutex) != MQX_OK) {
		printf("Mutex failed to unlock.\n");
		_mqx_exit(0);
	}
}

// IS CALLED WHEN TIMER EXPIRES
void timer_callback(_timer_id t, void* dataptr, unsigned int seconds, unsigned int miliseconds){
    (*(bool*)dataptr) = false;
}

// CREATE BUSY-WAIT DELAY FOR A GIVEN DURATION
void synthetic_compute_ms(unsigned int ms){
	bool flag = true;
	MQX_TICK_STRUCT Ticks;
	_time_init_ticks(&Ticks, 0);
	_time_add_msec_to_ticks(&Ticks, ms);
	_timer_start_oneshot_after_ticks((TIMER_NOTIFICATION_TICK_FPTR)timer_callback, (void *) &flag, TIMER_ELAPSED_TIME_MODE, &Ticks);
	// TIMER ELAPSED MODE ENSURES THE TIMER ONLY OCCURS WHEN THE USER TASK IS ACTUALLY RUNNING
	while (flag){
		//putchar('*');
	}
}

// Get current elapsed time in ms
unsigned int currentTime() {
	TIME_STRUCT ts;
	_time_get_elapsed(&ts);
	return (unsigned int)( ts.SECONDS * (1000) + ts.MILLISECONDS);
}
