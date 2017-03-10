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
void out_red_light_off(){GPIO_DRV_SetPinOutput(LED_Red);}
void out_green_light_off(){GPIO_DRV_SetPinOutput(LED_Green);}
void out_blue_light_off(){GPIO_DRV_SetPinOutput(LED_Blue);}
void out_kill_lights() {
	out_red_light_off();out_green_light_off();out_blue_light_off();
}

//And clearing pins turns the lights on
void out_red_light(){GPIO_DRV_ClearPinOutput(LED_Red);}
void out_green_light(){GPIO_DRV_ClearPinOutput(LED_Green);}
void out_blue_light(){GPIO_DRV_ClearPinOutput(LED_Blue);}
void out_white_light(){out_red_light();out_green_light();out_blue_light();}

//let's have some fun: Flashing party lights!
int fifty_fifty() {
	return rand() % 2;
}
void party_lights() {
	if (fifty_fifty()) {
		out_red_light();
	}if (fifty_fifty()) {
		out_green_light();
	}if (fifty_fifty()) {
		out_blue_light();
	}
}
