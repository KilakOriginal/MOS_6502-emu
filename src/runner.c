/*
 * A lexer, parser and interpreter that allows you to execute assembly code
 * from a file.
 */

#include <ctype.h>

#include "../hdr/runner.h"

int is_label_char(const char input)
    { return (isalnum(input) || input == '_'); }

Lexer Lexer_Initialise(const char* contents, const size_t contents_size)
{
    Lexer lexer = {0};
    lexer.contents = contents;
    lexer.contents_size = contents_size;

    return lexer;
}

const Token Lexer_Next(Lexer* lexer)
{
    // TODO: trim leading ws

    Token token = 
    {
        .repr = &lexer->contents[lexer->position],
    };

    if (lexer->position >= lexer->contents_size)
        return token;
    
    // Special characters
    switch (lexer->contents[lexer->position])
    {
        case EOF:
        {
            token.type = TOKEN_EOF;
        } break;
        case ';':
        {
            token.type = TOKEN_SEMCOL;
            token.repr_length = 1;
            lexer->position++;
        } break;
        case '#':
        {
            token.type = TOKEN_HASH;
            token.repr_length = 1;
            lexer->position++;
        } break;
        case '$':
        {
            token.type = TOKEN_DOLLAR;
            token.repr_length = 1;
            lexer->position++;
        } break;
        case '(':
        {
            token.type = TOKEN_LPAREN;
            token.repr_length = 1;
            lexer->position++;
        } break;
        case ')':
        {
            token.type = TOKEN_RPAREN;
            token.repr_length = 1;
            lexer->position++;
        } break;
        default:
            break;
    }

    // Labels
    if (isalpha(lexer->contents[lexer->position])
        || lexer->contents[lexer->position] == '_')
    {
        token.type = TOKEN_CHAR;
        while (lexer->position < lexer->contents_size 
            && is_label_char(lexer->contents[lexer->position]))
        {
            lexer->position++;
            token.repr_length++;
        }

        return token;
    }

    return token;
}
