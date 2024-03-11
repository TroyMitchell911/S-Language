/**
  ******************************************************************************
  * @file           : debug.h
  * @author         : Troy
  * @brief          : 该文件添加了Debug宏定义
  * @attention      : 该文件不对应任何C文件实体
  * @date           : 2024/3/11
  ******************************************************************************
  */
#ifndef __UTILS_COMMON_H
#define __UTILS_COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define ALL_STEPS_INDEPENDENCE

#ifdef DEBUG
#define ASSERT(condition, err_msg)                                  \
    do {                                                            \
        if(!(condition)){                                           \
            printf("ASSERT failed! %s:%d in function %s(): %s \n",  \
                __FILE__, __LINE__, __func__, err_msg);             \
            abort();                                                \
        }                                                           \
    } while(0);
#define S_LOGD(target,format, ...)    printf("[Debug]:" format "\r\n", ##__VA_ARGS__)
#define S_LOGE(target,format, ...)    printf("[Error]:" format "\r\n", ##__VA_ARGS__)
#define S_LOGI(target,format, ...)    printf("[Info]:" format "\r\n", ##__VA_ARGS__)
#define S_LOGW(target,format, ...)    printf("[Warning]:" format "\r\n", ##__VA_ARGS__)
#else
#define ASSERT(condition, err_msg) (void(0))
#define S_LOGD(target,format, ...)
#define S_LOGE(target,format, ...)
#define S_LOGI(target,format, ...)
#define S_LOGW(target,format, ...)
#endif

#endif