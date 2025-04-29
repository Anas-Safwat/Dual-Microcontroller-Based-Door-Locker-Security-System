/******************************************************************************
 *
 * Module: PIR Sensor
 *
 * File Name: pir_sensor.h
 *
 * Description: Header file for the PIR (Passive Infrared Sensor) sensor driver
 *
 ******************************************************************************/

#ifndef PIR_SENSOR_H_
#define PIR_SENSOR_H_

#include "../STD_TYPES/std_types.h"
#include "../GPIO/gpio.h" /* To use PORTS and PINS ID's */
#include "avr/io.h" /* To use AVR registers */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PIR_SENSOR_PIN_REG PINC /* Register to read the PIR sensor state */
#define PIR_SENSOR_PORT PORTC_ID /* Port where the PIR sensor is connected */
#define PIR_SENSOR_PIN PIN2_ID /* Pin number where the PIR sensor is connected

/*******************************************************************************
 *                                Function Prototypes                          *
 *******************************************************************************/

/*
 * Description:
 * Initializes the PIR sensor by setting up the corresponding GPIO pin direction.
 */
void PIR_init(void);


/*
 * Description:
 * Reads the state of the PIR sensor and
 * returns whether there is motion detected or not.
 *
 * Returns:
 *    - uint8: LOGIC_HIGH (1) if motion is detected, LOGIC_LOW (0) otherwise.
 */

uint8 PIR_getState(void);

#endif /* PIR_SENSOR_H_ */
