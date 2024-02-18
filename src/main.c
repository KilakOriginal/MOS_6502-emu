/*
 * Author:··········Malik Abdoul Hamidou
 * Date Created:····2024-02-14
 * Date Changed:····2024-02-17
 * Description:·····A small sample programme, demonstrating the emulator
 * License:·········GPL3 (See LICENCE)
*/

#include <ctype.h>

#include "../hdr/cpu.h"
#include "../hdr/runner.h"

int main(int argc, char** argv, char** envp)
{
	Mem mem;
	CPU cpu;
	int err = 0;

	MOS_6502_set_endianness(AUTO);

	CPU_Reset(&cpu, &mem);

	cpu.X = 0x0002;
	err += Set_Memory(&mem, 0xFFFC, INSTRUCTION_JMP_ABSOLUTE);
	err += Set_Memory(&mem, 0xFFFD, 0xFC);
	err += Set_Memory(&mem, 0xFFFE, 0xEF);
	err += Set_Memory(&mem, 0xEFFC, INSTRUCTION_LDA_ZEROPAGEX);
	err += Set_Memory(&mem, 0xEFFD, 0x40);
	err += Set_Memory(&mem, 0x0042, 0xff);
	err += Set_Memory(&mem, 0xEFFF, INSTRUCTION_ADC_IMMEDIATE);
	err += Set_Memory(&mem, 0xF000, 0x05);

	if (err != 0)
	{
		(void)perror("Memset failed");
		exit(EXIT_FAILURE);
	}

	CPU_Execute(&cpu, &mem, 10);

	(void)printf("Accumulator: %d\n", cpu.A);
	(void)printf("Carry: %d\nOverflow: %d\nZero: %d\nNegative: %d\n", cpu.C, cpu.V, cpu.Z, cpu.N);

	const char* test = "_label  #;A Comment\nNewLine $";
	Lexer lexer = Lexer_Initialise(test, strlen(test));
	Token token;
	token = Lexer_Advance(&lexer);
	while (token.type != TOKEN_EOF)
	{
		(void)printf("%.*s (%s)\n", (int) token.value_size, token.value, token_name(token.type));
		token = Lexer_Advance(&lexer);
	}

	return 0;
}
