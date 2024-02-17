#ifndef CPU_h
#define CPU_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum Endianness
{
	BIG,
	LITTLE,
	AUTO
} Endianness;


typedef unsigned char  Byte;	// 8 Bits
typedef unsigned short Word;	// 16 Bits
typedef unsigned int   u32;		// 32 Bits


// Memory
#define MAX_MEM 1024 * 64

typedef struct Memory
{
	Byte Data[MAX_MEM];
} Mem;


// CPU
#define MAX_ERRORS 10

typedef struct CPU
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
} CPU;


/**
 * @brief If arg is "AUTO" this will automatically determine the endianness of
 * the current system. Else arg is used to specify "BIG" or "LITTLE".
 */ 
void MOS_6502_set_endianness(int arg);


// Memory functions
void Mem_Initialise(Mem* mem);
const Byte Mem_Read_Byte(const CPU* cpu, 
                   const Mem* mem,
				   u32* cycles,
				   const Byte address);
const Byte Mem_Read_Word(const CPU* cpu,
                   const Mem* mem,
				   u32* cycles,
				   const Byte address);
const Byte Mem_Fetch_Byte(CPU* cpu, const Mem* mem, u32* cycles);
const Word Mem_Fetch_Word(CPU* cpu, const Mem* mem, u32* cycles);

const Byte Get_Memory(const Mem* mem, const Word index);
const int Set_Memory(Mem* mem, const Word index, const Byte data);


// CPU functions
void CPU_Reset(CPU* cpu, Mem* mem);
void CPU_Execute(CPU* cpu, Mem* mem, const u32 cycles);
const Byte CPU_Fetch_Register(const CPU* cpu,
                              const Byte register,
							  u32* cycles);

// Opcodes
// Add Memory to Accumulator with Carry
#define INSTRUCTION_ADC_IMMEDIATE   0x69	// Immediate
#define INSTRUCTION_ADC_ABSOLUTE    0x6D	// Absolute
#define INSTRUCTION_ADC_ABSOLUTEX   0x7D	// Absolute,X
#define INSTRUCTION_ADC_ABSOLUTEY   0x79	// Absolute,Y
#define INSTRUCTION_ADC_ZEROPAGE    0x65	// Zero Page
#define INSTRUCTION_ADC_ZEROPAGEX   0x75	// Zero Page,X
#define INSTRUCTION_ADC_INDIRECTX   0x61	// (Zero Page,X)
#define INSTRUCTION_ADC_INDIRECTY   0x71	// (Zero Page),Y

// "AND" Memory with Accumulator
#define INSTRUCTION_AND_IMMEDIATE   0x29	// Immediate
#define INSTRUCTION_AND_ABSOLUTE    0x2D	// Absolute
#define INSTRUCTION_AND_ABSOLUTEX   0x3D	// Absolute,X
#define INSTRUCTION_AND_ABSOLUTEY   0x39	// Absolute,Y
#define INSTRUCTION_AND_ZEROPAGE    0x25	// Zero Page
#define INSTRUCTION_AND_ZEROPAGEX   0x35	// Zero Page,X
#define INSTRUCTION_AND_INDIRECTX   0x21	// (Zero Page,X)
#define INSTRUCTION_AND_INDIRECTY   0x31	// (Zero Page),Y

// Arithmetic Shift Left
#define INSTRUCTION_ASL_ACCUMULATOR 0x0A	// A
#define INSTRUCTION_ASL_ABSOLUTE    0x0E	// Absolute
#define INSTRUCTION_ASL_ABSOLUTEX   0x1E	// Absolute,X
#define INSTRUCTION_ASL_ZEROPAGE    0x06	// Zero Page
#define INSTRUCTION_ASL_ZEROPAGEX   0x16	// Zero Page,X

// Branch on Carry Clear
#define INSTRUCTION_BCC_RELATIVE    0x90	// Relative

// Branch on Carry Set
#define INSTRUCTION_BCS_RELATIVE    0xB0	// Relative

// Branch on Result Zero
#define INSTRUCTION_BEQ_RELATIVE    0xF0	// Relative

// Test Bits in Memory with Accumulator
#define INSTRUCTION_BIT_ABSOLUTE    0x2C	// Absolute
#define INSTRUCTION_BIT_ZEROPAGE    0x24	// Zero Page

// Branch on Result Minus
#define INSTRUCTION_BMI_RELATIVE    0x30	// Relative

// Branch on Result Not Zero
#define INSTRUCTION_BNE_RELATIVE    0xD0	// Relative

// Branch on Result Plus
#define INSTRUCTION_BPL_RELATIVE    0x10	// Relative

// Break Command
#define INSTRUCTION_BRK_IMPLIED     0x00	// Implied

// Branch on Overflow Clear
#define INSTRUCTION_BVC_RELATIVE    0x50	// Relative

// Branch on Overflow Set
#define INSTRUCTION_BVS_RELATIVE    0x70	// Relative

// Clear Carry Flag
#define INSTRUCTION_CLC_IMPLIED     0x18	// Implied

// Clear Decimal Mode
#define INSTRUCTION_CLD_IMPLIED     0xD8	// Implied

// Clear Interrupt Disable
#define INSTRUCTION_CLI_IMPLIED     0x58	// Implied

// Clear Overflow Flag
#define INSTRUCTION_CLV_IMPLIED     0xB8	// Implied

// Compare Memory and Accumulator
#define INSTRUCTION_CMP_IMMEDIATE   0xC9	// Immediate
#define INSTRUCTION_CMP_ABSOLUTE    0xCD	// Absolute
#define INSTRUCTION_CMP_ABSOLUTEX   0xDD	// Absolute,X
#define INSTRUCTION_CMP_ABSOLUTEY   0xD9	// Absolute,Y
#define INSTRUCTION_CMP_ZEROPAGE    0xC5	// Zero Page
#define INSTRUCTION_CMP_ZEROPAGEX   0xD5	// Zero Page,X
#define INSTRUCTION_CMP_INDIRECTX   0xC1	// (Zero Page,X)
#define INSTRUCTION_CMP_INDIRECTY   0xD1	// (Zero Page),Y

// TODO: Add opcodes 

// Jump
#define INSTRUCTION_JMP_ABSOLUTE    0x4C	// Absolute
#define INSTRUCTION_JMP_INDIRECT    0x6C	// Indirect

// Jump To Subroutine
#define INSTRUCTION_JSR_ABSOLUTE    0x20	// Absolute

// TODO: Add opcodes 

// Load Accumulator
#define INSTRUCTION_LDA_IMMEDIATE   0xA9	// Immediate
#define INSTRUCTION_LDA_ZEROPAGE    0xA5	// Zero Page
#define INSTRUCTION_LDA_ZEROPAGEX   0xB5	// Zero Page,X
#define INSTRUCTION_LDA_ABSOLUTE    0xAD	// Absolute
#define INSTRUCTION_LDA_ABSOLUTEX   0xBD	// Absolute,X
#define INSTRUCTION_LDA_ABSOLUTEY   0xB9	// Absolute,Y
#define INSTRUCTION_LDA_INDIRECTX   0xA1	// Indirect,X
#define INSTRUCTION_LDA_INDIRECTY   0xB1	// Indirect,Y

// TODO: Add opcodes 

#endif // !CPU_h
