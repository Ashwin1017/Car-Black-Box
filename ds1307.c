/*
 * File:   ds1307.c
 * Author: ashwi
 *
 * Created on 28 September, 2024, 10:22 AM
 */


#include <xc.h>
#include "main.h"
#include "i2c.h"
#include "ds1307.h"



/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */

void init_ds1307(void)
{
    unsigned char dummy; // Variable to hold read values

    /* Stop the clock by setting the CH (Clock Halt) bit of the RTC */
    dummy = read_ds1307(SEC_ADDR); // Read the current seconds register
    write_ds1307(SEC_ADDR, dummy | 0x80); // Set CH bit to stop the clock

    /* Set 12-hour format in the HOUR register */
    dummy = read_ds1307(HOUR_ADDR); // Read the current hour register
    write_ds1307(HOUR_ADDR, dummy | 0x40); // Set 12-hour format bit

    /* 
     * Configure the control register of the DS1307:
     * Bit 7 - OUT (0 = output disabled)
     * Bit 6 - 0 (reserved)
     * Bit 5 - 0 (reserved)
     * Bit 4 - SQWE (1 to enable square wave output)
     * Bit 3 - 0 (reserved)
     * Bit 2 - 0 (reserved)
     * Bit 1 - RS1 (set to 1 for 32.768 KHz output)
     * Bit 0 - RS0 (set to 1 for 32.768 KHz output)
     */ 
    
    write_ds1307(CNTL_ADDR, 0x93); // Write 0x93 to configure SQW output i.e, the hexa value of control reg.

    /* Start the clock by clearing the CH bit of the RTC */
    dummy = read_ds1307(SEC_ADDR); // Read the current seconds register again
    write_ds1307(SEC_ADDR, dummy & 0x7F); // Clear CH bit to start the clock
}

void write_ds1307(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
}

unsigned char read_ds1307(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ);
	data = i2c_read();
	i2c_stop();

	return data;
}