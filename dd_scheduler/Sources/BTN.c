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

bool left_button() {
	return (bool) !GPIO_DRV_ReadPinInput(BTN_Left);
}

bool right_button() {
	return (bool) !GPIO_DRV_ReadPinInput(BTN_Right);
}

bool no_button() {
	return (!right_button() && !left_button());
}
