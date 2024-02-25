#ifndef RUNNER_h
#define RUNNER_h

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    TOKEN_EOF,      // End of File
    TOKEN_NUM,      // Numbers
    TOKEN_ID,       // Identifiers
    TOKEN_COMMENT,  // ;<Text>
    TOKEN_IMMD,     // Immediate
    TOKEN_HEXNUM,   // Hexadecimal
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_INVALID,
} TokenType;

typedef struct Token
{
    TokenType type;
    const char* value;
    size_t value_size;
} Token;

typedef struct TokenList
{
    size_t capacity;
    size_t current_size;
    Token* contents;
} TokenList;

typedef struct Lexer
{
    const char* contents;
    size_t contents_size;
    size_t position, line, beginning_of_line;
} Lexer;


// Token functions
const char* token_name(const TokenType type);


// Token List functions
TokenList* tokenlist_initialise(size_t capacity);
void tokenlist_free(TokenList* list);
const int tokenlist_resize(TokenList* list, const size_t capacity);
const int tokenlist_append(TokenList* list, const Token token);
const Token tokenlist_get(TokenList* list, const size_t index);


// Lexer funtions
Lexer Lexer_Initialise(const char* contents, const size_t contents_size);
const Token Lexer_Advance(Lexer* lexer);
TokenList* Lexer_Run(Lexer* lexer);

#endif // !RUNNER_h
