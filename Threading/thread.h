/*
 * thread.h
 *
 * Created: 1/10/2020 12:21:26 PM
 *  Author: samue
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define thread_Size 0x2B
uint16_t Thread_Table;

typedef struct Threads
{
	uint8_t registers[32];
	uint8_t contextData[10];
	uint8_t priority;
} Thread;

Thread tasks[2];
uint8_t queue;
uint8_t maxQueue;

uint16_t stackPointer;

extern void Clock_init(void);

extern void Fork_asm(uint16_t loc);

extern void Repair_asm(uint16_t loc);

extern void Context_Switch(uint16_t oldLoc, uint16_t newLoc);

void System_Init(void);

void Init_Thread_Timer(void);

void Fork(void (*process)(void));

void Kill(Thread th);

void Switch(Thread oldTh, Thread newTh);


