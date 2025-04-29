/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: Header file for the PWM driver (Timer0)
 *
 ******************************************************************************/

#ifndef PWM_H_
#define PWM_H_

#include "../STD_TYPES/std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Starts PWM signal generation on Timer0 with the specified duty cycle.
 *
 * Parameters:
 *    - duty_cycle: The PWM duty cycle percentage (0% to 100%).
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
