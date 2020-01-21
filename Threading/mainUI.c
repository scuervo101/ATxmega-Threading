/*
 * mainUI.c
 *
 * Created: 1/11/2020 6:48:55 PM
 *  Author: samue
 */ 

#include "mainUI.h"

char Init_Prompt_str[] = "    Dev Build RTOS!/";
char U_In_Prompt_str[] = "user@DEV-REV0.1.1:~$ /";

char Help_Prompt_str[] = "This is where the help prompt would go! /";

uint8_t command_Flag = 0;

char line[10];
uint8_t lineCnt = 0;

void Usart_Init(void)
{
	PORTD.OUTCLR = PIN3_bm;
	PORTD.DIRSET = PIN3_bm;
	PORTD.DIRCLR = PIN2_bm;
	
	USARTD0.CTRLA = USART_RXCINTLVL_LO_gc;
	USARTD0.CTRLB = USART_RXEN_bm + USART_TXEN_bm;
	USARTD0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc + USART_PMODE_EVEN_gc + USART_CHSIZE_8BIT_gc;
	
	USARTD0.BAUDCTRLA = (0xFF & BSEL);
	USARTD0.BAUDCTRLB = (BSCALE << 4) + (BSEL >> 8);
	
}

void Usart_Print(char str[])
{
	int x = 0;
	while(str[x] != '/')
	{
		while(!(USARTD0.STATUS & USART_DREIF_bm));
		USARTD0.DATA = str[x];
		x++;
	}
}

void LineFeed(void)										//This is just a function equivalent to pressing enter in a document
{
	while(!(USARTD0.STATUS & USART_DREIF_bm));
	USARTD0.DATA = 0x0d;
	while(!(USARTD0.STATUS & USART_DREIF_bm));
	USARTD0.DATA = 0x0a;
	
	return;
}

void U_In_Prompt(void)
{
	LineFeed();
	Usart_Print(U_In_Prompt_str);
}

void Help_Prompt(void)
{
	LineFeed();
	LineFeed();
	Usart_Print(Help_Prompt_str);
	LineFeed();
}


void Init_Prompt(void)
{
	LineFeed();
	LineFeed();
	
	Usart_Print(Init_Prompt_str);
	
	LineFeed();
	LineFeed();
}

void Main_UI(void)
{
	//Equivalent of main
	
	Usart_Init();
	
	Init_Prompt();
	U_In_Prompt();

	while(1)
	{
		if(command_Flag)
		{
			
			if(line[0] == 'f' && line[1] == 'o' && line[2] == 'r' && line[3] == 'k')
			{
				Fork(Main_LED);
			}
			else if(line[0] == 'h' && line[1] == 'e' && line[2] == 'l' && line[3] == 'p')
			{
				Help_Prompt();
			}
			
			U_In_Prompt();
			command_Flag = 0;
		}
	}
}

ISR(USARTD0_RXC_vect)
{
	char received;
	received = USARTD0.DATA;							//Save received value into a variable
	USARTD0.DATA = received;
	
	if(received == 0x0d)
	{
		command_Flag = 1;
		lineCnt = 0;
	}
	else if(received == 0x7f && lineCnt != 0)
	{
		lineCnt--;		
	}
	else
	{
		line[lineCnt++] = received;
	}
}