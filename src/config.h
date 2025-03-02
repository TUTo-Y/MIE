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
    char default_ttf_path[PATH_MAX]; // 默认字体路径
    char loginback_path[PATH_MAX];   // 登录背景路径

    char img_vert[PATH_MAX]; // 图像顶点着色器
    char img_frag[PATH_MAX]; // 图像片段着色器
    
    char loginback_vert[PATH_MAX]; // 登录背景顶点着色器
    char loginback_frag[PATH_MAX]; // 登录背景片段着色器

    char rrt_vert[PATH_MAX]; // 圆角纹理矩形顶点着色器
    char rrt_frag[PATH_MAX]; // 圆角纹理矩形片段着色器

    char rrc_vert[PATH_MAX]; // 圆角颜色矩形顶点着色器
    char rrc_frag[PATH_MAX]; // 圆角颜色矩形片段着色器

    char rt_vert[PATH_MAX]; // 纹理矩形顶点着色器
    char rt_frag[PATH_MAX]; // 纹理矩形片段着色器

    char rc_vert[PATH_MAX]; // 颜色矩形顶点着色器
    char rc_frag[PATH_MAX]; // 颜色矩形片段着色器

    char gaussblur_vert[PATH_MAX]; // 高斯模糊顶点着色器
    char gaussblur_frag[PATH_MAX]; // 高斯模糊片段着色器
} cfg;

extern cfg config;

/**
 * \brief 初始化加载配置文件
 * \param configFile 配置文件路径
 */
bool confInit(const char *configFile);

#endif // CONFIG_H