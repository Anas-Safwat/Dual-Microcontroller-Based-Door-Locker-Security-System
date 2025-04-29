 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: header file for BUZZER driver

 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "../GPIO/gpio.h" /* To use setup pin direction functions */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT PORTC_ID
#define BUZZER_PIN  PIN7_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/*
 * Description :
 * Function responsible for initialize the Buzzer driver.
 */
void BUZZER_init(void);


/*
 * Description :
 * Function responsible for enable the buzzer.
 */
void BUZZER_on(void);

/*
 * Description :
 * Function responsible for disable the buzzer.
 */
void BUZZER_off(void);



#endif /* BUZZER_H_ */
