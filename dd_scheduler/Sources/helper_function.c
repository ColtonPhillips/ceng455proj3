/*
 * helper_function.c
 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */
#include "helper_function.h"
#include "BTN.h"
#include "mqx.h"
#include <stdio.h>

//#define NOPRINT

void printlnU(unsigned char * str) {
#ifndef NOPRINT
	printf("%s\n",str);
#endif
}

void println(char * str) {
#ifndef NOPRINT
	printf("%s\n",str);
#endif
}

bool asrt(bool b) {
	if (b) {
		return b;
	}
	else {
		println("no good");
		_mqx_exit(1);
	}
	return false;
}

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

/*
#include <timer.h>
// TIME MEASUREMENT CONFIGURATION
#define TM_WALLCLOCK 0
#define TM_TICKS 1
#define TIME_MEASUREMENT TM_WALLCLOCK

// IS CALLED WHEN TIMER EXPIRES
void timer_callback(_timer_id t, void* dataptr, unsigned int seconds, unsigned int miliseconds){
    (*(bool*)dataptr) = false;
}

// CREATE BUSY-WAIT DELAY FOR A GIVEN DURATION
void synthetic_compute(unsigned int mseconds){
    bool flag = true;
	MQX_TICK_STRUCT Ticks;
	_time_init_ticks(&Ticks, 0);
	_time_add_sec_to_ticks(&Ticks, (int)ceil(mseconds / 50.0));
	_timer_start_oneshot_after(timer_callback, &flag, TIMER_KERNEL_TIME_MODE, &Ticks);
}
    // busy wait loop
    while (flag){}
}
 * */
