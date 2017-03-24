/*
 * BTN.c
 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */
#include "GPIO.h"
void init_buttons() {
// maybe do nothing?	GPIO_DRV_GetPinDir()
}

// Check if the player is pressing the left button.
bool in_left_button() {
	return (bool) !GPIO_DRV_ReadPinInput(BTN_Left);
}

// Check if the player is pressing the right button.
bool in_right_button() {
	return (bool) !GPIO_DRV_ReadPinInput(BTN_Right);
}

// Check if the player is pressing neither button
bool in_no_button() {
	return (!in_right_button() && !in_left_button());
}
