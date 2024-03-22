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

extern void flood_get_next_token(void* scanner);

static const char* scanner_mode_str[] = {
    "flood",
//    "dfa"
};

static scanner_get_next_token_t get_next_token_func[] = {
    flood_get_next_token,
};

#ifdef ALL_STEPS_INDEPENDENCE


static void __insert_table(scanner_t* scanner) {
    char buf[128] = {0};
    token_t *token = &scanner->cur_token;
    memcpy(buf, token->start, token->len);
    if(token->type == TOKEN_ID) {
        if(bsst_search(scanner->user_id_table_tree, buf) == NULL) {
            user_id_table_t *temp = (user_id_table_t*)malloc(sizeof(user_id_table_t));
            ASSERT(temp != NULL, "插入表时创建节点失败");
            bsst_insert(&scanner->user_id_table_tree, buf, &temp);
        }
    } else if(token->type == TOKEN_NUM) {
        if(bsst_search(scanner->constant_table_tree, buf) == NULL) {
            constant_table_t *temp = (constant_table_t*)malloc(sizeof(constant_table_t));
            ASSERT(temp != NULL, "插入表时创建节点失败");
            bsst_insert(&scanner->constant_table_tree, buf, &temp);
        }
    }
}

static void __export_token(token_t* token) {
    char buf1[128] = {0}, buf2[128] = {0};
    memcpy(buf2, token->start, token->len);
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
    utils_write_file("../build/scanner/test.txt", buf1, "a");
}
/**
 * @brief 获取所有token
 * @param arg 分词器指针
 */
static void _get_all_token(void* arg) {
    scanner_t* scanner = (scanner_t*)arg;
    scanner->get_next_token(scanner);
    while(scanner->cur_token.type != TOKEN_EOF){
        char buf[256] = {0};
        memcpy(buf, scanner->cur_token.start, scanner->cur_token.len);
        S_LOGD(TAG, "%d %s", scanner->cur_token.type, buf);
        /* 此处应该生成表 且导出二元式 */
        __insert_table(scanner);
        __export_token(&scanner->cur_token);
        scanner->get_next_token(scanner);
    }
    bsst_inorder(scanner->user_id_table_tree);
    bsst_inorder(scanner->constant_table_tree);
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
    scanner->prev_token = *token;
    token->type = TOKEN_EOF;
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

    scanner->file_path = file_path;
    utils_read_file(file_path, &scanner->source_code);
    if(scanner->source_code == NULL){
        S_LOGE(TAG, "Memory request failed");
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
        /* 未匹配到的模式 */
        if(_i == _scanner_mode_num){
            scanner->get_next_token = get_next_token_func[0];
        }else {
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