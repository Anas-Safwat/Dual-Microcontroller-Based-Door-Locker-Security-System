/******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc_motor.h
 *
 * Description: Header file for DC Motor driver

 *******************************************************************************/


#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "../GPIO/gpio.h"
#include "../STD_TYPES/std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DC_MOTOR_PIN1_PORT		PORTD_ID
#define DC_MOTOR_PIN2_PORT		PORTD_ID

#define DC_MOTOR_IN1_PIN		PIN6_ID
#define DC_MOTOR_IN2_PIN		PIN7_ID
#define DC_MOTOR_EN_PIN    		PIN3_ID


typedef enum{
	CW,ACW,STOP
}DC_MOTOR_state;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void DC_MOTOR_init(void);
void DC_MOTOR_rotate(DC_MOTOR_state state, uint8 speed);


#endif /* DC_MOTOR_H_ */
