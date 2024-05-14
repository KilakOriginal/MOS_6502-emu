/*
 * A lexer, parser and interpreter that allows you to execute assembly code
 * from a file.
 */

#include <ctype.h>

#include "util.h"
#include "runner.h"

const char* token_name(const TokenType type)
{
    switch(type)
    {
        case TOKEN_EOF: return "EOF";
        case TOKEN_NUM: return "NUM";
        case TOKEN_ID: return "ID";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_IMMD: return "IMMEDIATE";
        case TOKEN_HEXNUM: return "HEXADECIMAL";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        default: return "ILLEGAL";
    }
}

TokenList* tokenlist_initialise(size_t capacity)
{
    TokenList* list = malloc(sizeof(TokenList));
    list->capacity = capacity;
    list->current_size = 0;
    list->contents = malloc(sizeof(Token) * capacity);

    return list;
}

void tokenlist_free(TokenList* list)
{
    free(list->contents);
    free(list);
}

const int tokenlist_resize(TokenList* list, const size_t capacity)
{
    Token* new_contents = realloc(list->contents, sizeof(Token) * capacity);
    if (new_contents == NULL)
        return -1;

    list->contents = new_contents;
    list->capacity = capacity;
    if (capacity < list->current_size)
        list->current_size = capacity;

    return 0;
}

const int tokenlist_append(TokenList* list, const Token token)
{
    if (list->capacity <= list->current_size)
        return tokenlist_resize(list, list->capacity * 1.5);

    list->contents[list->current_size] = token;
    list->current_size++;

    return 0;
}

const Token tokenlist_get(TokenList* list, size_t index)
    { return list->contents[index]; }

int is_label_char(const char input)
    { return (isalnum(input) || input == '_'); }

Lexer Lexer_Initialise(const char* contents, const size_t contents_size)
{
    Lexer lexer = {0};
    lexer.contents = contents;
    lexer.contents_size = contents_size;

    return lexer;
}

const char Lexer_Consume(Lexer* lexer)
{
    assert(lexer->position < lexer->contents_size);

    char current_symbol = lexer->contents[lexer->position];
    lexer->position++;

    if (current_symbol == '\n')
    {
        lexer->line++;
        lexer->beginning_of_line = lexer->position;
    }

    return current_symbol;
}

void trim_left(Lexer* lexer)
{
    while(lexer->position < lexer->contents_size 
        && isspace(lexer->contents[lexer->position]))
        (void)Lexer_Consume(lexer);
}

const Token Lexer_Advance(Lexer* lexer)
{
    trim_left(lexer);

    Token token = 
    {
        .value = &lexer->contents[lexer->position],
    };

    if (lexer->position >= lexer->contents_size)
        return token;

    switch (lexer->contents[lexer->position])
    {
        case EOF:
        {
            token.type = TOKEN_EOF;
        } break;
        case ';':
        {
            token.type = TOKEN_COMMENT;

            while (lexer->position < lexer->contents_size
                && lexer->contents[lexer->position] != '\n')
            {
                token.value_size++;
                (void)Lexer_Consume(lexer);
            }
            
            if (lexer->position < lexer->contents_size)
                (void)Lexer_Consume(lexer);

        } break;
        case '#':
        {
            token.type = TOKEN_IMMD;
            
            while (lexer->position < lexer->contents_size
                && !isspace(lexer->contents[lexer->position]))
            {
                token.value_size++;
                (void)Lexer_Consume(lexer);
            }

            if (lexer->position < lexer->contents_size)
                (void)Lexer_Consume(lexer);
        } break;
        case '$':
        {
            token.type = TOKEN_HEXNUM;
            
            while (lexer->position < lexer->contents_size
                && !isspace(lexer->contents[lexer->position]))
            {
                token.value_size++;
                (void)Lexer_Consume(lexer);
            }

            if (lexer->position < lexer->contents_size)
                (void)Lexer_Consume(lexer);
        } break;
        case '(':
        {
            token.type = TOKEN_LPAREN;
            token.value_size = 1;
            lexer->position++;
        } break;
        case ')':
        {
            token.type = TOKEN_RPAREN;
            token.value_size = 1;
            lexer->position++;
        } break;
        default:
        {    
            if (isalpha(lexer->contents[lexer->position])
                || lexer->contents[lexer->position] == '_')
            {
                token.type = TOKEN_ID;
                while (lexer->position < lexer->contents_size 
                    && is_label_char(lexer->contents[lexer->position]))
                {
                    lexer->position++;
                    token.value_size++;
                }

                return token;
            }
            else    // Illegal/undefined
            {
                token.type = TOKEN_INVALID;
                while (lexer->position < lexer->contents_size
                    && !isspace(lexer->contents[lexer->position]))
                {
                    lexer->position++;
                    token.value_size++;
                }

                return token;
            }
        }
    }
    
    return token;
}

TokenList* Lexer_Run(Lexer* lexer)
{
    TokenList* destination = tokenlist_initialise(123);
    if (destination == NULL)
        die("TokenList initialisation failed!");

    Token token = Lexer_Advance(lexer);

    while (token.type != TOKEN_EOF)
	{
		tokenlist_append(destination, token);

		token = Lexer_Advance(lexer);
	}

    return destination;
}
