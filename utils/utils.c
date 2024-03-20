/**
  ******************************************************************************
  * @file           : utils.c
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/11
  ******************************************************************************
  */
#include <sys/stat.h>
#include "utils.h"


#define TAG                 "[utils]"

/**
 * @brief 读取文件函数
 * @param file_path 文件路径
 * @param buffer 缓冲区指针的指针，用于存储文件内容
 * @attention 使用完后需要手工free
 */
void utils_read_file(const char* file_path, char** buffer) {
    FILE* f = fopen(file_path, "r");
    /* If the file does not exist  */
    if(!f){
        printf("This file is not detected.");
        return;
    }
    /* Get the size of the file and malloc*/
    struct stat fstat;
    stat(file_path,&fstat);
    *buffer = (char*) malloc(fstat.st_size + 1);
    if(!*buffer){
        S_LOGE(TAG, "Memory request failed");
        return;
    }
    memset(*buffer, 0, fstat.st_size + 1);

    /* The content of the file is put into the buffer */
    fread(*buffer, sizeof(char), fstat.st_size, f);
    fclose(f);
}
#define UTILS_REPORT(func_name, type) \
                                      \
void utils_report_##func_name(const char* file_name, uint32_t line_num, const char* error) { \
    printf("%s:%ld %s:%s\n", file_name, line_num, type, error);                                \
}                               \

UTILS_REPORT(error, "error")
UTILS_REPORT(warning, "warning")