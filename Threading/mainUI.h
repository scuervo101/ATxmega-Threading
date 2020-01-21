/*
 * mainUI.h
 *
 * Created: 1/11/2020 6:49:09 PM
 *  Author: samue
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ledAnimation.h"

#define BSEL 117
#define BSCALE -3

void Main_UI(void);

void Uart_Init(void);

void Usart_Print(char str[]);

/*Function that will print out prompts and information*/
/*                     ---                            */

void LineFeed(void);

void Init_Prompt(void);

void U_In_Prompt(void);

void Help_Prompt(void);