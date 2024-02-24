/*
 * Author:··········Malik Abdoul Hamidou
 * Date Created:····2024-02-14
 * Date Changed:····2024-02-23
 * Description:·····A small sample programme, demonstrating the emulator
 * License:·········GPL3 (See LICENCE)
*/

#include <ctype.h>

#include "../hdr/cpu.h"
#include "../hdr/runner.h"
#include "../hdr/util.h"

int main(int argc, char** argv, char** envp)
{
	Mem mem;
	CPU cpu;
	int err = 0;

	MOS_6502_set_endianness(AUTO);

	CPU_Reset(&cpu, &mem);

	cpu.X = 0x0002;
    err += Set_Memory(&mem, 0xFFFC, INSTRUCTION_JSR_ABSOLUTE);
    err += Set_Memory(&mem, 0xFFFD, 0x11);
    err += Set_Memory(&mem, 0xFFFE, 0x11);
	err += Set_Memory(&mem, 0x1111, INSTRUCTION_JMP_ABSOLUTE);
	err += Set_Memory(&mem, 0x1112, 0xFC);
	err += Set_Memory(&mem, 0x1113, 0xEF);
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

	CPU_Execute(&cpu, &mem, 16);

	(void)printf("Accumulator: %d\n", cpu.A);
	(void)printf("Carry: %d\nOverflow: %d\nZero: %d\nNegative: %d\n", cpu.C, cpu.V, cpu.Z, cpu.N);

	/*const char* test = "_label  #48 ;A Comment\nNewLine $'zusasdfasdfaw4534tdgr dsf";
	Lexer lexer = Lexer_Initialise(test, strlen(test));

	TokenList* tokens = Lexer_Run(&lexer);

	for (int i = 0; i < tokens->current_size; i++)
		(void)printf("%.*s (%s)\n", 
		(int) tokenlist_get(tokens, i).value_size,
		tokenlist_get(tokens, i).value,
		token_name(tokenlist_get(tokens, i).type));

	free(tokens);*/

	return 0;
}
