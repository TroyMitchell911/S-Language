/**
  ******************************************************************************
  * @file           : symbols.inc
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/11
  ******************************************************************************
  */
#include "symbols_func.c"

typedef int (*symbol_func_t)(scanner_t* scanner);

struct symbol{
    char symbol;
    symbol_func_t func;
};

static struct symbol symbols[] = {
        {
                '+',
                _symbol_add_func
        },{
                '-',
                _symbol_sub_func
        },{
                '*',
                _symbol_mul_func
        },{
                '/',
                _symbol_div_func
        },{
                '%',
                _symbol_mod_func
        },{
                '(',
                _symbol_left_paren_func
        },{
                ')',
                _symbol_right_paren_func
        },{
                '[',
                _symbol_left_bracket_func
        },{
                ']',
                _symbol_right_bracket_func
        },{
                '<',
                _symbol_less_func
        },{
                '>',
                _symbol_greater_func
        },{
                '|',
                _symbol_or_func
        },{
                '&',
                _symbol_and_func
        },{
                '!',
                _symbol_not_func
        },{
                '=',
                _symbol_equal_func
        },{
                '\n',
                _symbol_enter_func
        },{
                ',',
                _symbol_comma_func
        },{
                ';',
                _symbol_semicolon_func
        },
};