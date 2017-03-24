/*
 * helper_function.c
 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */
#include "helper_function.h"
#include "mqx_ksdk.h"

//#define NOPRINT



void printlnU(unsigned char * str) {
#ifndef NOPRINT
	printf("%s\n",str);
  	_time_delay_ticks(10);
#endif
}

void println(char * str) {
#ifndef NOPRINT
	printf("%s\n",(unsigned char * )str);
  	_time_delay_ticks(10);
#endif
}

void printlnNoBlock(char * str) {
#ifndef NOPRINT
	printf("%s\n",(unsigned char * )str);
	fflush(stdout);
  	printf("");printf("");printf("");printf("");printf("");printf("");printf("");printf("");
  	printf("");printf("");printf("");printf("");printf("");printf("");printf("");printf("");
#endif
}

void printTD(char * msg,_task_id t) {
#ifndef NOPRINT
  	printf("%s\n",msg);
  	TD_STRUCT_PTR pTD = _task_get_td(t);
  	printf("ID: %d\n",(int)pTD->TASK_ID);
	_mqx_uint priority;
	_task_get_priority(t, &priority);
  	printf("Priority: %d (%d)\n",(int)prioritygettask(t), (int)priority);
  	printf("\n");
  	_time_delay_ticks(10);
#endif
}

void printTDNoBlock(char * msg,_task_id t) {
#ifndef NOPRINT
  	printf("%s\n",msg);
  	TD_STRUCT_PTR pTD = _task_get_td(t);
  	printf("ID: %d\n",(int)pTD->TASK_ID);
	_mqx_uint priority;
	_task_get_priority(t, &priority);
  	printf("Priority: %d (%d)\n",(int)prioritygettask(t), (int)priority);
  	printf("\n");
	fflush(stdout);
  	printf("");printf("");printf("");printf("");printf("");printf("");printf("");printf("");
  	printf("");printf("");printf("");printf("");printf("");printf("");printf("");printf("");
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

inline void spin(unsigned int n) {
	unsigned int i; char j = 1; char k = 2; char temp = 3;
	for (i = 0; i < n; i++) {
		temp = j;
		j = k;
		k = temp;
	}
}
