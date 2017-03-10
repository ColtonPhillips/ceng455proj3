/*
 * GPIO.c

 *
 *  Created on: Mar 10, 2017
 *      Author: coltonp
 */
#include "GPIO.h"
void init_GPIO() {
	GPIO_DRV_Init(gpio1_InpConfig0, gpio1_OutConfig0);
}
