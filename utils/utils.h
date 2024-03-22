/**
  ******************************************************************************
  * @file           : utils.h
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/11
  ******************************************************************************
  */
#ifndef __UTILS_H
#define __UTILS_H

#include <debug.h>

int utils_init(void);
void utils_clean_temp_files(void);
void utils_read_file(const char* file_path, char** buffer);
void utils_report_error(const char* file_name, uint32_t line_num, const char* error);
void utils_report_warning(const char* file_name, uint32_t line_num, const char* warning);
void utils_write_file(const char* file_name, char* buf, char* mode);
#endif