/**
  ******************************************************************************
  * @file           : utils.c
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/11
  ******************************************************************************
  */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils.h"


#define TAG                 "[utils]"
/**
 * @brief 检查编译需要的文件夹是否存在，不存在则创建
 * @param dir_path 程序根目录
 * @return 1检查成功 0检查失败
 * @attention 在返回0时应该注意需要终止此次编译，文件夹都没有编译什么
 */
static int _check_dir(const char* dir_path) {
    struct stat st;
    /* 使用stat函数检查文件夹是否存在 */
    if (stat(dir_path, &st) != 0) {
        int status = mkdir(dir_path);
        if(status != 0) {
            return 0;
        }
    }
    return 1;
}
/**
 * @brief 删除文件夹下的所有文件
 * @param dir_path 想要删除的文件夹目录
 */
static void _del_dir_all_files(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[256];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        struct stat st;
        if (stat(path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                _del_dir_all_files(path); // 递归删除子目录
                rmdir(path); // 删除空目录
            } else {
                remove(path); // 删除文件
            }
        }
    }
    closedir(dir);
}
/**
 * @brief 编译系统初始化函数
 * @return 返回初始化是否成功
 */
int utils_init(void) {
    const char* dir_paths[] = {
            "../build",
            "../build/scanner"
    };
    for(int i = 0; i < sizeof(dir_paths)/sizeof(dir_paths[0]); i++) {
        int ret = _check_dir(dir_paths[i]);
        if(ret == 0) {
            return 0;
        }
    }
    return 1;
}
/**
 * @brief 清除编译目录下的临时文件
 */
void utils_clean_temp_files(void) {
    const char* dir_paths[] = {
            "../build/scanner/"
    };
    for(int i = 0; i < sizeof(dir_paths)/sizeof(dir_paths[0]); i++) {
        _del_dir_all_files(dir_paths[i]);
    }
}


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

void utils_write_file(const char* file_name, char* buf, char* mode) {
    FILE *file = fopen(file_name, mode); // 打开文件用于追加
    if (file == NULL) {
        return;
    }
    /* 追加一行文本 */
    fprintf(file, buf);
    /* 关闭文件 */
    fclose(file);
}
