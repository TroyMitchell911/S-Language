/**
  ******************************************************************************
  * @file           : scanner.h
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/11
  ******************************************************************************
  */
#ifndef __SCANNER_H
#define __SCANNER_H

#include "debug.h"
#include "open_list.h"
#include "utils.h"

typedef void (*scanner_get_next_token_t)(void* scanner);

typedef enum token_type {
    TOKEN_INT = 1,
    TOKEN_CHAR,
    TOKEN_FLOAT,
    TOKEN_VOID,

    TOKEN_CONST,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_THEN,
    TOKEN_WHILE,
    TOKEN_SWITCH,
    TOKEN_BREAK,
    TOKEN_BEGIN,
    TOKEN_END,

    /* 15 16保留 */

    TOKEN_ID = 17,
    TOKEN_NUM,

    TOKEN_ADD,		        //'+'
    TOKEN_SUB,		        //'-'
    TOKEN_MUL,		        //'*'
    TOKEN_DIV,		        //'/'
    TOKEN_MOD,		        //'%'

    TOKEN_LEFT_PAREN,	    //'('
    TOKEN_RIGHT_PAREN,	    //')'
    TOKEN_LEFT_BRACKET,	    //'['
    TOKEN_RIGHT_BRACKET,    //']'

    /* 28保留 */

    TOKEN_LESS = 29,        //'<'
    TOKEN_GREATER,          //'>'
    TOKEN_LESS_EQUAL,       //'<='
    TOKEN_GREATER_EQUAL,    //'>='
    TOKEN_EQUAL_EQUAL,      //'=='
    TOKEN_NOT_EQUAL,        //'!='

    TOKEN_COMMA,		    //','
    TOKEN_SEMICOLON,        //';'

    TOKEN_DIV_EQUAL,        //'/='
    TOKEN_ADD_EQUAL,        //'+='
    TOKEN_SUB_EQUAL,        //'-='
    TOKEN_MUL_EQUAL,        //'*='
    TOKEN_MOD_EQUAL,        //'%='

    TOKEN_LOGIC_ORR,        //'||'
    TOKEN_LOGIC_AND,        //'&&'
    TOKEN_NOT,        //'!'

    TOKEN_EQUAL,            //'='

    TOKEN_EOF,

    TOKEN_UNKNOWN
}token_type_t;

#ifdef ALL_STEPS_INDEPENDENCE
typedef struct symbol_table {
    char* name;
    void* type;
    void* address;
}symbol_table_t;
typedef struct constant_table {
    char* name;
    long value;
}constant_table_t;
#endif

typedef struct token {
    token_type_t type;
    const char* start;
    uint32_t len;
    uint32_t line_num;
#ifdef ALL_STEPS_INDEPENDENCE
    open_list list;
#endif
}token_t;

typedef struct scanner {
    const char* file_path;
    char* source_code;
    char cur_char;
    char* next_char_ptr;
    token_t prev_token;
    token_t cur_token;
    int errors_num;
    int warnings_num;

    scanner_get_next_token_t get_next_token;
    void (*get_next_token_init)(void* arg);
    /* 此函数仅用于测试阶段 */
#ifdef ALL_STEPS_INDEPENDENCE
    void (*get_all_token)(void* scanner);
#endif
}scanner_t;



scanner_t* scanner_new(const char* scanner_mode, const char* file_path);
void scanner_delete(scanner_t* scanner);
#endif