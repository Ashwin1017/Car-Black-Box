/*
 * File:   password.c
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:09 AM
 */


#include <xc.h>
#include "main.h"
#include "adc.h"
#include "clcd.h"
#include "timer0.h"
#include "ext_eeprom.h"
#include "matrix_keypad.h"

extern unsigned char real_pass[5]; 
extern char main_f; 
extern unsigned int sec; 

void init_config1(void) { // Function to initialize configuration
    PEIE = 1; // Enable peripheral interrupts

    ADCON1 = 0x0F; // Configure ADC settings

    init_timer0(); // Initialize Timer0 for timing functions

    GIE = 1; // Enable global interrupts
}

void password(unsigned char key) { // Function to handle password entry
    clcd_print("ENTER_PASSWORD  ", LINE1(0)); // Prompt user to enter password

    unsigned char key1; // Variable to store the current key pressed
    static unsigned char entered_pass[5]; // Array to store entered password
    static unsigned int count = 0; // Counter for entered characters
    static unsigned char attempts = 3; // Remaining attempts for password entry
    static unsigned long int delay = 0; // Delay counter for blinking

    if (attempts > 0) // Check if there are attempts left
    {
        key1 = key; // Assign the current key

        // Blinking underscore for password entry
        if (count < 4) { // Allow up to 4 characters
            if (delay++ == 50000) { // Blink every 50000 iterations
                clcd_putch('_', LINE2(count)); // Display blinking underscore
            } else if (delay == 100000) { // Turn off underscore after 100000 iterations
                clcd_putch(' ', LINE2(count)); // Clear the underscore
                delay = 0; // Reset delay counter
            }

            if (key1 == MK_SW11) { // If SW11 is pressed
                entered_pass[count] = (0 + '0'); // Store '0' in entered password
                clcd_putch('*', LINE2(count)); // Display '*' for security
                count++; // Move to next character position
            } else if (key1 == MK_SW12) { // If SW12 is pressed
                entered_pass[count] = (1 + '0'); // Store '1' in entered password
                clcd_putch('*', LINE2(count)); // Display '*' for security
                count++; // Move to next character position
            }
        }

        if (count == 4) // Check if 4 characters have been entered
        {
            for (delay = 100000; delay--;); // Delay before checking password
            attempts--; // Decrement remaining attempts
            entered_pass[count] = '\0'; // Null-terminate the entered password string

            if (!my_strcmp(entered_pass, real_pass)) // Compare entered password with stored password
            {
                CLEAR_DISP_SCREEN; // Clear the display
                clcd_print("ENTERED_PASSWORD  ", LINE1(0)); // Show success message
                clcd_print("  SUCCESSFULL  ", LINE2(0)); // Show success message
                for (delay = 500000; delay--;); // Delay before proceeding
                main_f = MENU; // Transition to MENU state
                count = 0; // Reset count for future entries
            } else // If password does not match
            {
                CLEAR_DISP_SCREEN; // Clear the display
                clcd_print("TRY AGAIN", LINE1(0)); // Prompt to try again
                clcd_putch(attempts + '0', LINE2(0)); // Display remaining attempts
                clcd_print("Attempts left", LINE2(2)); // Inform about attempts left
                for (delay = 500000; delay--;); // Delay before resetting
                CLEAR_DISP_SCREEN; // Clear the display again
                clcd_print("ENTER_PASSWORD  ", LINE1(0)); // Prompt for password entry again
                count = 0; // Reset input position
            }
        }
    } else if (attempts == 0) // If no attempts are left
    {
        CLEAR_DISP_SCREEN; // Clear the display
        init_config1(); // Re-initialize configuration
        clcd_print("  USER_BLOCKED  ", LINE1(0)); // Inform user they are blocked
        clcd_print("wait for ", LINE2(0)); // Message to wait
        clcd_print("sec", LINE2(13)); // Message to wait for seconds

        // Display seconds left for blocking
        while (sec > 0) { // Loop while seconds are left
            clcd_putch('0' + (sec / 100), LINE2(9)); // Display hundreds digit
            clcd_putch('0' + ((sec / 10) % 10), LINE2(10)); // Display tens digit
            clcd_putch('0' + (sec % 10), LINE2(11)); // Display units digit
            // Delay or wait until sec is decremented
        }
        CLEAR_DISP_SCREEN; // Clear the display after waiting
        if (sec == 0) { // If time has elapsed
            attempts = 3; // Reset attempts to 3
            main_f = DASHBOARD; // Transition to DASHBOARD state
        }
    }
}

int my_strcmp(char *str1, char *str2) { // Function to compare two strings
    while (*str1 != '\0' && *str2 != '\0') { // Loop until end of both strings
        if (*str1 != *str2) { // Check for differing characters
            return *str1 - *str2; // Return difference if not equal
        }
        str1++; // Move to next character in str1
        str2++; // Move to next character in str2
    }
    // After the loop, if both strings end at the same time
    if (*str1 == '\0' && *str2 == '\0') { // If both strings are equal in length
        return 0; // Return 0 for equality
    }
    // If one string is longer than the other
    return (*str1 - *str2); // Return difference in lengths
}
