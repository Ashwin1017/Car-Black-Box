/*
 * File:   store_event.c
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:10 AM
 */


#include <xc.h>
#include "main.h"
#include "adc.h"
#include "clcd.h"
#include "i2c.h"
#include "ext_eeprom.h"
#include "matrix_keypad.h"


extern unsigned char time[9]; // External array to hold time data (HH:MM:SS)
extern unsigned char event[][3]; // External 2D array for event characters
extern unsigned char ev_i; // External variable for the current event index
extern unsigned char log_c; // External variable for the log count
extern unsigned char speed; // External variable for speed

unsigned char store_data[11]; // Array to store event data, including null terminator

void store_event()
{ 
    unsigned int j; // Loop index
  
    // Store time and event data in the store_data array
    store_data[0] = time[0]; // Store hour tens place
    store_data[1] = time[1]; // Store hour units place
    store_data[2] = time[3]; // Store minute tens place
    store_data[3] = time[4]; // Store minute units place
    store_data[4] = time[6]; // Store second tens place
    store_data[5] = time[7]; // Store second units place
    store_data[6] = event[ev_i][0]; // Store first character of the current event
    store_data[7] = event[ev_i][1]; // Store second character of the current event
    store_data[8] = (speed / 10 + 48); // Store tens digit of speed as a character
    store_data[9] = (speed % 10 + 48); // Store units digit of speed as a character
    store_data[10] = '\0'; // Null-terminate the data string
    
    // Write stored data to EEPROM at the next available log position
    for (j = 0; j < 10; j++)
    {
        write_ext_eeprom(log_c * 10 + j, store_data[j]); // Write each data byte to EEPROM
    }
    
    log_c++; // Increment log count for the next log entry
}
