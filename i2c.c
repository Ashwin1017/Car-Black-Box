/*
 * File:   i2c.c
 * Author: ashwi
 *
 * Created on 28 September, 2024, 10:09 AM
 */


#include <xc.h>

void init_i2c(void)
{
	/* Set SCL and SDA pins as inputs */
	TRISC3 = 1;
	TRISC4 = 1;
	/* Set I2C master mode */
	SSPCON1 = 0x28;

	SSPADD = 0x31;
	/* Use I2C levels, worked also with '0' */
	CKE = 0;
	/* Disable slew rate control  worked also with '0' */
	SMP = 1;
	/* Clear SSPIF interrupt flag */
	SSPIF = 0;
	/* Clear bus collision flag */
	BCLIF = 0;
}

void i2c_idle(void)  // wait until the I2C operation is finished
{
	while (!SSPIF);  // loop until the SSP interrupt flag is set
	SSPIF = 0;      // reset the SSP interrupt flag
}

void i2c_ack(void)  // check the acknowledgment status
{
	if (ACKSTAT)     // if acknowledgment status indicates an ACK received
	{
		/* Do debug print here if required */  // optional debug message
	}
}

void i2c_start(void)  // start an I2C communication session
{
	SEN = 1;    // initiate the start condition by setting the SEN bit
	i2c_idle();  // wait until the start condition is completed
}

void i2c_stop(void)  // end the I2C communication session
{
	PEN = 1;    // initiate the stop condition by setting the PEN bit
	i2c_idle();  // wait until the stop condition is completed
}

void i2c_rep_start(void)  // perform a repeated start in I2C
{
	RSEN = 1;  // initiate repeated start condition by setting the RSEN bit
	i2c_idle();  // wait until the repeated start condition is completed
}

void i2c_write(unsigned char data)  // send data over I2C
{
	SSPBUF = data;  // place data into the SSP buffer for transmission
	i2c_idle();  // wait until the write operation is completed
}

void i2c_rx_mode(void)  // prepare to receive data over I2C
{
	RCEN = 1;  // enable the receive mode by setting the RCEN bit
	i2c_idle();  // wait until the receive mode is ready
}

void i2c_no_ack(void)  // configure for NACK after receiving data
{
	ACKDT = 1;  // set the acknowledgment data to NACK
	ACKEN = 1;  // enable the NACK response
}

unsigned char i2c_read(void)  // read data from I2C bus
{
	i2c_rx_mode();  // switch to receive mode
	i2c_no_ack();  // prepare to send NACK after reading

	return SSPBUF;  // return the data received from the SSP buffer
}
                                                                                                                    
