/******************************************************************************
 *
 * File Name: CONTROL_ECU.c
 *
 * Description: Source file for the CONTROL ECU Application
 *
 *******************************************************************************/

#include "Drivers/UART/uart.h"
#include "Drivers/I2C/twi.h"
#include "Drivers/EXTERNAL_EEPROM/external_eeprom.h"
#include "Drivers/DC_MOTOR/dc_motor.h"
#include "Drivers/BUZZER/buzzer.h"
#include "Drivers/PIR_SENSOR/pir_sensor.h"
#include "Drivers/TIMER/timer.h"
#include "Drivers/STD_TYPES/std_types.h"
#include <util/delay.h>
#include <string.h>

/*************************************************************************************************
 *                                   Preprocessor Directives                                     *
 *************************************************************************************************/

#define PASS_LOAD        0xA0    // Command: Load new password
#define PASS_IN          0xF1    // Command: Verify existing password
#define PASS_UPDATE      0xE0    // Command: Update existing password
#define PASS_CORRECT     0xC0    // Response: Password verified successfully
#define PASS_FAIL        0xF0    // Response: Password verification failed
#define PEOPLE_IN        0xB0    // Response: People detected entering
#define PEOPLE_NO        0xD0    // Response: No people detected
#define ALARM_ON         0xF2    // Command: Activate alarm
#define START_ADDRESS    0x000   // Starting EEPROM address for password storage
#define DOOR_CLOSED      0xF3    // Response: Door closed

uint8 counter = 0;
uint8 PROG_status = 0;
boolean status = FALSE;
boolean peopleIN = FALSE;
uint8 pass[10] = { 0 };
uint8 i = 0;

/*
 * ISR Callback for Timer1: Increments counter variable with each timer interrupt.
 */
void Timer1_handler(void) {
    ++counter;
}

int main() {

    // UART Configuration and Initialization
    UART_ConfigType uart_cfg = { UART_8_BIT, UART_PARITY_DISABLED,
            UART_1_STOP_BIT, 9600 };
    UART_init(&uart_cfg);

    // Buzzer Initialization
    BUZZER_init();

    // I2C Configuration and Initialization
    TWI_ConfigType i2c_cfg = { 0x01, 2, TWI_PRE_1 };
    TWI_init(&i2c_cfg);

    // Motor Initialization
    DC_MOTOR_init();

    // PIR Sensor Initialization
    PIR_init();

    // Timer1 Configuration and Initialization for timeout tracking
    SREG |= (1<<7);  // Enable global interrupts
    Timer_ConfigType timer1 = { 0, 31250, TIMER1_ID, TIMER_PRESCALE_256,
            TIMER_COMPARE_MODE };
    Timer_init(&timer1);
    Timer_setCallBack(Timer1_handler, TIMER1_ID);


    while(1) {
        // Listen for program command from UART
        PROG_status = UART_receiveByte();

        // Handling password creation and verification process
        if (PROG_status == PASS_LOAD) {

            // Receive new password (10 bytes: 5 for password and 5 for confirmation)
            for (i = 0; i < 10; ++i) {
                pass[i] = UART_receiveByte();
            }

            // Verify if the new password matches the confirmation input
            for (i = 0; i < 5; ++i) {
                if (pass[i] != pass[i + 5]) {
                    // Notify HMI of mismatch in password confirmation
                    UART_sendByte(PASS_FAIL);
                    break;
                }
            }

            if (i == 5) {  // Password successfully matched
                UART_sendByte(PASS_CORRECT);  // Notify HMI of successful match

                // Save new password to EEPROM
                for (i = 0; i < 5; ++i) {
                    status = EEPROM_writeByte(START_ADDRESS + i, pass[i]);
                    _delay_ms(10); // Delay for EEPROM write
                }
            }
        } else if (PROG_status == PASS_IN) {
            // Receive and validate user-entered password

            for (i = 0; i < 5; ++i) {
                pass[i] = UART_receiveByte();
            }

            // Retrieve stored password from EEPROM for comparison
            for (i = 5; i < 10; ++i) {
                status = EEPROM_readByte(START_ADDRESS + i - 5, &pass[i]);
                _delay_ms(10); // Delay for EEPROM read
            }

            // Compare entered password with stored password
            for (i = 0; i < 5; ++i) {
                if (pass[i] != pass[i + 5]) {
                    UART_sendByte(PASS_FAIL);  // Notify HMI of failure
                    break;
                }
            }

            if (i == 5) {
                UART_sendByte(PASS_CORRECT);  // Password verification success

                // Open door for 15 seconds
                DC_MOTOR_rotate(CW, 100);
                counter = 0;
                while (counter < 5);

                // Stop the motor (door closed)
                DC_MOTOR_rotate(STOP, 0);

                // Check for any further people entering
                UART_sendByte(PEOPLE_IN);
                _delay_ms(500);
                do {
                    peopleIN = PIR_getState();
                } while (peopleIN);

                // Begin door closure sequence
                UART_sendByte(PEOPLE_NO);
                counter = 0;
                DC_MOTOR_rotate(ACW, 100);
                while (counter < 5);
                DC_MOTOR_rotate(STOP, 0);
                UART_sendByte(DOOR_CLOSED);
            }
        } else if (PROG_status == PASS_UPDATE) {
            // Handle password update

            for (i = 0; i < 5; ++i) {
                pass[i] = UART_receiveByte();
            }

            // Retrieve stored password for verification
            for (i = 5; i < 10; ++i) {
                status = EEPROM_readByte(START_ADDRESS + i - 5, &pass[i]);
                _delay_ms(10);
            }

            // Verify current password before updating
            for (i = 0; i < 5; ++i) {
                if (pass[i] != pass[i + 5]) {
                    UART_sendByte(PASS_FAIL);  // Current password mismatch
                    break;
                }
            }

            if (i == 5) {  // Password verified for update
                UART_sendByte(PASS_CORRECT);

                // Receive new password
                for (i = 0; i < 10; ++i) {
                    pass[i] = UART_receiveByte();
                }

                // Verify new password confirmation
                for (i = 0; i < 5; ++i) {
                    if (pass[i] != pass[i + 5]) {
                        UART_sendByte(PASS_FAIL);
                        break;
                    }
                }

                if (i == 5) {  // New password confirmed
                    UART_sendByte(PASS_CORRECT);

                    // Save new password to EEPROM
                    for (i = 0; i < 5; ++i) {
                        status = EEPROM_writeByte(START_ADDRESS + i, pass[i]);
                        _delay_ms(10);
                    }
                }
            }
        } else if (PROG_status == ALARM_ON) {
            // Activate alarm for a duration of 60 seconds
            BUZZER_on();
            counter = 0;
            while (counter < 60);
            BUZZER_off();
        }
    }
}
