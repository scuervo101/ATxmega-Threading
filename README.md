# Multi-Tasking
 RTOS built on the ATxmega128a1u

 Multi-Tasking represented on the ATxmega128a1u. The RTOS currently only runs a few threads using a round robin scheduling algorithm. Future plans are to have multiple scheduling options to swap between such as priority scheduling and for the console to be able to create and kill other threads.
 
 RTOS works by interrupting between a process, pushing the process' entire context (registers, program counter, IO registers, and stack pointer), and loading another process' context. Each process has its own stack of size 0x200, allowing for each process to have a stack of 512 bytes.
 
# General Structure

Yoo.
 
# Context Switch
 ContextSwitch is a function written in assembly to save the context of the previous process before swapping to another process. Returns function by pushing the low, middle, and high byte onto the stack, as the ATxmega does not have a linker register. Once the ret (or reti in cases of interrupt handlers) instruction is called, the return address is popped off the stack (high, middle, then low byte) and loaded into the program counter. This is important for ContextSwitch to save the previous process' Program Counter and load in a new process' Program Counter before returning
 
    ldd r24, Y+34		;Restore Low byte PC
    push r24
    ldd r24, Y+33		;Restore Middle byte PC
    push r24
    ldd r24, Y+32		;Restore High byte PC
    push r24
    ~~~~
    reti
 
# Fork (new process)
 Fork is a function designed to create a thread. The user will call the fork function with a pointer to the process being created. The fork function will save the initial context of that process then return to that newly created process. Fork function also creates the new stack of each process.
 
 # PuTTY Setting:
 * Serial, 
 * 128000 baud rate
 * 8 data bits 
 * 1 stop bit 
 * even parity 
 * no flow control
