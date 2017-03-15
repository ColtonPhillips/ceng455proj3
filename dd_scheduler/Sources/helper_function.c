/*
 * helper_function.c
 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */
#include "helper_function.h"
#include "mqx.h"
#include <stdio.h>

void prntln(char * str) {
	printf("%s\n",str);
}

bool asrt(bool b) {
	if (b) {
		return b;
	}
	else {
		prntln("no good");
		_mqx_exit(1);
	}
	return false;
}

bool asrtmsg(char * str, bool b) {
	if (b) {
		return b;
	}
	else {
		prntln(str);
		_mqx_exit(1);
	}
	return false;
}

void have_fun() {
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
