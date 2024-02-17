#ifndef RUNNER_h
#define RUNNER_h

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    TOKEN_EOF,      // End of File
    TOKEN_NUM,      // Numbers
    TOKEN_CHAR,     // Characters
    TOKEN_SEMCOL,   // ;
    TOKEN_HASH,     // Immediate
    TOKEN_DOLLAR,   // Hexadecimal
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
} TokenType;

typedef struct Token
{
    TokenType type;
    const char* repr;
    size_t repr_length;
} Token;

typedef struct Lexer
{
    const char* contents;
    size_t contents_size;
    size_t position, line, beginning_of_line;
} Lexer;

// Lexer funtions
Lexer Lexer_Initialise(const char* contents, const size_t contents_size);
const Token Lexer_Next(Lexer* lexer);

#endif // !Runner_h
