/*
 * File:   isr.c
 * Author: ashwi
 *
 * Created on 28 September, 2024, 10:53 AM
 */


#include <xc.h>
#include "main.h"

unsigned int sec=120;
void __interrupt() isr(void)
{
	static unsigned short count;
     
	if (TMR0IF)
	{
		TMR0 = TMR0 + 8;

		if (count++ == 20000)
		{
			count = 0;
			sec--;
		}
		TMR0IF = 0;
	}
}