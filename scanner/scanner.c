/**
  ******************************************************************************
  * @file           : scanner.c
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/11
  ******************************************************************************
  */
#include <ctype.h>
#include "scanner.h"


#define TAG                 "[scanner]"

/* 导入flood的获取下一个token函数 */
extern void flood_get_next_token(void* scanner);

/* 分词器模式字符串数组 */
static const char* scanner_mode_str[] = {
    "flood",
//    "dfa"
};

/* 分词器获取下一个token的函数，应该与scanner_mode_str的顺序一致 */
static scanner_get_next_token_t get_next_token_func[] = {
    flood_get_next_token,
};

#ifdef ALL_STEPS_INDEPENDENCE

/**
 * @brief 将token插入表中
 * @param scanner
 * @note 该函数会根据token的类型自动选择标识符表或者常量表
 *       from __insert_table：别慌 我有自己的节奏 不用管我
 */
static void __insert_table(scanner_t* scanner) {
    char buf[128] = {0};
    token_t *token = &scanner->cur_token;
    /* buf存储该token的字符串 */
    memcpy(buf, token->start, token->len);
    if(token->type == TOKEN_ID) {
        /* 如果类型是用户标识符的话 则存储到标识符表里面 */
        if(bsst_search(scanner->user_id_table_tree, buf) == NULL) {
            user_id_table_t *temp = (user_id_table_t*)malloc(sizeof(user_id_table_t));
            ASSERT(temp != NULL, "插入表时创建节点失败");
            bsst_insert(&scanner->user_id_table_tree, buf, &temp);
        }
    } else if(token->type == TOKEN_NUM) {
        /* 如果类型是常量的话 则存储到常量表里面 */
        if(bsst_search(scanner->constant_table_tree, buf) == NULL) {
            constant_table_t *temp = (constant_table_t*)malloc(sizeof(constant_table_t));
            ASSERT(temp != NULL, "插入表时创建节点失败");
            bsst_insert(&scanner->constant_table_tree, buf, &temp);
        }
    }
}
/**
 * @brief 导出token为二元式
 * @param token 想要导出的token
 */
static void __export_token(token_t* token) {
    char buf1[128] = {0}, buf2[128] = {0};
    /* buf2存储该token的字符串 */
    memcpy(buf2, token->start, token->len);
    /* 以下为构建二元式到buf1 */
    sprintf(buf1, "(");
    if(token->type >= TOKEN_INT && token->type <= TOKEN_END) {
        sprintf(buf1, "%s%s,-)", buf1, buf2);
    } else if(token->type == TOKEN_ID) {
        sprintf(buf1, "%sid,%s)", buf1, buf2);
    } else if(token->type == TOKEN_NUM) {
        sprintf(buf1, "%snum,%s)", buf1, buf2);
    } else {
        sprintf(buf1, "%s%s,-)", buf1, buf2);
    }
    /* 追加写入二元式到该文件当中
     * 在每次分词器启动阶段，会清除编译目录下所有文件
     * 所以可以直接用追加的形式 */
    utils_write_file("../build/scanner/test.txt", buf1, "a");
}
/**
 * @brief 获取所有token
 * @param arg 分词器指针
 */
static void _get_all_token(void* arg) {
    scanner_t* scanner = (scanner_t*)arg;
    /* 首先获取一个token判断是不是eof类型的，防止文件为空 */
    scanner->get_next_token(scanner);
    while(scanner->cur_token.type != TOKEN_EOF){
        char buf[256] = {0};
        /* 将token指向字符串复制到buf里面 */
        memcpy(buf, scanner->cur_token.start, scanner->cur_token.len);
        S_LOGD(TAG, "%d %s", scanner->cur_token.type, buf);
        /* 此处应该生成表 且导出二元式 */
        __insert_table(scanner);
        __export_token(&scanner->cur_token);
        scanner->get_next_token(scanner);
    }
    /* 遍历一下二叉树 把生成的表打印出来 */
    printf("---------------用户标识符表---------------\n");
    bsst_inorder(scanner->user_id_table_tree);
    printf("----------------------------------------\n");
    printf("------------------常量表-----------------\n");
    bsst_inorder(scanner->constant_table_tree);
    printf("----------------------------------------\n");
    printf("%d errors, %d warnings", scanner->errors_num, scanner->warnings_num);
}
#endif

static void _get_next_token_init(void* arg) {
    scanner_t *scanner = (scanner_t*)arg;
    token_t* token = &scanner->cur_token;
    /* 跳过空格 */
    while(isspace(scanner->cur_char)){
        if(scanner->cur_char == '\n'){
            scanner->cur_token.line_num ++;
        }
        scanner->cur_char = *scanner->next_char_ptr++;
    }
    /* 将token保存到上一个token之后就可以开始获取下一个token了 */
    scanner->prev_token = *token;
    token->type = TOKEN_EOF;
    /* 设置token的起始地址 */
    token->start = scanner->next_char_ptr - 1;
    token->len = 0;
}

/**
 * @brief 创建分词器
 * @param scanner_mode 选择分词器模式，支持模式见scanner_mode_str数组
 * @param file_path 需要分词的文件目录
 * @return 分词器指针
 */
scanner_t* scanner_new(const char* scanner_mode, const char* file_path) {
    scanner_t* scanner = malloc(sizeof(scanner_t));
    if(!scanner){
        return NULL;
    }
    /* 存入需要分词的文件路径 */
    scanner->file_path = file_path;
    /* 读取源代码 */
    utils_read_file(file_path, &scanner->source_code);
    /* 如果读取失败 */
    if(scanner->source_code == NULL){
        S_LOGE(TAG, "Memory request failed");
        /* 别忘了释放分词器 防止内存泄露 */
        free(scanner);
        return NULL;
    }
    S_LOGI(TAG, "\n%s", scanner->source_code);
    scanner->cur_char = *scanner->source_code;
    scanner->next_char_ptr = scanner->source_code + 1;
    scanner->cur_token.line_num = 1;
    scanner->cur_token.type = TOKEN_UNKNOWN;
    scanner->cur_token.start = NULL;
    scanner->cur_token.len = 0;
    scanner->prev_token = scanner->cur_token;
    /* 设置获取下一个token的函数 */
    if(scanner_mode == NULL){
        /* 默认模式为flood模式 */
        scanner->get_next_token = get_next_token_func[0];
    }else{
        uint8_t _i, _scanner_mode_num = sizeof(scanner_mode_str) / sizeof(scanner_mode_str[0]);
        for(_i = 0; _i < _scanner_mode_num; _i++) {
            if(strcmp(scanner_mode_str[_i], scanner_mode) == 0){
                break;
            }
        }
        if(_i == _scanner_mode_num){
            /* 未匹配到的模式 默认模式0 也就是flood模式*/
            scanner->get_next_token = get_next_token_func[0];
        }else {
            /* 匹配到模式了 设定为该模式对应的函数 */
            scanner->get_next_token = get_next_token_func[_i];
        }
    }
    scanner->get_next_token_init = _get_next_token_init;
#ifdef ALL_STEPS_INDEPENDENCE
    scanner->get_all_token = _get_all_token;
#endif

    return scanner;
}
/**
 * @brief 删除分词器
 * @param scanner 分词器指针
 */
void scanner_delete(scanner_t* scanner) {
    free(scanner->source_code);
    free(scanner);
#ifdef ALL_STEPS_INDEPENDENCE
    bsst_destory(scanner->user_id_table_tree, NULL);
    bsst_destory(scanner->constant_table_tree, NULL);
#endif
}