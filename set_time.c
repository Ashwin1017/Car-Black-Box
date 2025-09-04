/*
 * File:   set_time.c
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:09 AM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "matrix_keypad.h"


extern unsigned char time[9]; // External variable to store the current time
extern unsigned char ev_i = 0; // External variable for event index, initialized to 0
extern char main_f; // External variable for main state flag

void set_time(char key) {
    unsigned char hour = 0, min = 0, sec = 0; // Variables to store hours, minutes, and seconds
    unsigned static change; // Static variable to track which time component is being changed
    static long int delay; // Static variable for delay timing
    unsigned char *change_time = time; // Pointer to the current time array
    unsigned static int val1 = 0, val2 = 0; // Static counters for key presses

    key = read_switches(LEVEL_CHANGE); // Read the current switch state
    clcd_print(time, LINE2(0)); // Display the current time on the LCD

    // Increment hour/minute/second selector
    if (key == MK_SW12) {
        val1++; // Increment val1 if switch MK_SW12 is pressed
    }
    // Change selected time value based on val1
    if (val1 < 500 && val1 > 0 && key == ALL_RELEASED) {
        val1 = 0; // Reset val1
        change = (change + 1) % 3; // Cycle through 0 (hours), 1 (minutes), 2 (seconds)
    } 
    else if (val1 > 500 && key == ALL_RELEASED) {
        val1 = 0; // Reset val1
        main_f = MENU; // Exit time setting and go to menu
    }

    if (key == MK_SW11) {
        val2++; // Increment val2 if switch MK_SW11 is pressed
    }
    // Adjust time based on current selection indicated by val2
    if (val2 < 500 && val2 > 0 && key == ALL_RELEASED) {
        val2 = 0; // Reset val2

        if (change == 0) { // If changing hours
            change_time[1]++; // Increment hour's units
            if (change_time[1] > '9') { // If units exceed 9
                change_time[1] = '0'; // Reset units
                change_time[0]++; // Increment tens
            }
            // Handle hour overflow (24-hour format)
            if (change_time[0] == '2' && change_time[1] == '4') {
                change_time[0] = '0'; // Reset to 00
                change_time[1] = '0'; // Reset to 00
            }
        } else if (change == 1) { // If changing minutes
            change_time[4]++; // Increment minute's units
            if (change_time[4] > '9') { // If units exceed 9
                change_time[4] = '0'; // Reset units
                change_time[3]++; // Increment tens
            }
            // Handle minute overflow (60 minutes)
            if (change_time[3] == '6') {
                change_time[3] = '0'; // Reset tens
                change_time[4] = '0'; // Reset units
            }
        } else if (change == 2) { // If changing seconds
            change_time[7]++; // Increment second's units
            if (change_time[7] > '9') { // If units exceed 9
                change_time[7] = '0'; // Reset units
                change_time[6]++; // Increment tens
            }
            // Handle second overflow (60 seconds)
            if (change_time[6] == '6') {
                change_time[6] = '0'; // Reset tens
                change_time[7] = '0'; // Reset units
            }
        }
    } 
    else if (val2 > 500 && key == ALL_RELEASED) { // If val2 exceeds 500 and key is released
        val2 = 0; // Reset val2
        clcd_print("SET TIME SUCCESS", LINE1(0)); // Indicate time set success
        clcd_print("                  ", LINE2(0)); // Clear second line

        // Write updated time to RTC (Real-Time Clock)
        hour = ((change_time[0] - '0') << 4) | (change_time[1] - '0'); // Convert hour to BCD format
        write_ds1307(HOUR_ADDR, hour); // Write hour to RTC

        min = ((change_time[3] - '0') << 4) | (change_time[4] - '0'); // Convert minute to BCD format
        write_ds1307(MIN_ADDR, min); // Write minute to RTC

        sec = ((change_time[6] - '0') << 4) | (change_time[7] - '0'); // Convert second to BCD format
        write_ds1307(SEC_ADDR, sec); // Write second to RTC

        for (delay = 200000; delay--;); // Delay for timing
        ev_i = 12; // Set event index to 12
        store_event(); // Store the event of setting time
        CLEAR_DISP_SCREEN; // Clear the display

        main_f = DASHBOARD; // Exit to dashboard
    }
}
