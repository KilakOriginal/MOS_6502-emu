#include <stdarg.h>
#include "../hdr/cpu.h"

void die(char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	(void)fprintf(stderr, format, argptr);
	va_end(argptr);

	exit(EXIT_FAILURE);
}

void Memory_Initialise(struct Mem* mem)
	{ (void)memset(mem, 0, sizeof(mem->Data)); }

Byte Memory_Read_Byte(struct CPU* cpu, struct Mem* mem, u32* cycles, Byte address)
{
	Byte data = Get_Memory(mem, address);
	*cycles -= 1;

	return data;
}

Byte Memory_Fetch_Byte(struct CPU* cpu, struct Mem* mem, u32* cycles)
{
	Byte data = Get_Memory(mem, cpu->PC);
	cpu->PC++;
	*cycles -= 1;

	return data;
}

int validate_index(u32 index)
	{ return (index >= 0 && index < MAX_MEM); }

Byte Get_Memory(struct Mem* mem, u32 index)
{
	if (!validate_index(index))
		die("Invalid address '%d'", index);
	return mem->Data[index];
}

int Set_Memory(struct Mem* mem, u32 index, u32 data)
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
	Memory_Initialise(mem);
}

void lda_set_flags(struct CPU* cpu)
{
	cpu->Z = (cpu->A == 0);
	cpu->N = (cpu->A & 64) != 0; //7th bit set
}

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
		(void)printf("Reading %d...\n", cpu->PC);

		instruction = Memory_Fetch_Byte(cpu, mem, cycles_remaining);

		switch(instruction)
		{
			case INSTRUCTION_LDA_IMMEDIATE:
			{
				cpu->A = 
					Memory_Fetch_Byte(cpu, 
						          mem,
						          cycles_remaining);
				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Immediate\n");
			} break;
			case INSTRUCTION_LDA_ZEROPAGE:
			{
				Byte zero_page_address = 
					Memory_Fetch_Byte(cpu, 
							  mem, 
							  cycles_remaining);
				cpu->A = Memory_Read_Byte(cpu,
						          mem,
						          cycles_remaining,
						          zero_page_address);
				lda_set_flags(cpu);

				// DEBUG
				(void)printf("Executed LDA Zero Page\n");
			} break;
			case INSTRUCTION_LDA_ZEROPAGEX:
				break;
			case INSTRUCTION_LDA_ABSOLUTE:
				break;
			case INSTRUCTION_LDA_ABSOLUTEX:
				break;
			case INSTRUCTION_LDA_ABSOLUTEY:
				break;
			case INSTRUCTION_LDA_INDIRECTX:
				break;
			case INSTRUCTION_LDA_INDIRECTY:
				break;
			default:
			{
				(void)fprintf(stderr,
					      "Illegal instruction '%d'@%d\n",
					      instruction,
					      cpu->PC);
			} break;
		}

	}

	free(cycles_remaining);
}

