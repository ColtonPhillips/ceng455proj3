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
