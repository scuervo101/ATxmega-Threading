# Multi-Tasking
 RTOS built on the ATxmega128a1u

 Multi-Tasking represented on the ATxmega128a1u. The RTOS currently only runs a few threads using a round robin scheduling algorithm. Future plans are to have multiple scheduling options to swap between such as priority scheduling and for the console to be able to create and kill other threads.
 
 RTOS works by interrupting between a process, pushing the process' entire context (registers, program counter, IO registers, and stack pointer), and loading another process' context. Each process has its own stack of size 0x200, allowing for each process to have a stack of 512 bytes.
 
 Fork is a function designed to create a thread. The user will call the fork function with a pointer to the process being created. The fork function will save the initial context of that process then return to that newly created process. Fork function also creates the new stack of each process.
 
 # PuTTY Setting:
 Serial, 128000 baud rate, 8 data bits, 1 stop bit, even parity, no flow control
