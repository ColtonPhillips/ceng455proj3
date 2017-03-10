/*
 * RGB.c
 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */

#include <stdlib.h>
#include "gpio1.h"

void init_RGB_light() {
	// soft blue light indicates gp-io is initialized.
	GPIO_DRV_TogglePinOutput(LED_Blue);
}
//Setting pins turns the light off, oddly:
void red_light_off(){GPIO_DRV_SetPinOutput(LED_Red);}
void green_light_off(){GPIO_DRV_SetPinOutput(LED_Green);}
void blue_light_off(){GPIO_DRV_SetPinOutput(LED_Blue);}
void kill_lights() {
	red_light_off();green_light_off();blue_light_off();
}

//And clearing pins turns the lights on
void red_light(){GPIO_DRV_ClearPinOutput(LED_Red);}
void green_light(){GPIO_DRV_ClearPinOutput(LED_Green);}
void blue_light(){GPIO_DRV_ClearPinOutput(LED_Blue);}
void white_light(){red_light();green_light();blue_light();}

//let's have some fun: Flashing party lights!
int fifty_fifty() {
	return rand() % 2;
}
void party_lights() {
	if (fifty_fifty()) {
		red_light();
	}if (fifty_fifty()) {
		green_light();
	}if (fifty_fifty()) {
		blue_light();
	}
}
