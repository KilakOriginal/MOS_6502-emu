/*
 * A lexer, parser and interpreter that allows you to execute assembly code
 * from a file.
 */

#include <ctype.h>

#include "../hdr/util.h"
#include "../hdr/runner.h"

const char* token_name(const TokenType type)
{
    switch(type)
    {
        case TOKEN_EOF: return "EOF";
        case TOKEN_NUM: return "NUM";
        case TOKEN_ID: return "ID";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_HASH: return "HASH";
        case TOKEN_DOLLAR: return "DOLLAR";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        default: return "ILLEGAL";
    }
}

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
            token.type = TOKEN_HASH;
            token.value_size = 1;
            lexer->position++;
        } break;
        case '$':
        {
            token.type = TOKEN_DOLLAR;
            token.value_size = 1;
            lexer->position++;
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

void Lexer_Run(Lexer* lexer, Token* destination, const size_t size)
{
    int i = 0;
    int max_space = size / sizeof(Token);
    Token token = Lexer_Advance(lexer);

    while (token.type != TOKEN_EOF)
	{
        if (i >= max_space)
            die("Destination too small!");

		destination[i] = token;
        i++;

		token = Lexer_Advance(lexer);
	}
}
