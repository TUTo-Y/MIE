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
    char res_path[PATH_MAX];       // 资源路径
    char loginback_path[PATH_MAX]; // 登录背景路径
} cfg;

extern cfg config;

/**
 * \brief 初始化加载配置文件
 * \param configFile 配置文件路径
 */
bool confInit(const char *configFile);

#endif // CONFIG_H