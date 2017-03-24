/*
 * BTN.c
 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */
#include "gpio1.h"
void init_buttons() {
// maybe do nothing?	GPIO_DRV_GetPinDir()
}

bool in_left_button() {
	return (bool) !GPIO_DRV_ReadPinInput(BTN_Left);
}

bool in_right_button() {
	return (bool) !GPIO_DRV_ReadPinInput(BTN_Right);
}

bool in_no_button() {
	return (!in_right_button() && !in_left_button());
}
