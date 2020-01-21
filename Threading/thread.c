/*
 * thread.c
 *
 * Created: 1/10/2020 12:21:17 PM
 *  Author: samue
 */ 

#include "thread.h"

void System_Init(void)
{
	Clock_init();
	
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();
}

void Init_Thread_Timer(void)
{
	TCF0.CNT = 0x0000;
	TCF0.PER = 0x0143;
	
	TCF0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	TCF0.CTRLA = TC_CLKSEL_DIV1_gc;
}

void Fork(void (*process)(void))
{
	if(maxQueue != 0)
	{
		Repair_asm(Thread_Table);
		queue++;
	}
	
	TCF0.CTRLA = TC_CLKSEL_OFF_gc;
	
	TCF0.CNT = 0x0000;
	TCF1.CNT = 0x0000;
	TCF1.PER = 0x0050;
	
	TCF1.INTCTRLA = TC_OVFINTLVL_LO_gc;
	
	maxQueue++;
	stackPointer = 0x3FFF - (0x200 * maxQueue);

	CPU_SPL = (0xFF & stackPointer);
	CPU_SPH = (stackPointer >> 8);
	
	TCF0.CTRLA = TC_CLKSEL_DIV1_gc;
	TCF1.CTRLA = TC_CLKSEL_DIV1_gc;

	process();
}

ISR(TCF0_OVF_vect)
{
	if(maxQueue != 1)
	{
		Thread *oldTh, *newTh;
		oldTh = &tasks[queue++];
		
		if(queue == maxQueue)
			queue = 0;
		
		newTh = &tasks[queue];
		
		Context_Switch(oldTh, newTh);
	}
}


ISR(TCF1_OVF_vect)
{
	TCF1.CTRLA = TC_CLKSEL_OFF_gc;
	
	uint16_t location = Thread_Table + ((maxQueue-1)*thread_Size);
	
	Fork_asm(location);
}