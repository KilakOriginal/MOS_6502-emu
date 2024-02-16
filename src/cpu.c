#include <stdarg.h>
#include <assert.h>
#include "../hdr/cpu.h"

static int endianness = BIG;
static int err = 0;

void die(char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	(void)fprintf(stderr, format, argptr);
	va_end(argptr);

	exit(EXIT_FAILURE);
}

/**
 * @brief Set the endianness to correctly represent the 6502's memory
 * 
 * The MOS 6502 uses little endian numbers so if the system running this code
 * is big endian, the LSB and MSB need to be reversed.
 * Invalid arguments will result in termination.
 * 
 * @param arg one of BIG=0, LITTLE=1, AUTO=2; sets endianness to the system's00000000
*/
void MOS_6502_set_endianness(int arg)
{
	if (arg == AUTO)
	{
		int i = 1;
		if (*((char *)&i) == 1)
			endianness = LITTLE;
	}

	else if (arg == BIG || arg == LITTLE)
		endianness = arg;

	else
		die("Invalid endianness; must be one of BIG=0, LITTLE=1, AUTO=2");
}

Byte is_sign_set(Byte input)
	{ return input >> 7; }

void Mem_Initialise(struct Mem* mem)
	{ (void)memset(mem, 0, sizeof(mem->Data)); }

Byte Mem_Read_Byte(struct CPU* cpu, struct Mem* mem, u32* cycles, Byte address)
{
	Byte data = Get_Memory(mem, address);
	*cycles -= 1;

	return data;
}

Byte Mem_Read_Word(struct CPU* cpu, struct Mem* mem, u32* cycles, Byte address)
{
	Word data = Mem_Read_Byte(cpu, mem, cycles, address);
	if (endianness == LITTLE)
		data |= (Mem_Read_Byte(cpu, mem, cycles, address + 1) << 0x08);
	else
	{
		data <<= 0x08;
		data |= Mem_Read_Byte(cpu, mem, cycles, address + 1);
	}

	return data;
}

Byte Mem_Fetch_Byte(struct CPU* cpu, struct Mem* mem, u32* cycles)
{
	Byte data = Get_Memory(mem, cpu->PC);
	cpu->PC++;
	*cycles -= 1;

	return data;
}

Word Mem_Fetch_Word(struct CPU* cpu, struct Mem* mem, u32* cycles)
{
	Word data = Mem_Fetch_Byte(cpu, mem, cycles);
	if (endianness == LITTLE)
	     data |= (Mem_Fetch_Byte(cpu, mem, cycles) << 0x08);
	else
	{
		data <<= 0x08;
		data |= (Mem_Fetch_Byte(cpu, mem, cycles));
	}

	return data;
}

Byte CPU_Fetch_Register(struct CPU* cpu, Byte reg, u32* cycles)
{
	*cycles -= 1;
	return reg;
}

int validate_index(Word index)
	{ 
		assert(MAX_MEM >= sizeof(Word));
		return (index >= 0);
	}

Byte Get_Memory(struct Mem* mem, Word index)
{
	if (!validate_index(index))
		die("Invalid address '%d'", index);
	return mem->Data[index];
}

int Set_Memory(struct Mem* mem, Word index, Byte data)
{
	if (!validate_index(index))
		return -1;
	
	mem->Data[index] = data;
	
	return 0;
}

void CPU_Reset(struct CPU* cpu, struct Mem* mem)
{
	cpu->PC = 0xFFFC;	// Set Programme Counter
	cpu->SP = 0x00FF;	// Set Stack Pointer
	cpu->I  = 1;		// Set Interrupt Disable
	cpu->D  = 0;		// Clear Decimal Flag
	Mem_Initialise(mem);
}

// Flags
void adc_set_flags(struct CPU* cpu, Byte input, Word sum)
{
	cpu->C = (sum & 0xFF00) != 0;
	cpu->Z = (cpu->A == 0);
	cpu->V = (~((cpu->N << 7) ^ input) & (input ^ sum)) >> 7;
	cpu->N = is_sign_set(cpu->A);
}

void lda_set_flags(struct CPU* cpu)
{
	cpu->Z = (cpu->A == 0);
	cpu->N = is_sign_set(cpu->A);
}

// ...

// Addressing Modes
// ...

/**
 * @brief This function will emulate a MOS 6502 on virtual/simulated memory.
 * 
 * The individual instructions are fetched from memory and then interpreted.
 * If there are't enough cycles left to execute an instruction, the cpu will
 * crash.
 * TODO: Think: Perhaps the CPU should just stop mid execution?
 * Invalid memory addresses will result in termination, overflows are wrapped.
 * 
 * @param cpu the cpu you want to emulate
 * @param mem the memory on which the cpu will run
 * @param cycles the number of cycles for which you allow the cpu to run
*/
void CPU_Execute(struct CPU* cpu, struct Mem* mem, u32 cycles)
{
	Byte instruction;
	u32* cycles_remaining = malloc(sizeof(u32));

	if (cycles_remaining == NULL)
	{
		(void)perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	*cycles_remaining = cycles;

	while (*cycles_remaining > 0)
	{
		//DEBUG
		(void)printf("Reading %d. Cycles remaining: %d\n", cpu->PC, *cycles_remaining);

		instruction = Mem_Fetch_Byte(cpu, mem, cycles_remaining);

		switch(instruction)
		{
			// ADC
			case INSTRUCTION_ADC_IMMEDIATE:
			{
				assert(*cycles_remaining >= 2-1);

				Byte input = Mem_Fetch_Byte(cpu, mem, cycles_remaining);
				Word sum = (cpu->A + input + cpu->C);

				cpu->A = sum & 0xFF;

				adc_set_flags(cpu, input, sum);
			} break;


			//JMP
			case INSTRUCTION_JMP_ABSOLUTE:
			{
				assert(*cycles_remaining >= 3-1);

				Word jmp_addr = Mem_Fetch_Word(cpu, mem, cycles_remaining);
				if (validate_index(jmp_addr))
					cpu->PC = jmp_addr;
				else
					die("Illegal Jump Address '%d'", jmp_addr);
				
				// DEBUG
				(void)printf("Executed JMP Absolute to %d\n", jmp_addr);
			} break;
			case INSTRUCTION_JMP_INDIRECT:
			{
				assert(*cycles_remaining >= 5-1);
				
				// DEBUG
				(void)printf("Executed JMP Indirect\n");
			} break;


			// JSR
			case INSTRUCTION_JSR_ABSOLUTE:
			{
				assert(*cycles_remaining >= 6-1);
				
				// DEBUG
				(void)printf("Executed JSR Absolute\n");
			} break;


			// LDA
			case INSTRUCTION_LDA_IMMEDIATE:
			{
				assert(*cycles_remaining >= 2-1);

				cpu->A = 
					Mem_Fetch_Byte(cpu, 
						       mem,
						       cycles_remaining);
				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Immediate\n");
			} break;
			case INSTRUCTION_LDA_ZEROPAGE:
			{
				assert(*cycles_remaining >= 3-1);

				Byte zero_page_address = 
					Mem_Fetch_Byte(cpu, 
						       mem, 
						       cycles_remaining);
				cpu->A = Mem_Read_Byte(cpu,
						       mem,
						       cycles_remaining,
						       zero_page_address);
				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Zero Page\n");
			} break;
			case INSTRUCTION_LDA_ZEROPAGEX:
			{
				assert(*cycles_remaining >= 4-1);

				Byte offset = CPU_Fetch_Register(cpu,
					                         cpu->X,
								 cycles_remaining);
				Byte zero_page_address =
					(Mem_Fetch_Byte(cpu,
							mem,
							cycles_remaining)
					+ offset) % sizeof(mem->Data);
				cpu->A = Mem_Read_Byte(cpu,
						       mem,
						       cycles_remaining,
						       zero_page_address);

				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Zero Page,X\n");
			} break;
			case INSTRUCTION_LDA_ABSOLUTE:
			{
				assert(*cycles_remaining >= 4-1);
				//...
				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Absolute");
			} break;
			case INSTRUCTION_LDA_ABSOLUTEX:
			{
				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Absolute,X");
			} break;
			case INSTRUCTION_LDA_ABSOLUTEY:
			{
				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Absolute,Y");
			} break;
			case INSTRUCTION_LDA_INDIRECTX:
			{
				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Indirect,X");
			} break;
			case INSTRUCTION_LDA_INDIRECTY:
			{
				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Indirect,Y");
			} break;
			default:
			{
				(void)fprintf(stderr,
					      "Illegal instruction '%d'@%d\n",
					      instruction,
					      cpu->PC);
				err++;
				if (err >= MAX_ERRORS)
					die("Critical Failure Detected! Aborting...\n");
			} break;
		}

	}

	free(cycles_remaining);
}

