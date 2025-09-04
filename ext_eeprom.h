/* 
 * File:   ext_eeprom.h
 * Author: ashwi
 *
 * Created on 28 September, 2024, 10:08 AM
 */

#ifndef Ext_eeprom
#define Ext_eeprom

#define SLAVE_READ_eeprom		0xA1
#define SLAVE_WRITE_eeprom		0xA0


void write_ext_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_ext_eeprom(unsigned char address1);

#endif