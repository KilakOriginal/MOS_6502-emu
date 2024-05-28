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

void Mem_Write_word(Mem* mem,
                    u32* cycles,
                    const Word word,
                    const Word address);

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

// Compare X Register
#define INSTRUCTION_CPX_IMMEDIATE   0xE0    // Immediate
#define INSTRUCTION_CPX_ZEROPAGE    0xE4    // Zero Page
#define INSTRUCTION_CPX_ABSOLUTE    0xEC    // Absolute

// Compare Y Register
#define INSTRUCTION_CPY_IMMEDIATE   0xC0    // Immediate
#define INSTRUCTION_CPY_ZEROPAGE    0xC4    // Zero Page
#define INSTRUCTION_CPY_ABSOLUTE    0xCC    // Absolute

// Decrement Memory
#define INSTRUCTION_DEC_ZEROPAGE    0xC6    // Zero Page
#define INSTRUCTION_DEC_ZEROPAGEX   0xD6    // Zero Page,X
#define INSTRUCTION_DEC_ABSOLUTE    0xCE    // Absolute
#define INSTRUCTION_DEC_ABSOLUTEX   0xDE    // Absolute,X

// Bitwise Exclusive Or
#define INSTRUCTION_EOR_IMMEDIATE   0x49    // Immediate
#define INSTRUCTION_EOR_ZEROPAGE    0x45    // Zero Page
#define INSTRUCTION_EOR_ZEROPAGEX   0x55    // Zero Page X
#define INSTRUCTION_EOR_ABSOLUTE    0x4D    // Absolute
#define INSTRUCTION_EOR_ABSOLUTEX   0x5D    // Absolute X
#define INSTRUCTION_EOR_ABSOLUTEY   0x59    // Absolute Y
#define INSTRUCTION_EOR_INDIRECTX   0x41    // Indirect X
#define INSTRUCTION_EOR_INDIRECTY   0x51    // Indirect Y

// Flag Instructions
#define INSTRUCTION_CLC             0x18    // Clear Carry
#define INSTRUCTION_SEC             0x38    // Set Carry
#define INSTRUCTION_CLI             0x58    // Clear Input
#define INSTRUCTION_SEI             0x78    // Set Interrupt
#define INSTRUCTION_CLV             0xB8    // Cleaer Overflow
#define INSTRUCTION_CLD             0xD8    // Clear Decimal
#define INSTRUCTION_SED             0xF8    // Set Decimal

// Increment Memory
#define INSTRUCTION_INC_ZEROPAGE    0xE6    // Zero Page
#define INSTRUCTION_INC_ZEROPAGEX   0xF6    // Zero Page,X
#define INSTRUCTION_INC_ABSOLUTE    0xEE    // Absolute
#define INSTRUCTION_INCABSOLUTEX    0xFE    // Absolute,X

// Jump
#define INSTRUCTION_JMP_ABSOLUTE    0x4C	// Absolute
#define INSTRUCTION_JMP_INDIRECT    0x6C	// Indirect

// Jump To Subroutine
#define INSTRUCTION_JSR_ABSOLUTE    0x20	// Absolute

// Load Accumulator
#define INSTRUCTION_LDA_IMMEDIATE   0xA9	// Immediate
#define INSTRUCTION_LDA_ZEROPAGE    0xA5	// Zero Page
#define INSTRUCTION_LDA_ZEROPAGEX   0xB5	// Zero Page,X
#define INSTRUCTION_LDA_ABSOLUTE    0xAD	// Absolute
#define INSTRUCTION_LDA_ABSOLUTEX   0xBD	// Absolute,X
#define INSTRUCTION_LDA_ABSOLUTEY   0xB9	// Absolute,Y
#define INSTRUCTION_LDA_INDIRECTX   0xA1	// Indirect,X
#define INSTRUCTION_LDA_INDIRECTY   0xB1	// Indirect,Y

// Load X Register
#define INSTRUCTION_LDX_IMMEDIATE   0xA2    // Immediate
#define INSTRUCTION_LDX_ZEROPAGE    0xA6    // Zero Page
#define INSTRUCTION_LDX_ZEROPAGEY   0xB6    // Zero Page,Y
#define INSTRUCTION_LDX_ABSOLUTE    0xAE    // Absolute
#define INSTRUCTION_LDX_ABSOLUTEY   0xBE    // Absolute,Y

// Load Y Register
#define INSTRUCTION_LDY_IMMEDIATE   0xA0    // Immediate
#define INSTRUCTION_LDY_ZEROPAGE    0xA4    // Zero Page
#define INSTRUCTION_LDY_ZEROPAGEX   0xB4    // Zero Page,Y
#define INSTRUCTION_LDY_ABSOLUTE    0xAC    // Absolute
#define INSTRUCTION_LDY_ABSOLUTEX   0xBC    // Absolute,Y

// Logical Shift Right
#define INSTRUCTION_LSR_ACCUMULATOR 0x4A    // Accumulator
#define INSTRUCTION_LSR_ZEROPAGE    0x46    // Zero Page
#define INSTRUCTION_LSR_ZEROPAGEX   0x56    // Zero Page,X
#define INSTRUCTION_LSR_ABSOLUTE    0x4E    // Absolute
#define INSTRUCTION_LSR_ABSOLUTEX   0x5E    // Absolute,X

// No Operation
#define INSTRUCTION_NOP_IMPLIED     0xEA    // Implied

// Bitwise Or with Accumulator
#define INSTRUCTION_ORA_IMMEDIATE   0x09    // Immedeate
#define INSTRUCTION_ORA_ZEROPAGE    0x05    // Zero Page
#define INSTRUCTION_ORA_ZEROPAGEX   0x15    // Zero Page,X
#define INSTRUCTION_ORA_ABSOLUTE    0x0D    // Absolute
#define INSTRUCTION_ORA_ABSOLUTEX   0x1D    // Absolute,X
#define INSTRUCTION_ORA_ABSOLUTEY   0x19    // Absolute,Y
#define INSTRUCTION_ORA_INDIRECTX   0x01    // Indirect,X
#define INSTRUCTION_ORA_INDIRECTY   0x11    // Indirect,Y

// Register Instructions

// Rotate Left

// Rotate Right

// Return from Interrupt

// Return from Interrupt

// Return from Subroutine

// Subtract with Carry

// Store Accumulator

// Stack Instructions

// Store X Register

// Store Y Register

#endif // !CPU_h
