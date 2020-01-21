# Multi-Tasking
 RTOS built on the ATxmega128a1u

 Multi-Tasking represented on the ATxmega128a1u. The RTOS currently only runs a few threads using a round robin scheduling algorithm. Future plans are to have multiple scheduling options to swap between such as priority scheduling and for the console to be able to create and kill other threads.
 
 RTOS works by interrupting between a process, pushing the process' entire context (registers, program counter, IO registers, and stack pointer), and loading another process' context. Each process has its own stack of size 0x200, allowing for each process to have a stack of 512 bytes.
 
## General Structure
 For easy thread access and manipulation, I created a thread structure (shown below) that stores the registers, contextData (ramp registers, status register, stack pointer, and PC), and priority (for priority scheduling).

    typedef struct Threads
    {
       uint8_t registers[32];
       uint8_t contextData[10];
       uint8_t priority;
    } Thread;
    
 Each thread will be 0x2B or 43 bytes long. This is important as the scheduler will contain and array of Threads and indexing between each thread in assembly will be as easy as adding 43 to the memory address. The memory address will passed in as a reference to the element in the thread array (&threadArray[0]). This will be saved in assembly to the Y register
 
 The registers are located first to easily save them with a displacement match the register number (for example, to access register r27, you can load with 27 displacement: ldd r27, Y+27).
 
 The contextData array of the Thread (Y+32 - Y+41) contains in order: Program Counter (3 byte Big Endian)(Y+32 - Y+34), Stack Pointer (2 byte Big Endian)(Y+35 - Y+36), Status Register(Y+37), RampD(Y+38), RampX(Y+39), RampY(Y+40), RampZ(Y+41).
 
 Each Thread has its own stack (represented with differend stack pointers). This is created by the Fork function.
## Context Switch
 ContextSwitch is a function written in assembly to save the context of the previous process before swapping to another process. Returns function by pushing the low, middle, and high byte onto the stack, as the ATxmega does not have a linker register. Once the ret (or reti in cases of interrupt handlers) instruction is called, the return address is popped off the stack (high, middle, then low byte) and loaded into the program counter. This is important for ContextSwitch to save the previous process' Program Counter and load in a new process' Program Counter before returning. Below are a few snippets of code that demonstrate removing the PC from the stack and loading it back in before returning.
 
    pop r24				;PC High byte
    std Y+32, r24
    pop r24				;PC Middle byte
    std Y+33, r24
    pop r24				;PC Low byte
    std Y+34, r24
    ~~~~
    ldd r24, Y+34		;Restore Low byte PC
    push r24
    ldd r24, Y+33		;Restore Middle byte PC
    push r24
    ldd r24, Y+32		;Restore High byte PC
    push r24
    ~~~~
    reti
    
 The stack pointer is the next important attribute to consider when switching to a new process. As mentioned in the general structure section, each thread has access to its own 512 byte stack. To emulate mutliple stacks, the fork function creates individual stack pointers that are saved in each process' context and loaded into the stack pointer depending on which process is running. The stack pointer is saved only once all items from the ContextSwitch interrupt are removed (including the PC). After everything is removed from the stack, the process' individual stack pointer is saved, then the process' stack pointer is loaded and the PC is pushed onto that new stack. Below is the code for saving the stack and loading a new stack pointer.
 
    in r24, CPU_SPH		;Stack Pointer High byte
    std Y+35, r24
    in r24, CPU_SPL		;Stack Pointer Low byte
    std Y+36, r24
    ~~~~
    ldd r24, Y+35
    out CPU_SPH, r24
    ldd r24, Y+36
    out CPU_SPL, r24
(Out and In are specified instruction for IO Registers such as the stack pointer)

 After the stack pointer and Program Counter, we save each register and ramp register (used for extended 24 bit addressing) for the old process and load all of the registers from the new project. A few things to be careful with: I am using r24 as the scratch register for all of the instructions, so that must be loaded last. Also the Y register (r28, r29) is being used as a pointer to the memory location of the old process (and X register (r26, r27) temporarily holds the pointer to the new process to eventually copy it back to the Y register) meaning we need to push the YL (r28) and YH (r29) of the new process onto the stack and then pop that off the stack into r28 and r29 (so we do not unintentionally change the memory location from which we are reading before we finish grabbing all the needed information). Below are some code snippets of carefully handling the Y register and the scratch registers.
 
    mov YL, XL
    mov YH, XH
    ~~~~
    ldd r24, Y+28		;Store r28 on stack to not change Y register
    push r24	
    ldd r24, Y+29		;Store r29 on stack to not change Y register
    push r24			
    ldd r24, Y+24		;Restore our scratch Registers last
    ldd r25, Y+25		;
    
    pop r29
    pop r28
(mov YL, XL and mov YH, XH copies X->Y. This is done after we have finished saving the first thread's context)

## Fork (new process)
 Fork is a function designed to create a thread. The user will call the fork function with a pointer to the process being created. The fork function will save the initial context of that process then return to that newly created process (eventually a context switch will return to the process that called fork). Fork function also creates the new stack of each process. Below is the code that creates a new stack for each individual process.
 
    stackPointer = 0x3FFF - (0x200 * maxQueue);
    
    CPU_SPL = (0xFF & stackPointer);
    CPU_SPH = (stackPointer >> 8);
    
 When calling from a process, we must repair the context of the caller process. This is why I wrote repairContext.s, an assembly function that quickly repairs the context of the caller process and contiues to runn the Fork function. This function stores the program counter and any changed registers to the thread struct of the caller process. Then this functions returns back to the Fork function.
## PuTTY Setting:
 * Serial
 * 128000 baud rate
 * 8 data bits 
 * 1 stop bit 
 * even parity 
 * no flow control
