#include "../hdr/util.h"

void die(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	(void)fprintf(stderr, format, argptr);
	va_end(argptr);

	exit(EXIT_FAILURE);
}
