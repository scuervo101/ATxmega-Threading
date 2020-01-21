/*
 * Threading.c
 *
 * Created: 1/8/2020 11:02:40 AM
 * Author : samue
 */ 

#include <avr/io.h>
#include "thread.h"
#include "mainUI.h"


/*
uint8_t some_Random_Function(uint8_t num)
{
	num+=5;
	return num;
}*/

int main(void)
{
	System_Init();
	Init_Thread_Timer();
	
	Thread_Table = (uint16_t)tasks;
	queue = 0x00;
	maxQueue = 0x00;
	
	Fork(Main_UI);
	
    while (1) 
	{
    }
}

