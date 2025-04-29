![image](https://github.com/user-attachments/assets/39ee5297-5460-4cb6-99f3-4f357e42eb8c)

# Project Overview
This project is a Dual Microcontroller-Based Door Locker Security System designed to provide a secure and efficient method for controlling access to a door using password authentication. The system is built around two ATmega32 microcontrollers (HMI_ECU and Control_ECU), which communicate via UART to manage operations. The system features enhanced functionality with components like PIR motion sensor, H-bridge motor control, buzzer alerts, and the ability to change the password.

# Key Features
- **Password Protection:** The system securely stores and verifies a password using external **EEPROM.**
- **LCD and Keypad Interface:** A user-friendly LCD and 4x4 keypad allow users to input and manage passwords easily.
- **Motorized Door Control:** The door is unlocked/locked using a motor driven by an H-bridge for precise control.
- **PIR Sensor Integration:** Detects motion to hold the door open while people are entering.
- **Buzzer Alerts:** Activates a buzzer for incorrect password attempts or system alerts.
- **Password Change:** Users can change the system password after successful verification.
- **Security Lock:** After three consecutive failed password attempts, the system locks for one minute to prevent further inputs.

# Hardware Components
- **HMI_ECU:** Connected to LCD, 4x4 Keypad, and UART for communication with Control_ECU.
- **Control_ECU:** Connected to External EEPROM, PIR sensor, Buzzer, H-bridge Motor Driver, and DC Motor.

# System Operation
1. Create a System Password: Users create and confirm a password through the keypad. If the passwords match, they are stored in EEPROM.
2. **Main Options:** The LCD displays main options, such as unlocking the door or changing the password.
3. **Open Door:** Users enter the password, and if correct, the door unlocks. The motor rotates to unlock the door for 15 seconds, and the PIR sensor ensures the door stays open while motion is detected.
4. **Change Password:** Users can change the password after verifying the current one.
5. **Security Lock:** If the password is entered incorrectly three times, the system locks for one minute to prevent further attempts.

# Drivers and Components
- **GPIO Driver:** Used for GPIO control in both ECUs.
- **UART Driver**: Facilitates communication between the two microcontrollers.
- **LCD Driver:** Controls the 2x16 LCD display for system interaction.
- **Keypad Driver:** Reads inputs from the 4x4 keypad.
- **I2C Driver:** Used to communicate with the external EEPROM for password storage.
- **PWM Driver:** Controls the motor speed for door operation.
- **Timer Driver:** Manages timed operations like door unlocking and locking.
- **Buzzer Driver:** Handles buzzer operations for alerts and errors.
- **PIR Driver:** Detects motion near the door to control door operations.

# Conclusion
This project is a significant step in my journey into embedded systems and security solutions. It demonstrates how to integrate various hardware components, implement password-based security, and use UART communication for multi-microcontroller systems. I hope this project serves as a valuable resource for others working on similar embedded system projects.



