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
    if(utils_init()) {
        utils_clean_temp_files();
        scanner_t* scanner = scanner_new(NULL, file_path);
        scanner->get_all_token(scanner);
        scanner_delete(scanner);
    }

    return 0;
}
