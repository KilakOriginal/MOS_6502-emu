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
	Byte SP;	// Stack Pointer
	
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
Byte Memory_Read_Byte(struct CPU* cpu, struct Mem* mem, u32* cycles, Byte address);
Byte Memory_Fetch_Byte(struct CPU* cpu, struct Mem* mem, u32* cycles);

Byte Get_Memory(struct Mem* mem, u32 index);
int Set_Memory(struct Mem* mem, u32 index, u32 value);

// CPU functions
void CPU_Reset(struct CPU* cpu, struct Mem* mem);
void CPU_Execute(struct CPU* cpu, struct Mem* mem, u32 cycles);

// Opcodes
// Load Accumulator
#define INSTRUCTION_LDA_IMMEDIATE 0xA9	// Immediate
#define INSTRUCTION_LDA_ZEROPAGE  0xA5	// Zero Page
#define INSTRUCTION_LDA_ZEROPAGEX 0xB5	// Zero Page,X
#define INSTRUCTION_LDA_ABSOLUTE  0xAD	// Absolute
#define INSTRUCTION_LDA_ABSOLUTEX 0xBD	// Absolute,X
#define INSTRUCTION_LDA_ABSOLUTEY 0xB9	// Absolute,Y
#define INSTRUCTION_LDA_INDIRECTX 0xA1	// Indirect,X
#define INSTRUCTION_LDA_INDIRECTY 0xB1	// Indirect,Y

#endif // !CPU_h

