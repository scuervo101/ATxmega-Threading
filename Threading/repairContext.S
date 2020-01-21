
/*
 * repairContext.S
 *
 * Created: 1/15/2020 11:19:16 PM
 *  Author: samue
 */ 
 #include <avr/io.h>

.global Repair_asm
Repair_asm:
	
	push XL
	push XH

	mov XL, YL
	mov XH, YH

	mov YL, r24		;Load the memory location for our new Thread
	mov YH, r25		;

	pop r03
	std Y+26, r03

	pop r03
	std Y+27, r03

	pop r00			;Pop off return address
	pop r01			;
	pop r02			;

	pop r24			;R24 Pointer
	pop r25			;R25

	pop r03
	std Y+29, r03

	pop r03
	std Y+28, r03

	pop r03
	std Y+32, r03

	pop r03
	std Y+33, r03

	pop r03
	std Y+34, r03

	in r03, CPU_SPH
	std Y+35, r03

	in r03, CPU_SPL
	std Y+36, r03

	ldd r03, Y+34
	push r03

	ldd r03, Y+33
	push r03

	ldd r03, Y+32
	push r03

	ldd r03, Y+28
	push r03

	ldd r03, Y+29
	push r03

	push r25
	push r24

	push r02
	push r01
	push r00

	mov YL, XL
	mov YH, XH

	ret