/*
 * ledAnimation.h
 *
 * Created: 1/14/2020 10:22:14 PM
 *  Author: samue
 */ 

#include <avr/io.h>

void IO_Init(void);

void TC_Init(void);

void Delay_1ms(void);

void Frame_Rate(void);

void Play_Mode(void);

void Edit_Mode(void);

void Main_LED(void);