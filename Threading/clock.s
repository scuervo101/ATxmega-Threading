
/*
 * clock.s
 *
 * Created: Date
 *  Author: Name
 */ 

#include <avr/io.h>

.global Clock_init
Clock_init:
	
	push r16							;Save the register

	ldi r16, OSC_RC32MEN_bm				;Here we are enabling the 32MHz oscillator
	sts OSC_CTRL, r16					;

OSC_Ready:
	lds r16, OSC_STATUS					;Load the oscillator status register

	sbrs r16, OSC_RC32MRDY_bp			;Poll the 32MHz ready bit
	rjmp OSC_Ready						;Until the 32MHz oscillator is ready

	ldi r16, 0xD8						;Now we must send the CPU_CPP register a 0xD8 signature to access protect IO registers
	sts CPU_CCP, r16					;For only 4 clock cycles

	ldi r16, (0x01 << CLK_SCLKSEL_gp)	;Here we are setting the protect clk register to come from the 32MHz oscillator
	sts CLK_CTRL, r16					;

CLK_Prescalers:
	ldi r16, 0xD8						;Send the signature again
	sts CPU_CCP, r16					;
	
	ldi r16, ((0x00 << CLK_PSADIV_gp) + (0x00 << CLK_PSBCDIV_gp))	;This doesn't do anything the the prescaller
	;ldi r16, ((0x03 << CLK_PSADIV_gp) + (0x00 << CLK_PSBCDIV_gp))				;But this would set our first prescaler (A) to divide by 4
	sts CLK_PSCTRL, r16												;To get us 8MHz

	pop r16								;Restore r16 register

	ret