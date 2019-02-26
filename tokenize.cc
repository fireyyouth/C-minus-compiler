#include <cctype>
#include <cstdint>
#include <stdexcept>
#include "tokenize.h"

#define DEBUG
#include "debug.h"

using namespace std;
#define assign_token_and_consume(_type, n) \
    do {    \
        token->type = (_type);\
        token->content = string((const char *)(t), (n));\
        t += n; \
    } while(0)

#define throw_error()   \
    throw runtime_error(string(__func__) + ":" + to_string(__LINE__));

const uint8_t * tokenize_one(const uint8_t * s, const uint8_t * e, Token * token) {

    while (s < e && isspace(*s)) {
        ++s; 
    }
    if (s >= e) {
        token->type = NONE; 
        return s;
    }

    const uint8_t * t = s;

    if (isdigit(*t)) {
        ++t;
        while (t < e && isdigit(*t)) {
            ++t;    
        }
        token->content = string((const char *)s, t - s);
        token->type = NUM;
        debug("return %.*s\n", t - s, s);
        return t;
    } else if (isalpha(*t)) {
        ++t;
        while (t < e && (isdigit(*t) || isalpha(*t) || *t == '_')) {
            ++t;    
        }
        token->content = string((const char *)s, t - s);
        if (token->content == "def") {
            token->type = DEF; 
        } else if (token->content == "read") {
            token->type = RD; 
        } else if (token->content == "write") {
            token->type = WR; 
        } else if (token->content == "if") {
            token->type = IF; 
        } else if (token->content == "else") {
            token->type = ELSE; 
        } else if (token->content == "while") {
            token->type = WHILE; 
        } else {
            token->type = ID; 
        }
        debug("return %.*s\n", t - s, s);
        return t;
    }

    switch(*t) {
    case '+':
    case '-':
        assign_token_and_consume(ADD_OP, 1);
        break;
    case '*':
    case '/':
    case '%':
        assign_token_and_consume(MUL_OP, 1);
        break;
    case '&':
        if (t + 1 < e && *(t + 1) == '&') {
            assign_token_and_consume(BOOL_OP, 2);
        } else {
            throw_error();
        }
        break;
    case '|':
        if (t + 1 < e && *(t + 1) == '|') {
            assign_token_and_consume(BOOL_OP, 2);
        } else {
            throw_error();
        }
        break;
    case '(':
        assign_token_and_consume(LP, 1);
        break;
    case ')':
        assign_token_and_consume(RP, 1);
        break;
    case ',':
        assign_token_and_consume(COMMA, 1);
        break;
    case '{':
        assign_token_and_consume(LBRACE, 1);
        break;
    case '}':
        assign_token_and_consume(RBRACE, 1);
        break;
    case ';':
        assign_token_and_consume(SEMICOLON, 1);
        break;
    case '<':
    case '>':
        if ((t + 1) < e && *(t + 1) == '=') {
            assign_token_and_consume(CMP_OP, 2);
        } else {
            assign_token_and_consume(CMP_OP, 1);
        }
        break;
    case '=':
        if ((t + 1) < e && *(t + 1) == '=') {
            assign_token_and_consume(CMP_OP, 2);
        } else {
            assign_token_and_consume(EQUAL, 1);
        }
        break;
    case '!':
        if (t + 1 < e && *(t + 1) == '=') {
            assign_token_and_consume(CMP_OP, 2);
        } else {
            throw_error();
        }
        break;
    default:
        debug("error:%c[%x]\n", *t, *t);
        throw_error();
    }
    debug("return %c[%x]\n", *t, *t);
    return t; 
}

