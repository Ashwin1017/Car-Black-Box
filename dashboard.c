/* 
 * File:   dashboard.c
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:05 AM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h" 
#include "adc.h" 
#include "matrix_keypad.h"

extern unsigned char time[9]; 
extern unsigned char event[][3]; 
extern unsigned char ev_i=0; 

unsigned short adc_reg_val; // Variable to store ADC value
unsigned char speed; // Variable to store calculated speed

void dashboard() { // Function to display dashboard information

    clcd_print("  Time   EVN SPD", LINE1(0)); // Print header on the first line
    get_time(); // Get the current time from the RTC

    clcd_print(time, LINE2(0)); // Display current time on the second line
    clcd_print(event[ev_i], LINE2(10)); // Display current event based on index

    adc_reg_val = read_adc(CHANNEL4); // Read ADC value from CHANNEL4

    // Convert ADC value to speed 
    speed = (adc_reg_val) / 10; // Calculate speed by dividing ADC value by 10

    // Display speed on CLCD
    clcd_putch('0' + (speed / 10), LINE2(14)); // Print the tens digit of speed
    clcd_putch('0' + (speed % 10), LINE2(15)); // Print the units digit of speed
}