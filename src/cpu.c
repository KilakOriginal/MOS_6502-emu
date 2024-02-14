#include "../hdr/cpu.h"

void Memory_Initialise(struct Mem* mem)
{
	(void)memset(mem, 0, sizeof(mem->Data));
}

Byte Memory_Fetch(struct CPU* cpu, struct Mem* mem, u32* cycles)
{
	Byte data = Get_Memory(mem, cpu->PC);
	cpu->PC++;
	*cycles -= 1;

	return data;
}

Byte Get_Memory(struct Mem* mem, u32 index)
{
	if (index >= MAX_MEM)
	{
		(void)fprintf(stderr, "Invalid address %d; terminating execution...\n", index);
		exit(EXIT_FAILURE);
	}

	return mem->Data[index];
}

void CPU_Reset(struct CPU* cpu, struct Mem* mem)
{
	cpu->PC = 0xFFFC;	// Set Programme Counter
	cpu->SP = 0x00FF;	// Set Stack Pointer
	cpu->I  = 1;		// Set Interrupt Disable
	cpu->D  = 0;		// Clear Decimal Flag
	Memory_Initialise(mem);
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

	while (cycles > 0)
	{
		instruction = Memory_Fetch(cpu, mem, &cycles);
		(void)printf("Fetched %d\n", instruction);
	}

	free(cycles_remaining);
}

