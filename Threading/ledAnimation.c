/*
 * ledAnimation.c
 *
 * Created: 1/14/2020 10:22:31 PM
 *  Author: samue
 */ 

#include "ledAnimation.h"

uint8_t sizeAnimation;
uint8_t animation[20];

void IO_Init(void)
{
	PORTA.DIRCLR = PIN7_bm + PIN6_bm + PIN5_bm + PIN4_bm + PIN3_bm + PIN2_bm + PIN1_bm + PIN0_bm; //dip switches
	PORTC.DIRSET = PIN7_bm + PIN6_bm + PIN5_bm + PIN4_bm + PIN3_bm + PIN2_bm + PIN1_bm + PIN0_bm; //leds
	
	PORTE.DIRCLR = PIN1_bm + PIN0_bm;
	PORTF.DIRCLR = PIN3_bm + PIN2_bm;
}

void TC_Init(void)
{
	TCC0.CNT = 0x00;
	TCC0.PER = 0x01F8;		//Using DIV64
	
	TCC1.CNT = 0x00;
	TCC1.PER = 0x0627;		//Using DIV1024
}

void Delay_1ms(void)
{
	TCC0.CTRLA = TC_CLKSEL_DIV64_gc;
	
	while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
	
	TCC0.CTRLA = TC_CLKSEL_OFF_gc;
	TCC0.INTFLAGS = 0xFF;
}

void Frame_Rate(void)
{
	TCC1.CTRLA = TC_CLKSEL_DIV1024_gc;
	
	while(!(TCC1.INTFLAGS & TC1_OVFIF_bm));
	
	TCC1.CTRLA = TC_CLKSEL_OFF_gc;
	TCC1.INTFLAGS = 0xFF;
}

void Edit_Mode(void)
{
	while(1)
	{
		if(!(PORTE.IN & PIN0_bm))
			Play_Mode();
		
		PORTC.OUT = PORTA.IN;
		
		if((!(PORTF.IN & PIN2_bm)) && (sizeAnimation != 20))
		{
			Delay_1ms();
			if(!(PORTF.IN & PIN2_bm))
			{
				while(!(PORTF.IN & PIN2_bm));
				animation[sizeAnimation] = PORTA.IN;
				sizeAnimation++;
			}
		}	
	}	
}

void Play_Mode(void)
{
	volatile uint8_t queue = 0;
	
	while(1)
	{
		if(!(PORTE.IN & PIN1_bm))
			Edit_Mode();
			
		if(queue == sizeAnimation)
			queue = 0;
			
		PORTC.OUT = animation[queue];
		queue++;
		Frame_Rate();
	}
}

void Main_LED(void)
{
	IO_Init();
	TC_Init();
	
	sizeAnimation = 0;
	
	Edit_Mode();
}