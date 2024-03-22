/**
  ******************************************************************************
  * @file           : main.c
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/11
  ******************************************************************************
  */
#include <stdio.h>
#include <sys/stat.h>
#include "scanner.h"
#include "utils.h"

static const char* file_path = "../test.ss";

int main() {
    /* 工具初始化成功 */
    if(utils_init()) {
        /* 清除中间文件 */
        utils_clean_temp_files();
        /* 创建分词器 */
        scanner_t* scanner = scanner_new(NULL, file_path);
        /* 获取所有token */
        scanner->get_all_token(scanner);
        /* 删除分词器 */
        scanner_delete(scanner);
    }

    return 0;
}
