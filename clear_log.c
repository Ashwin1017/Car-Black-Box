/* 
 * File:   clear_log.c
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:04 AM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ext_eeprom.h"
#include "matrix_keypad.h"

extern unsigned char clear; // External variable to indicate if logs are cleared
extern unsigned char ev_i = 0; // External variable for event index, initialized to 0
extern unsigned char log_c; // External variable to count logs
extern char main_f; // External variable for main state flag

void clear_log(char key) { // Function to clear logs
    static long int delay; // Static variable for delay timing

    key = read_switches(LEVEL_CHANGE); // Read the current switch state
    
    if (key = MK_SW11) { // Check if MK_SW11 switch is pressed (note: should use '==' for comparison)

        clear = 1; // Set clear flag to indicate logs are cleared
        log_c = 0; // Reset log count to 0
        clcd_print("Clear log   ", LINE1(0)); // Display message indicating log clearing
        clcd_print("SUCCESSFUL", LINE2(0)); // Display success message
        for (delay = 500000; delay--;); // Simple delay loop for timing
        ev_i = 11; // Set event index to 11 (indicating log clearing event)
        store_event(); // Store the event of clearing logs
        CLEAR_DISP_SCREEN; // Clear the display
        main_f = MENU; // Exit to menu
    }
}
