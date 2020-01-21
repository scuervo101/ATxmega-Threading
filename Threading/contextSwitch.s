
/*
 * contextSwitch.s
 *
 * Created: 1/10/2020 1:55:03 PM
 *  Author: samue
 */ 

#include <avr/io.h>

.global Context_Switch
Context_Switch:

	mov YL, r24				;Load old Thread pointer into Y
	mov YH, r25				;

	mov XL, r22				;Load new Thread pointer into X
	mov XH, r23				;

	in r24, CPU_SPL			;
	in r25, CPU_SPH			;
							;	This removes the next 7 items from the stack
	adiw r24, 0x07			;	More efficient then 7 pop instructions
							;
	out CPU_SPL, r24		;
	out CPU_SPH, r25		;

	;----------------------------------------------------------------------Pre-switch setup!
	;-----------------------------------Save all of the old Thread's registers and data before we can switch context

	pop r24
	std Y+29, r24

	pop r24
	std Y+28, r24

	pop r24
	std Y+31, r24

	pop r24
	std Y+30, r24

	pop r24
	std Y+27, r24

	pop r24
	std Y+26, r24

	pop r24
	std Y+25, r24

	pop r24
	std Y+24, r24

	pop r24
	std Y+23, r24

	pop r24
	std Y+22, r24

	pop r24
	std Y+21, r24

	pop r24
	std Y+20, r24

	pop r24
	std Y+19, r24

	pop r24
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

	mov YL, XL
	mov YH, XH

	;--------------------------------------------------Begin Switch!
	;-------------------------Process of restoring all registers and data from next Thread!

	ld r0, Y
	ldd r1, Y+1
	ldd r2, Y+2
	ldd r3, Y+3
	ldd r4, Y+4
	ldd r5, Y+5
	ldd r6, Y+6
	ldd r7, Y+7
	ldd r8, Y+8
	ldd r9, Y+9
	ldd r10, Y+10
	ldd r11, Y+11
	ldd r12, Y+12
	ldd r13, Y+13
	ldd r14, Y+14
	ldd r15, Y+15
	ldd r16, Y+16
	ldd r17, Y+17
	ldd r18, Y+18
	ldd r19, Y+19
	ldd r20, Y+20
	ldd r21, Y+21
	ldd r22, Y+22
	ldd r23, Y+23
	
	ldd r26, Y+26
	ldd r27, Y+27

	ldd r30, Y+30
	ldd r31, Y+31

	ldd r24, Y+37
	sts CPU_SREG, r24

	ldd r24, Y+38
	sts CPU_RAMPD, r24

	ldd r24, Y+39
	sts CPU_RAMPX, r24

	ldd r24, Y+40
	sts CPU_RAMPY, r24

	ldd r24, Y+41
	sts CPU_RAMPZ, r24

	ldd r24, Y+35
	out CPU_SPH, r24

	ldd r24, Y+36
	out CPU_SPL, r24

	ldd r24, Y+34		;Restore Low byte PC
	push r24

	ldd r24, Y+33		;Restore Middle byte PC
	push r24

	ldd r24, Y+32		;Restore High byte PC
	push r24

	ldd r24, Y+28		;Store r28 on stack to not change Y register
	push r24			

	ldd r24, Y+29		;Store r29 on stack to not change Y register
	push r24			

	ldd r24, Y+24		;Restore our scratch Registers last
	ldd r25, Y+25		;

	pop r29
	pop r28

	reti
