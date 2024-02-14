#include "../hdr/cpu.h"

int main(int argc, char** argv, char** envp)
{
	struct Mem mem;
	struct CPU cpu;

	CPU_Reset(&cpu, &mem);
	CPU_Execute(&cpu, &mem, 2);

	return 0;
}

