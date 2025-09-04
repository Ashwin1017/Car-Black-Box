/*
 * File:   download_log.c
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:07 AM
 */


#include <xc.h>
#include "main.h"
#include "uart.h"
#include "clcd.h"
#include "i2c.h"
#include "ext_eeprom.h"
#include "matrix_keypad.h"


extern unsigned char clear; // External variable to check if logs are cleared
extern unsigned char ev_i=0; // External variable for event index
extern unsigned char log_c;
extern char main_f; // External variable for main state flag

void download_log() {
    static long int delay; // Delay counter for timing
    unsigned char arr[10][17]; // Array to store log entries

    // Check if logs should be displayed (not cleared)
    if (clear == 0) { 
        puts("#  TIME   EVN SP"); // Print header for log display
        puts("\n\r");

        // Fill the array with log entries
        for (int log = 0; log < log_c; log++) {
            // Fill in the time for each log entry
            arr[log][0] = read_ext_eeprom(log * 10 + 0); // Hour
            arr[log][1] = read_ext_eeprom(log * 10 + 1); // Minute
            arr[log][2] = ':'; // Separator
            arr[log][3] = read_ext_eeprom(log * 10 + 2); // Second
            arr[log][4] = read_ext_eeprom(log * 10 + 3); // Next second part
            arr[log][5] = ':'; // Separator
            arr[log][6] = read_ext_eeprom(log * 10 + 4); // Millisecond
            arr[log][7] = read_ext_eeprom(log * 10 + 5); // Next millisecond part
            arr[log][8] = ' '; // Space for formatting
            arr[log][9] = ' '; // Space for formatting

            // Fill in the event for each log entry
            arr[log][10] = read_ext_eeprom(log * 10 + 6); // First part of event
            arr[log][11] = read_ext_eeprom(log * 10 + 7); // Second part of event
            arr[log][12] = ' '; // Space for formatting

            // Fill in the speed for each log entry
            arr[log][13] = read_ext_eeprom(log * 10 + 8); // First part of speed
            arr[log][14] = read_ext_eeprom(log * 10 + 9); // Second part of speed
            arr[log][15] = '\0'; // Null-terminate the string for proper printing
        }

        // Print each log entry
        for (int i = 0; i < log_c; i++) {
            puts(arr[i]); // Output the log entry
            puts("\n\r"); // New line after each entry
        }

        // Indicate completion of the download process
        CLEAR_DISP_SCREEN; // Clear the display
        clcd_print("Download Log ", LINE1(1)); // Display download message
        clcd_print("Completed", LINE2(1)); // Display completed message

        // Wait before clearing the display
        for (delay = 500000; delay--;); // Simple delay loop
        ev_i = 10; // Set event index to 10
        store_event(); // Store the download event
        CLEAR_DISP_SCREEN; // Clear the display
        main_f = MENU; // Set flag to return to menu
    } 
    else {
        // If logs are cleared, indicate to the user
        CLEAR_DISP_SCREEN; // Clear the display
        clcd_print("LOGS ARE CLEARED", LINE1(1)); // Display cleared message

        // Wait before clearing the display
        for (delay = 500000; delay--;); // Simple delay loop
        CLEAR_DISP_SCREEN; // Clear the display
        main_f = MENU; // Set flag to return to menu
    }
}