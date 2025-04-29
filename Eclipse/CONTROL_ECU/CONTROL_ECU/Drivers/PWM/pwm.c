/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Description: Source file for the PWM driver (Timer0)
 *
 ******************************************************************************/

#include "pwm.h"
#include "../GPIO/gpio.h" /* To configure PWM output pin */
#include "avr/io.h" /* To use AVR Timer registers */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description:
 * Initializes and starts PWM on Timer0 with the given duty cycle.
 *
 * Parameters:
 *    - duty_cycle: The PWM duty cycle percentage (0% to 100%).
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{
	TCNT0 = 0; /* Initialize Timer0 counter register */

	/* Set Output Compare Register (OCR0) based on duty cycle percentage */
	OCR0 = (duty_cycle * 255) / 100;

	/* Configure Timer0:
	 * - Fast PWM Mode (WGM00 & WGM01 = 1)
	 * - Non-inverting mode (COM01 = 1, COM00 = 0)
	 * - Prescaler 64 (CS02 = 0, CS01 = 1, CS00 = 1)
	 */
	TCCR0 |= 0x6B;

	/* Set PB3 (OC0) as output for PWM signal */
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
}
