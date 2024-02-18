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
    const char* value;
    size_t value_size;
} Token;

typedef struct Lexer
{
    const char* contents;
    size_t contents_size;
    size_t position, line, beginning_of_line;
} Lexer;


// Token functions
const char* token_name(const TokenType type);

// Lexer funtions
Lexer Lexer_Initialise(const char* contents, const size_t contents_size);
const Token Lexer_Advance(Lexer* lexer);

#endif // !RUNNER_h
