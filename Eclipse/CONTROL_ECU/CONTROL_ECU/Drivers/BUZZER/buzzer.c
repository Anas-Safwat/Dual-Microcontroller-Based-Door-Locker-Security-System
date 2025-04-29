 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: source file for BUZZER driver

 *******************************************************************************/

#include "buzzer.h"

void BUZZER_init(void){

	/* setup the pin direction to be output */
	GPIO_setupPinDirection(BUZZER_PORT,BUZZER_PIN,PIN_OUTPUT);

	/* off buzzer initially*/
	BUZZER_off();

}

void BUZZER_on(void){

	/* write logic high on the pin to enable the buzzer */
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_HIGH);

}

void BUZZER_off(void){

	/* write logic low on the pin to disable the buzzer */
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);

}


