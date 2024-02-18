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
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_SEMCOL: return "SEMICOLON";
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

const Token Lexer_Advance(Lexer* lexer)
{
    // TODO: trim leading ws

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
            token.type = TOKEN_SEMCOL;
            token.value_size = 1;
            lexer->position++;
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
                token.type = TOKEN_CHAR;
                while (lexer->position < lexer->contents_size 
                    && is_label_char(lexer->contents[lexer->position]))
                {
                    lexer->position++;
                    token.value_size++;
                }

                return token;
            }

            else
                die("Illegal Character '%s'",
                    lexer->contents[lexer->position]);
        }
    }
    
    return token;
}
