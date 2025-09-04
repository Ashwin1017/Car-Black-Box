/*
 * File:   clcd.c
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:29 AM
 */


#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char byte, unsigned char control_bit)
{
	CLCD_RS = control_bit;           // Set RS pin to determine instruction or data mode
	CLCD_PORT = byte;                // Output the byte to the data port

	/* Should be atleast 200ns */
	CLCD_EN = HI;                    // Set EN pin high to latch data/inst.
	CLCD_EN = LO;                    // Set EN pin low to complete the latch

	PORT_DIR = INPUT;                // Set RD7 direction to input to read busy flag
	CLCD_RW = HI;                    // Set RW pin high to read from LCD
	CLCD_RS = INSTRUCTION_COMMAND;  // Set RS pin to instruction  mode

	do
	{
		CLCD_EN = HI;                // Set EN pin high to read busy flag
		CLCD_EN = LO;                // Set EN pin low to complete the read
	} while (CLCD_BUSY);            // Wait until the busy flag is cleared

	CLCD_RW = LO;                    // Set RW pin low to return to write mode
	PORT_DIR = OUTPUT;               // Set RD7 direction back to output
}

void init_clcd()
{
	/* Set PortD as output port for CLCD data */
	TRISD = 0x00;            // Configure PortD as all outputs for data lines

	/* Set PortC as output port for CLCD control */
	TRISC = TRISC & 0xF8;     // Configure PortC bits RC0, RC1, and RC2 as outputs for control lines

	CLCD_RW = LO;             // Set RW pin to low for write operation

    /* Startup Time for the CLCD controller */
    __delay_ms(30);          // Wait for at least 30 ms for LCD to power up

    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);  // Initialize LCD in 8-bit mode
    __delay_us(4100);                                 // Wait for at least 4.1 ms
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);  // Repeat 8-bit mode initialization
    __delay_us(100);                                  // Wait for at least 100 us
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);  // Repeat 8-bit mode initialization
    __delay_us(1);                                   // Wait for at least 1 us
    
    CURSOR_HOME;                    // Move cursor to home position
    __delay_us(100);               // Wait for at least 100 us
    TWO_LINE_5x8_MATRIX_8_BIT;     // Set LCD to 2 lines, 5x8 matrix, 8-bit mode
    __delay_us(100);               // Wait for at least 100 us
    CLEAR_DISP_SCREEN;             // Clear the display screen
    __delay_us(500);               // Wait for at least 500 us
    DISP_ON_AND_CURSOR_OFF;        // Turn on the display and turn off the cursor
    __delay_us(100);               // Wait for at least 100 us
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);  // Set the starting address for the display
	while (*data != '\0')                   // Loop through each character in the string
	{
		clcd_write(*data++, DATA_COMMAND); // Write each character to the display
	}
}

void clcd_putch(const unsigned char data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);  // Set the address for the character to be written
	clcd_write(data, DATA_COMMAND);        // Write the character data to the display
}

