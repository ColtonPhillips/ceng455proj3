/*
 * helper_function.c
 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */
#include "helper_function.h"
#include "BTN.h"
#include <stdio.h>
#include <math.h>

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
