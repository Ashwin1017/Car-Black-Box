/*
 * File:   main.c
 * Author: ashwin
 *
 * Created on 25 September, 2024, 8:11 AM
 */


#include <xc.h> // Include XC8 compiler header for PIC microcontrollers
#include "main.h" // Include main application header for global definitions and functions
#include "adc.h" // Include ADC (Analog to Digital Converter) header for ADC functionality
#include "uart.h" // Include UART (Universal Asynchronous Receiver-Transmitter) header for serial communication
#include "clcd.h" // Include CLCD (Character LCD) header for controlling the character LCD
#include "timer0.h" // Include Timer0 header for timer functionality
#include "i2c.h" // Include I2C (Inter-Integrated Circuit) header for I2C communication
#include "ds1307.h" // Include DS1307 header for real-time clock functionality
#include "ext_eeprom.h" // Include external EEPROM header for EEPROM operations
#include "matrix_keypad.h" // Include matrix keypad header for handling keypad input


unsigned char clock_reg[3]; // Array to hold clock registers (hour, minute, second)
unsigned char time[9]; // Array to hold formatted time string
unsigned char event[][3] = {"ON", "GR", "GN", "G1", "G2", "G3", "G4", "G5", "_C","VL","DL","CL","ST","CP"}; // Event labels
unsigned char real_pass[5] = "1111"; // Default password
unsigned char ev_i = 0; // Index for event selection
unsigned char log_c = 0; // Log counter
unsigned char address = 200; // EEPROM address for storing data
unsigned char key; // Variable to hold key press value
char main_f = 0; // Main state variable
char menu_f = 0; // Menu state variable

void init_config() { // Function to initialize all hardware configurations
    init_adc(); // Initialize ADC
    init_uart(); // Initialize UART
    init_clcd(); // Initialize character LCD
    init_i2c(); // Initialize I2C communication
    init_ds1307(); // Initialize DS1307 RTC
    init_matrix_keypad(); // Initialize matrix keypad
}

void get_time(void) { // Function to read and format the current time from DS1307
    clock_reg[0] = read_ds1307(HOUR_ADDR); // Read hour register
    clock_reg[1] = read_ds1307(MIN_ADDR); // Read minute register
    clock_reg[2] = read_ds1307(SEC_ADDR); // Read seconds register

    // Format hour based on 12-hour or 24-hour mode
    if (clock_reg[0] & 0x40) { // Check if it's in 12-hour format
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01); // Get hour tens place
        time[1] = '0' + (clock_reg[0] & 0x0F); // Get hour units place
    } else {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03); // Get hour tens place for 24-hour format
        time[1] = '0' + (clock_reg[0] & 0x0F); // Get hour units place
    }
    time[2] = ':'; // Add colon separator
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F); // Get minute tens place
    time[4] = '0' + (clock_reg[1] & 0x0F); // Get minute units place
    time[5] = ':'; // Add colon separator
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F); // Get second tens place
    time[7] = '0' + (clock_reg[2] & 0x0F); // Get second units place
    time[8] = '\0'; // Null-terminate the string
}

void main(void) { // Main function
    init_config(); // Initialize configurations
    
    for (unsigned int j = 0; j < 4; j++) { // Store the password in EEPROM
        write_ext_eeprom(address, real_pass[j]); // Write each character of the password
        address++; // Increment EEPROM address
    }

    while (1) { // Infinite loop for main program execution
        key = read_switches(STATE_CHANGE); // Read the current key press

        // Handle event selection based on key presses
        if (key == MK_SW1) { // If SW1 is pressed
            ev_i = 8; // Set event index to 8
            store_event(); // Store the event
        }

        if (key == MK_SW2) { // If SW2 is pressed
            if (ev_i < 7) { // Increment index if less than 8
                ev_i++; // Move to the next event
                store_event(); // Store the event
            }
        }

        if (key == MK_SW3) { // If SW3 is pressed
            if (ev_i > 1 && ev_i < 8) { // Decrement index if between 1 and 8
                ev_i--; // Move to the previous event
                store_event(); // Store the event
            }
        }

        if (ev_i == 8) { // If event index is 8
            if (key == MK_SW2 || key == MK_SW3) { // Check for SW2 or SW3 press
                ev_i = 2; // Wrap around to event index 2
                store_event(); // Store the event
            }
        } 

        if (main_f == DASHBOARD) { // If in DASHBOARD state
            dashboard(); // Call dashboard function
            
            if (key == MK_SW11) { // If SW11 is pressed
                CLEAR_DISP_SCREEN; // Clear display
                main_f = PASSWORD; // Transition to PASSWORD state
            }
        }
        else if (main_f == PASSWORD) { // If in PASSWORD state
            password(key); // Call password handling function
        }
        else if (main_f == MENU) { // If in MENU state
            menu(key); // Call menu function
        }
        else if (main_f == MENU_ENTER) { // If entering MENU options
            if (menu_f == VIEWLOG) { // If viewing log
                view_log(key); // Call view log function
            }
            else if (menu_f == DOWNLOADLOG) { // If downloading log
                download_log(); // Call download log function
            }
            else if (menu_f == CLEARLOG) { // If clearing log
                clear_log(key); // Call clear log function
            }
            else if (menu_f == SETTIME) { // If setting time
                set_time(key); // Call set time function
            }
            else if (menu_f == CHANGEPASS) { // If changing password
                change_password(key); // Call change password function
            }
        }
    }
}
