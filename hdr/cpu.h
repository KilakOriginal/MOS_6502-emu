#ifndef CPU_h
#define CPU_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char  Byte;	// 8 Bits
typedef unsigned short Word;	// 16 Bits
typedef unsigned int   u32;


// Memory
#define MAX_MEM 1024 * 64

struct Mem
{
	Byte Data[MAX_MEM];
};


// CPU
struct CPU
{

	Word PC;	// Programme Counter
	Word SP;	// Stack Pointer
	
	// Registers
	Byte A;		// Accumulator
	Byte X;		// Index Register X
	Byte Y;		// Index Register Y
	
	// Processor status
	Byte C : 1;	// Carry Flag
	Byte Z : 1;	// Zero Flag
	Byte I : 1;	// Interrupt Disable
	Byte D : 1;	// Decimal Mode
	Byte B : 1;	// Break Command
	Byte V : 1;	// Overflow Flag
	Byte N : 1;	// Negative Flag
};


// Memory functions
void Memory_Initialise(struct Mem* mem);
Byte Memory_Fetch(struct CPU*, struct Mem*, u32*);

Byte Get_Memory(struct Mem*, u32);

// CPU functions
void CPU_Reset(struct CPU*, struct Mem*);
void CPU_Execute(struct CPU*, struct Mem*, u32);

#endif // !CPU_h

