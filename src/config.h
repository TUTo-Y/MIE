/**
 * \file config.h
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "log.h"

typedef struct
{
    size_t width;  // 窗口宽度
    size_t height; // 窗口高度

    char res_path[PATH_MAX]; // 资源路径
} cfg;

extern cfg config;


/**
 * \brief 初始化加载配置文件
 * \param configFile 配置文件路径
 */
bool confInit(const char *configFile);

#endif // CONFIG_H