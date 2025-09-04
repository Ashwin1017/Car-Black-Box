/*
 * File:   menu.c
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:08 AM
 */


#include <xc.h>
#include "main.h"
#include "adc.h"
#include "clcd.h"
#include "ext_eeprom.h"
#include "matrix_keypad.h"


char menu1[5][20] = {"View log        ", "Download log       ", "Clear log    ", "Set time       ", "Change password"}; // Define menu options
char menu1_ind = 0; // Current index of the menu selection
char ss = 0;        // State  selection
extern char main_f; // External variable for the main state
extern char menu_f; // External variable for the menu state

void menu(char key) { // Function to display and handle menu navigation
    
    static unsigned int press_time1 = 0, press_time2 = 0; // Counters for button press durations

    if (ss == 0) { // Check if selection state is 0
        clcd_putch(0x7E, LINE1(0)); // Display a highlight character in the first line
        clcd_putch(' ', LINE2(0)); // Clear the second line
    } else {
        clcd_putch(' ', LINE1(0)); // Clear the first line
        clcd_putch(0x7E, LINE2(0)); // Display a highlight character in the second line
    }

    clcd_print(menu1[menu1_ind], LINE1(1)); // Print the current menu option on the first line
    clcd_print(menu1[menu1_ind + 1], LINE2(1)); // Print the next menu option on the second line

    key = read_switches(LEVEL_CHANGE); // Read the key pressed for navigation

    if (key == MK_SW12) { // If the button for moving down is pressed
        press_time1++; // Increment the press duration counter
    } 
    else if (press_time1 > 0 && press_time1 < 500) { // If the button was held down briefly

        if (ss == 0) { // If the selection state is 0
            ss = 1; // Change the state to 1 (highlighting)
        } else if (menu1_ind < 3) { // If not at the bottom of the menu
            menu1_ind++; // Move down the menu
        }
        press_time1 = 0; // Reset the press duration counter
    } 
    else if (press_time1 > 500) { // If the button was held down long enough
        press_time1 = 0; // Reset the press duration counter
        main_f = MENU_ENTER; // Set the main state to MENU_ENTER
        CLEAR_DISP_SCREEN; // Clear the display
        menu_f = menu1_ind + ss; // Store the current menu index in menu_f
    }

    if (key == MK_SW11) { // If the button for moving up is pressed
        press_time2++; // Increment the press duration counter
    } 
    else if (press_time2 > 0 && press_time2 < 500) { // If the button was held down briefly

        if (ss == 1) { // If the selection state is 1
            ss = 0; // Change the state to 0 (no highlighting)
        } else if (menu1_ind > 0) { // If not at the top of the menu
            menu1_ind--; // Move up the menu
        }
        press_time2 = 0; // Reset the press duration counter
    } 
    else if (press_time2 > 500) { // If the button was held down long enough
        press_time2 = 0; // Reset the press duration counter
        CLEAR_DISP_SCREEN; // Clear the display
        main_f = DASHBOARD; // Set the main state to DASHBOARD
    }
}
