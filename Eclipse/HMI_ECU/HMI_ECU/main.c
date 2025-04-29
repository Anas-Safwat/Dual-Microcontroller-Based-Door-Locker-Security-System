/******************************************************************************
 *
 * File Name: HMI_ECU.c
 *
 * Description: Source file for the HMI ECU Application
 *
 *******************************************************************************/

#include "Drivers/LCD/lcd.h"
#include "Drivers/KEYPAD/keypad.h"
#include "Drivers/UART/uart.h"
#include "Drivers/TIMER/timer.h"
#include "Drivers/STD_TYPES/std_types.h"
#include <avr/io.h>
#include <util/delay.h>

/*************************************************************************************************
 *                                    Preprocessor Directives                                    *
 *************************************************************************************************/

#define PASS_LOAD        0xA0
#define PASS_IN          0xF1
#define PASS_UPDATE      0xE0
#define PASS_CORRECT     0xC0
#define PASS_FAIL        0xF0
#define PEOPLE_IN        0xB0
#define PEOPLE_NO        0xD0
#define ALARM_ON         0xF2
#define DOOR_CLOSED      0xF3

/*************************************************************************************************/


/* Password storage and status tracking variables */
uint8 pass[10] = { 0 };
uint8 i = 0;
uint8 pass_state = 0;
uint8 key = 0;
uint8 counter = 0;
uint8 enter_fails = 0;
uint8 update_fails = 0;
uint8 people_status = 0;

/* Timer interrupt handler */
void Timer1_handler(void) {
	++counter;  // Increment counter on each timer interrupt
}

int main() {

	/****************************************** Initializations *****************************************/
	/* Initialize LCD */
	LCD_init();

	/* Configure and initialize UART */
	UART_ConfigType uart_cfg = { UART_8_BIT, UART_PARITY_DISABLED,
			UART_1_STOP_BIT, 9600 };
	UART_init(&uart_cfg);

	/* Initialize Timer1 */
	SREG |= (1<<7);  // Enable global interrupts
	Timer_ConfigType time1 = { 0, 31250, TIMER1_ID, TIMER_PRESCALE_256,
			TIMER_COMPARE_MODE };
	Timer_init(&time1);
	Timer_setCallBack(Timer1_handler, TIMER1_ID);


	/* Prompt user to enter password for the first time */
	while (pass_state != PASS_CORRECT) {
		UART_sendByte(PASS_LOAD);
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_moveCursor(1, 0);

		/* Capture and mask password input */
		for (i = 0; i < 5; ++i) {
			pass[i] = KEYPAD_getPressedKey();
			LCD_displayCharacter('*');
			_delay_ms(500);
		}

		while (KEYPAD_getPressedKey() != '=');  // Wait for enter key
		_delay_ms(500);

		/* Prompt user to re-enter password */
		LCD_clearScreen();
		LCD_displayString("Plz re-enter the");
		LCD_displayStringRowColumn(1, 0, "same pass: ");

		for (i = 5; i < 10; ++i) {
			pass[i] = KEYPAD_getPressedKey();
			LCD_displayCharacter('*');
			_delay_ms(500);
		}
		while (KEYPAD_getPressedKey() != '=');  // Confirm entry
		_delay_ms(500);

		/* Transmit password via UART to Control ECU */
		for (i = 0; i < 10; ++i) {
			UART_sendByte(pass[i]);
		}

		/* Verify password */
		pass_state = UART_receiveByte();
		if (pass_state == PASS_FAIL) {
			LCD_clearScreen();
			LCD_displayString("PASSWORD WRONG!!");
			counter = 0;
			while (counter < 1);  // Delay before retry
		}
	}


	while (1) {
		/* Display main menu options */
		LCD_clearScreen();
		LCD_displayString("+ : OPEN DOOR");
		LCD_displayStringRowColumn(1, 0, "- : CHANGE PASS");

		/* Wait for user selection */
		do {
			key = KEYPAD_getPressedKey();
		} while (key != '+' && key != '-');

		_delay_ms(500);

		/* Process user choice */
		if (key == '+')  // Open Door
		{
			UART_sendByte(PASS_IN);
			LCD_clearScreen();
			LCD_displayString("Plz enter old");
			LCD_displayStringRowColumn(1, 0, "pass: ");

			for (i = 0; i < 5; ++i) {
				pass[i] = KEYPAD_getPressedKey();
				LCD_displayCharacter('*');
				_delay_ms(500);
			}
			while (KEYPAD_getPressedKey() != '=');
			_delay_ms(500);

			/* Transmit password */
			for (i = 0; i < 5; ++i) {
				UART_sendByte(pass[i]);
				_delay_ms(10);
			}
			_delay_ms(100);

			/* Receive validation status */
			pass_state = UART_receiveByte();

			if (pass_state == PASS_CORRECT) {
				enter_fails = 0;
				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 1, "Door Unlocking");
				LCD_displayStringRowColumn(1, 3, "Please Wait");
				counter = 0;
				while (counter < 5);  // Allow 15 seconds for entry
				_delay_ms(500);

				/* Check for people entering */
				people_status = UART_receiveByte();
				LCD_clearScreen();
				if (people_status == PEOPLE_IN) {
					LCD_displayStringRowColumn(0, 0, "Wait for People");
					LCD_displayStringRowColumn(1, 3, "to Enter");
				}
				people_status = UART_receiveByte();
				if (people_status == PEOPLE_NO) {
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 2, "Door Locking");
					counter = 0;
					while (counter < 5);  // Allow time for people to exit
				}
				people_status = UART_receiveByte();
			} else if (pass_state == PASS_FAIL) {
				++enter_fails;
				if (enter_fails == 3) {
					/* Lock system for 1 minute */
					UART_sendByte(ALARM_ON);
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 1, "System LOCKED");
					LCD_displayStringRowColumn(1, 0, "Wait for 1 min");
					counter = 0;
					while (counter < 60);  // 1-minute delay
					enter_fails = 0;
				} else {
					/* Incorrect password */
					LCD_clearScreen();
					LCD_displayString("PASSWORD WRONG!!");
					counter = 0;
					while (counter < 1);
				}
			}
		} else if (key == '-')  // Change Password
		{
			UART_sendByte(PASS_UPDATE);
			LCD_clearScreen();
			LCD_displayString("Plz enter old");
			LCD_displayStringRowColumn(1, 0, "pass: ");

			for (i = 0; i < 5; ++i) {
				pass[i] = KEYPAD_getPressedKey();
				LCD_displayCharacter('*');
				_delay_ms(500);
			}
			while (KEYPAD_getPressedKey() != '=');
			_delay_ms(500);

			/* Transmit current password */
			for (i = 0; i < 5; ++i) {
				UART_sendByte(pass[i]);
			}

			/* Verify current password */
			pass_state = UART_receiveByte();
			if (pass_state == PASS_CORRECT) {
				update_fails = 0;
				LCD_clearScreen();
				LCD_displayString("Plz Enter New Pass:");
				LCD_moveCursor(1, 0);

				/* Enter new password */
				for (i = 0; i < 5; ++i) {
					pass[i] = KEYPAD_getPressedKey();
					LCD_displayCharacter('*');
					_delay_ms(500);
				}
				while (KEYPAD_getPressedKey() != '=');
				_delay_ms(500);
				LCD_clearScreen();
				LCD_displayString("Plz re-enter the");
				LCD_displayStringRowColumn(1, 0, "same pass: ");

				/* Re-enter new password for confirmation */
				for (i = 5; i < 10; ++i) {
					pass[i] = KEYPAD_getPressedKey();
					LCD_displayCharacter('*');
					_delay_ms(500);
				}
				while (KEYPAD_getPressedKey() != '=');
				_delay_ms(500);

				/* Transmit new password */
				for (i = 0; i < 10; ++i) {
					UART_sendByte(pass[i]);
				}

				/* Confirm new password */
				pass_state = UART_receiveByte();
				if (pass_state == PASS_FAIL) {
					LCD_clearScreen();
					LCD_displayString("PASSWORD WRONG!!");
					counter = 0;
					while (counter < 1);
				}
			} else if (pass_state == PASS_FAIL) {
				++update_fails;
				if (update_fails == 3) {
					/* Lock system for 1 minute */
					UART_sendByte(ALARM_ON);
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 1, "System LOCKED");
					LCD_displayStringRowColumn(1, 0, "Wait for 1 min");
					counter = 0;
					while (counter < 60);
					update_fails = 0;
				} else {
					/* Incorrect password */
					LCD_clearScreen();
					LCD_displayString("PASSWORD WRONG!!");
					counter = 0;
					while (counter < 1);
				}
			}
		}
	}
	return 0;
}

