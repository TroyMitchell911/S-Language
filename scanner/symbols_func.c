/**
  ******************************************************************************
  * @file           : symbols_func.c
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/11
  ******************************************************************************
  */
#include "scanner.h"
#include "utils.h"

#define SYMBOL_AND_EQUAL_METHOD(func_name, next_char, token_name)       \
static int _symbol_##func_name##_func(scanner_t* scanner){              \
    if(next_char != NULL && _match_next_char(scanner, next_char)) {     \
        scanner->cur_token.type = TOKEN_##token_name##_EQUAL;           \
        scanner->cur_token.len = 2;                         \
    }else {                                                 \
        scanner->cur_token.type = TOKEN_##token_name;       \
        scanner->cur_token.len = 1;                         \
    }                                                       \
    return 1;                                               \
}
SYMBOL_AND_EQUAL_METHOD(add, '=', ADD)
SYMBOL_AND_EQUAL_METHOD(sub, '=', SUB)
SYMBOL_AND_EQUAL_METHOD(mul, '=', MUL)
SYMBOL_AND_EQUAL_METHOD(mod, '=', MOD)
SYMBOL_AND_EQUAL_METHOD(not, '=', NOT)
SYMBOL_AND_EQUAL_METHOD(less, '=', LESS)
SYMBOL_AND_EQUAL_METHOD(greater, '=', GREATER)
SYMBOL_AND_EQUAL_METHOD(equal, '=', EQUAL)
/**
 * @brief 对于/以及以/开头的符号进行处理
 * @param scanner 分词器指针
 * @return 作为token返回1 不作为token返回0
 */
static int _symbol_div_func(scanner_t* scanner) {
    if(_match_next_char(scanner, '=')) {
        scanner->cur_token.type = TOKEN_DIV_EQUAL;
        scanner->cur_token.len = 2;
        return 1;
    }else if(_match_next_char(scanner, '*')) {
        _get_next_char(scanner);
        /* 查找注释对 */
        while(scanner->cur_char != '*') {
            if(scanner->cur_char == '\n') {
                scanner->cur_token.line_num ++;
            }
            _get_next_char(scanner);
        }
        /* 如果仅存在星号不存在斜杠则报错注释对不全 */
        if(!_match_next_char(scanner, '/')) {
            utils_report_error(scanner->file_path, scanner->cur_token.line_num, "缺少对应的注释对");
            _get_next_char(scanner);
            scanner->cur_token.start = scanner->next_char_ptr - 1;
        }else{
            /* 跳过'/' */
            _get_next_char(scanner);
            _skip_blanks(scanner);
            scanner->cur_token.start = scanner->next_char_ptr - 1;
        }
        return 0;
    }else if(_match_next_char(scanner, '/')) {
        _skip_line(scanner);
        scanner->cur_token.start = scanner->next_char_ptr - 1;
        return 0;
    }
    scanner->cur_token.type = TOKEN_DIV;
    return 1;
}
/**
 * @brief 左小括号解析
 * @param scanner 分词器指针
 * @return 作为token返回1 不作为token返回0
 */
static int _symbol_left_paren_func(scanner_t* scanner){
    scanner->cur_token.type = TOKEN_LEFT_PAREN;
    return 1;
}
/**
 * @brief 右小括号解析
 * @param scanner 分词器指针
 * @return 作为token返回1 不作为token返回0
 */
static int _symbol_right_paren_func(scanner_t* scanner){
    scanner->cur_token.type = TOKEN_RIGHT_PAREN;
    return 1;
}
/**
 * @brief 左中括号解析
 * @param scanner 分词器指针
 * @return 作为token返回1 不作为token返回0
 */
static int _symbol_left_bracket_func(scanner_t* scanner){
    scanner->cur_token.type = TOKEN_LEFT_BRACKET;
    return 1;
}
/**
 * @brief 右中括号解析
 * @param scanner 分词器指针
 * @return 作为token返回1 不作为token返回0
 */
static int _symbol_right_bracket_func(scanner_t* scanner){
    scanner->cur_token.type = TOKEN_RIGHT_BRACKET;
    return 1;
}
/**
 * @brief |解析
 * @param scanner 分词器指针
 * @return 作为token返回1 不作为token返回0
 */
static int _symbol_or_func(scanner_t* scanner){
    scanner->cur_token.type = TOKEN_LOGIC_ORR;
    return 1;
}
/**
 * @brief &解析
 * @param scanner 分词器指针
 * @return 作为token返回1 不作为token返回0
 */
static int _symbol_and_func(scanner_t* scanner){
    scanner->cur_token.type = TOKEN_LOGIC_AND;
    return 1;
}
/**
 * @brief 换行解析
 * @param scanner 分词器指针
 * @return 默认返回0跳过该字符 不作为token
 */
static int _symbol_enter_func(scanner_t* scanner){
    scanner->cur_token.line_num ++;
    _get_next_char(scanner);
    scanner->cur_token.start = scanner->next_char_ptr - 1;
    return 0;
}
