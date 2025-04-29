/******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for DC Motor driver

 *******************************************************************************/
#include "dc_motor.h"
#include "../PWM/pwm.h" /* To use PWM mode */



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void DC_MOTOR_init(void){

	/* Setup the DC Motor pins direction */
	GPIO_setupPinDirection(DC_MOTOR_PIN1_PORT, DC_MOTOR_IN1_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PIN2_PORT, DC_MOTOR_IN2_PIN, PIN_OUTPUT);

	/* Stop the motor at first */
	DC_MOTOR_rotate(STOP, 0);

}


void DC_MOTOR_rotate(DC_MOTOR_state state, uint8 speed){


	/* Set the duty cycle of the PWM */
	PWM_Timer0_Start(speed);

	/* Setup DC Motor pins to rotate clock wise*/
	if(state == CW){
		GPIO_writePin(DC_MOTOR_PIN1_PORT, DC_MOTOR_IN1_PIN, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PIN2_PORT, DC_MOTOR_IN2_PIN, LOGIC_LOW);
	}

	/* Setup DC Motor pins to rotate anti-clock wise*/
	else if(state == ACW){
		GPIO_writePin(DC_MOTOR_PIN1_PORT, DC_MOTOR_IN1_PIN, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PIN2_PORT, DC_MOTOR_IN2_PIN, LOGIC_HIGH);
	}

	/* Setup DC Motor pins to stop */
	else{
		GPIO_writePin(DC_MOTOR_PIN1_PORT, DC_MOTOR_IN1_PIN, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PIN2_PORT, DC_MOTOR_IN2_PIN, LOGIC_LOW);

	}


}
