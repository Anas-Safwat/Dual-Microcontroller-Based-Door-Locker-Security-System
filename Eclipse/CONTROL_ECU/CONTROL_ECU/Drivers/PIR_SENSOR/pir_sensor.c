/******************************************************************************
 *
 * Module: PIR Sensor
 *
 * File Name: pir_sensor.c
 *
 * Description: Source file for the PIR sensor driver
 *
 ******************************************************************************/

#include "pir_sensor.h"
#include "../COMMON_MACROS/common_macros.h" /* To use macros like BIT_IS_SET */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Description:
 * Initializes the PIR sensor by configuring the GPIO pin as an output.
 */
void PIR_init(void){
	GPIO_setupPinDirection(PIR_SENSOR_PORT, PIR_SENSOR_PIN, PIN_OUTPUT);
}


/*
 * Description:
 * Reads the current state of the PIR sensor and determines if motion is detected.
 *
 * Returns:
 *    - uint8: LOGIC_HIGH (1) if motion is detected, LOGIC_LOW (0) otherwise.
 */

uint8 PIR_getState(void){
	uint8 motion_value;

	/* Check if the PIR sensor pin is set (motion detected) */
	if(BIT_IS_SET(PIR_SENSOR_PIN_REG, PIR_SENSOR_PIN)){
		motion_value = LOGIC_HIGH;
	}
	else{
		motion_value = LOGIC_LOW;
	}

	return motion_value;
}
