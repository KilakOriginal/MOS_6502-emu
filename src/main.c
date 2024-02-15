#include "../hdr/cpu.h"

int main(int argc, char** argv, char** envp)
{
	struct Mem mem;
	struct CPU cpu;
	int err = 0;

	MOS_6502_set_endianness(AUTO);

	CPU_Reset(&cpu, &mem);

	cpu.X = 0x0002;
	err += Set_Memory(&mem, 0xFFFC, INSTRUCTION_JMP_ABSOLUTE);
	err += Set_Memory(&mem, 0xFFFD, 0x00FC);
	err += Set_Memory(&mem, 0xFFFE, 0x00EF);
	err += Set_Memory(&mem, 0xEFFC, INSTRUCTION_LDA_ZEROPAGEX);
	err += Set_Memory(&mem, 0xEFFD, 0x0040);
	err += Set_Memory(&mem, 0x0042, 0x0084);
	err += Set_Memory(&mem, 0x0000, 0xFF00);

	if (err != 0)
	{
		(void)perror("Memset failed");
		exit(EXIT_FAILURE);
	}

	CPU_Execute(&cpu, &mem, 7);

	(void)printf("Accumulator: %d\n", cpu.A);

	return 0;
}

