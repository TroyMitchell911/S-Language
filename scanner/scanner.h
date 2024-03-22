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

#include <string.h>
#include "debug.h"
#include "utils.h"
#include "binary_search_string_tree.h"


#define SCANNER_REPORT(type, scanner, format, ...) \
    printf("%s:%u " type ":" format"\r\n", scanner->file_path, \
            scanner->cur_token.line_num,                   \
            ##__VA_ARGS__);                                 \
    if(strcmp(type,"error") == 0)  scanner->errors_num ++;     \
    else if(strcmp(type, "warning") == 0)  scanner->warnings_num ++;

/* 定义获取下一个token指针函数类型 */
typedef void (*scanner_get_next_token_t)(void* scanner);

/* 定义token的类型 */
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
    TOKEN_NOT,              //'!'

    TOKEN_EQUAL,            //'='

    TOKEN_EOF,

    TOKEN_UNKNOWN
}token_type_t;

#ifdef ALL_STEPS_INDEPENDENCE
/* 用户标识符表的结构 type和address我也不知道干嘛的 文档是这样写的 可能未来有用 */
typedef struct user_table_table {
    void* type;
    void* address;
}user_id_table_t;
/* 常量表的结构 value我也不知道干嘛的 文档是这样写的 可能未来有用 */
typedef struct constant_table {
    long value;
}constant_table_t;
#endif

/* token的数据结构 */
typedef struct token {
    /* 该token的类型 */
    token_type_t type;
    /* 通过token起始地址和长度获取token字符串 不用每一个token都申请内存 */
    /* token起始地址 */
    const char* start;
    /* token长度 */
    uint32_t len;
    /* token所在的行数 */
    uint32_t line_num;
}token_t;

/* 分词器的数据结构 */
typedef struct scanner {
    /* 所分词的文件的路径 */
    const char* file_path;
    /* 读取出来源码存放的指针 */
    char* source_code;
    /* 分词器读源码的每一个字符都会放到这个变量 */
    char cur_char;
    /* 下一个要获取的字符的地址 */
    char* next_char_ptr;
    /* 指向上一个获取token */
    token_t prev_token;
    /* 当前获取的token */
    token_t cur_token;
    /* 错误数量 */
    int errors_num;
    /* 警告数量 */
    int warnings_num;

    /* 获取下一个token的方法 */
    scanner_get_next_token_t get_next_token;
    /* 获取下一个token之前需要调用的初始化函数 */
    void (*get_next_token_init)(void* arg);
    /* 此函数仅用于测试阶段 */
#ifdef ALL_STEPS_INDEPENDENCE
    /* 用户标识符表所使用的树根节点 */
    bsst_t *user_id_table_tree;
    /* 常量表所使用的树根节点 */
    bsst_t *constant_table_tree;
    /* 获取所有token方法 */
    void (*get_all_token)(void* scanner);
#endif
}scanner_t;



scanner_t* scanner_new(const char* scanner_mode, const char* file_path);
void scanner_delete(scanner_t* scanner);
#endif