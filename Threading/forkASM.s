
/*
 * forkASM.s
 *
 * Created: 1/12/2020 1:57:49 PM
 *  Author: samue
 */ 

 #include <avr/io.h>

.global Fork_asm
Fork_asm:

	mov YL, r24		;Load the memory location for our new Thread
	mov YH, r25		;
	
	pop r24			;Pop off return address
	pop r24			;
	pop r24

	pop r24			;Pop off garbage data
	pop r24			;

	pop r24
	std Y+29, r24

	pop r24
	std Y+28, r24

	pop r24			;5 pops
	std Y+31, r24

	pop r24
	std Y+30, r24

	pop r24
	std Y+27, r24

	pop r24
	std Y+26, r24

	pop r24
	std Y+25, r24

	pop r24			;10 pops
	std Y+24, r24

	pop r24
	std Y+23, r24

	pop r24
	std Y+22, r24

	pop r24
	std Y+21, r24

	pop r24
	std Y+20, r24

	pop r24			;15 pops
	std Y+19, r24

	pop r24			;16 pops
	std Y+18, r24

	std Y+17, r17
	std Y+16, r16
	std Y+15, r15
	std Y+14, r14
	std Y+13, r13
	std Y+12, r12
	std Y+11, r11
	std Y+10, r10
	std Y+9, r09
	std Y+8, r08
	std Y+7, r07
	std Y+6, r06
	std Y+5, r05
	std Y+4, r04
	std Y+3, r03
	std Y+2, r02

	pop r24				;Save Rampz
	std Y+41, r24

	pop r24				;Save Rampy
	std Y+40, r24

	pop	r24				;Save Rampx
	std Y+39, r24

	pop r24				;Save Rampd
	std Y+38, r24

	pop r24				;Save status register
	std Y+37, r24

	pop r24
	st Y, r24

	pop r24
	std Y+1, r24

	pop r24				;PC High byte
	std Y+32, r24

	pop r24				;PC Middle byte
	std Y+33, r24

	pop r24				;PC Low byte
	std Y+34, r24

	in r24, CPU_SPH		;Stack Pointer High byte
	std Y+35, r24

	in r24, CPU_SPL		;Stack Pointer Low byte
	std Y+36, r24

	ldd r24, Y+34		;Restore Low byte PC
	push r24

	ldd r24, Y+33		;Restore Middle byte PC
	push r24

	ldd r24, Y+32		;Restore High byte PC
	push r24

	reti

		