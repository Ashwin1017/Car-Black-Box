/*
 * File:   view_log.c
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:11 AM
 */


#include <xc.h>
#include "main.h"
#include "adc.h"
#include "clcd.h"
#include "ext_eeprom.h"
#include "matrix_keypad.h"


unsigned char clear = 0; // Flag to indicate if logs are cleared
extern unsigned char ev_i = 0; // External variable for current event index
extern unsigned char log_c; // External variable for log count
extern char main_f; // External variable for main state flag
extern char menu_f; // External variable for menu state flag

void view_log(char key) {
    static unsigned int view_select1 = 0, view_select2 = 0; // Counters for switch presses
    static long int delay, log = 0; // Delay for timing and current log index

    key = read_switches(LEVEL_CHANGE); // Read current switch state

    if (key == MK_SW11) {
        view_select1++; // Increment forward selection counter for SW11
    }
    else if (view_select1 > 0 && view_select1 < 300) {
        view_select1 = 0; // Reset forward selection counter
        if (log < (log_c - 1)) // Ensure log index does not exceed log count
            log++; // Increment log index
    } 
    else if (view_select1 > 300) {
        view_select1 = 0; // Reset counter if threshold exceeded
        ev_i = 9; // Set event index to 9
        store_event(); // Store the view log event
        main_f = MENU; // Set flag for menu navigation
    }

    if (key == MK_SW12) {
        view_select2++; // Increment backward selection counter for SW12
    }
    else if (view_select2 > 0 && view_select2 < 300) {
        view_select2 = 0; // Reset backward selection counter
        if (log > 0) // Ensure log index does not go below zero
            log--; // Decrement log index
    }
    else if (view_select2 > 300) {
        view_select2 = 0; // Reset counter if threshold exceeded
    }

    if (clear == 0) { // Check if logs should be displayed
        clcd_print("#  TIME   EVN SP", LINE1(0)); // Print header for log display
        clcd_putch(log + '0', LINE2(0)); // Display current log index
        clcd_putch(' ', LINE2(1)); // Space after log index

        // Display time from EEPROM
        clcd_putch(read_ext_eeprom(log * 10 + 0), LINE2(2)); // Hour tens
        clcd_putch(read_ext_eeprom(log * 10 + 1), LINE2(3)); // Hour units
        clcd_putch(':', LINE2(4)); // Time separator
        clcd_putch(read_ext_eeprom(log * 10 + 2), LINE2(5)); // Minute tens
        clcd_putch(read_ext_eeprom(log * 10 + 3), LINE2(6)); // Minute units
        clcd_putch(':', LINE2(7)); // Time separator
        clcd_putch(read_ext_eeprom(log * 10 + 4), LINE2(8)); // Second tens
        clcd_putch(read_ext_eeprom(log * 10 + 5), LINE2(9)); // Second units
        clcd_putch(' ', LINE2(10)); // Space after time

        // Display event from EEPROM
        clcd_putch(read_ext_eeprom(log * 10 + 6), LINE2(11)); // First event character
        clcd_putch(read_ext_eeprom(log * 10 + 7), LINE2(12)); // Second event character
        clcd_putch(' ', LINE2(13)); // Space after event

        // Display speed from EEPROM
        clcd_putch(read_ext_eeprom(log * 10 + 8), LINE2(14)); // Speed tens
        clcd_putch(read_ext_eeprom(log * 10 + 9), LINE2(15)); // Speed units
    }

    if (clear == 1) { // Check if logs have been cleared
        clcd_print("NO LOGS TO SHOW", LINE1(0)); // Indicate that there are no logs to display
        clcd_print("                ", LINE2(0)); // Clear second line
        for (delay = 800000; delay--;); // Simple delay loop for visibility
        CLEAR_DISP_SCREEN; // Clear the display
        main_f = MENU; // Set flag to return to menu
    }
}
