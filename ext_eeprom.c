/*
 * File:   ext_eeprom.c
 * Author: ashwi
 *
 * Created on 28 September, 2024, 10:23 AM
 */


#include <xc.h>
#include "main.h"
#include "i2c.h"
#include "ext_eeprom.h"



/* 
 * 
 * A0  -  Write Mode
 * A1  -  Read Mode
 */


void write_ext_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_eeprom);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(int i=3000;i--;);
}

unsigned char read_ext_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE_eeprom);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ_eeprom);
	data = i2c_read();
	i2c_stop();

	return data;
}
