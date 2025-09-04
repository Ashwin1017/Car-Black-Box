/*
 * File:   change_password.c
 * Author: ashwi
 *
 * Created on 3 October, 2024, 8:43 AM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "ext_eeprom.h"

extern unsigned char real_pass[5]; // External array for the real password
extern unsigned char ev_i; // External variable for event index

extern char main_f; // External variable for main state flag

void change_password(char key) {
    static unsigned char i = 0, j = 0, k = 0; // Counters for password entry
    static unsigned char enter_pass[5], new_pass[5], re_enter_pass[5]; // Arrays for passwords
    static unsigned long int delay; // Delay counter for blinking

    // Display prompt for entering the current password
    clcd_print("Enter Cnt Pwd:", LINE1(0));

    // Loop to enter the old password
    while (i < 4) {
        key = read_switches(STATE_CHANGE); // Read current switch state
        
        // Blinking underscore for password entry
        if (delay++ == 50000) {
            clcd_putch('_', LINE2(i)); // Display underscore
        } else if (delay == 100000) {
            clcd_putch(' ', LINE2(i)); // Clear underscore
            delay = 0; // Reset delay
        }

        // Check for key presses to store the entered password
        if (MK_SW11 == key) {
            enter_pass[i] = '0'; // Store '0' for key press
            clcd_putch('*', LINE2(i)); // Display asterisks for security
            i++; // Increment index
        } 
        else if (MK_SW12 == key) {
            enter_pass[i] = '1'; // Store '1' for key press
            clcd_putch('*', LINE2(i)); // Display asterisks for security
            i++; // Increment index
        }
    }

    enter_pass[4] = '\0'; // Null-terminate the entered password

    // Check if the entered old password matches the real password
    if (!my_strcmp(enter_pass, real_pass)) {
        // Prompt for new password
        CLEAR_DISP_SCREEN; // Clear the display
        clcd_print("Enter New Pwd:", LINE1(0)); // Display prompt for new password
        j = 0; // Reset index for new password input

        // Loop to enter the new password
        while (j < 4) {
            key = read_switches(STATE_CHANGE); // Read current switch state
            
            // Blinking underscore for new password entry
            if (delay++ == 50000) {
                clcd_putch('_', LINE2(j)); // Display underscore
            } else if (delay == 100000) {
                clcd_putch(' ', LINE2(j)); // Clear underscore
                delay = 0; // Reset delay
            }

            // Check for key presses to store the new password
            if (MK_SW11 == key) {
                new_pass[j] = '0'; // Store '0' for key press
                clcd_putch('*', LINE2(j)); // Display asterisks for security
                j++; // Increment index
            } 
            else if (MK_SW12 == key) {
                new_pass[j] = '1'; // Store '1' for key press
                clcd_putch('*', LINE2(j)); // Display asterisks for security
                j++; // Increment index
            }
        }

        new_pass[4] = '\0'; // Null-terminate the new password

        // Prompt to re-enter the new password
        CLEAR_DISP_SCREEN; // Clear the display
        clcd_print("Re-enter Pwd:", LINE1(0)); // Display prompt for re-entering password
        k = 0; // Reset index for re-entering password

        // Loop to re-enter the new password
        while (k < 4) {
            key = read_switches(STATE_CHANGE); // Read current switch state
            
            // Blinking underscore for re-entering password
            if (delay++ == 50000) {
                clcd_putch('_', LINE2(k)); // Display underscore
            } else if (delay == 100000) {
                clcd_putch(' ', LINE2(k)); // Clear underscore
                delay = 0; // Reset delay
            }

            // Check for key presses to store the re-entered password
            if (MK_SW11 == key) {
                re_enter_pass[k] = '0'; // Store '0' for key press
                clcd_putch('*', LINE2(k)); // Display asterisks for security
                k++; // Increment index
            } 
            else if (MK_SW12 == key) {
                re_enter_pass[k] = '1'; // Store '1' for key press
                clcd_putch('*', LINE2(k)); // Display asterisks for security
                k++; // Increment index
            }
        }

        re_enter_pass[4] = '\0'; // Null-terminate the re-entered password

        // Check if the re-entered password matches the new password
        if (!my_strcmp(re_enter_pass, new_pass)) {
            CLEAR_DISP_SCREEN; // Clear the display
            clcd_print("Password changed", LINE1(0)); // Display success message
            clcd_print("Successful", LINE2(0)); // Display confirmation
            for (unsigned int z = 0; z < 4; z++) {
                real_pass[z] = new_pass[z]; // Update the real password
            }
            for (delay = 500000; delay--;); // Simple delay loop
            // Reset counters
            i = j = k = 0; // Reset all counters
            ev_i = 13; // Set event index
            store_event(); // Store the password change event
            CLEAR_DISP_SCREEN; // Clear the display
            main_f = MENU; // Set flag to return to menu
        } 
        else {
            // Handle password mismatch
            CLEAR_DISP_SCREEN; // Clear the display
            clcd_print("Pass not matched", LINE1(0)); // Display mismatch message
            for (delay = 500000; delay--;); // Simple delay loop
            i = j = k = 0; // Reset counters
            CLEAR_DISP_SCREEN; // Clear the display
            main_f = MENU; // Set flag to return to menu
        }
    } 
    else {
        // Handle incorrect old password case
        CLEAR_DISP_SCREEN; // Clear the display
        clcd_print("Pass incorrect", LINE1(0)); // Display incorrect password message
        for (delay = 500000; delay--;); // Simple delay loop
        i = j = k = 0; // Reset counters
        CLEAR_DISP_SCREEN; // Clear the display
        main_f = MENU; // Set flag to return to menu
    }
}
