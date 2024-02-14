#include "../hdr/cpu.h"

int main(int argc, char** argv, char** envp)
{
	struct Mem mem;
	struct CPU cpu;
	int err = 0;

	CPU_Reset(&cpu, &mem);

	cpu.X = 0x0002;
	err += Set_Memory(&mem, 0xFFFC, INSTRUCTION_LDA_ZEROPAGEX);
	err += Set_Memory(&mem, 0xFFFD, 0x0040);
	err += Set_Memory(&mem, 0x0042, 0x0084);

	if (err != 0)
	{
		(void)perror("Memset failed");
		exit(EXIT_FAILURE);
	}

	CPU_Execute(&cpu, &mem, 4);

	(void)printf("Accumulator: %d\n", cpu.A);

	return 0;
}

