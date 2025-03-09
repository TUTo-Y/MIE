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

    char icon_vert[PATH_MAX]; // 图标顶点着色器
    char icon_frag[PATH_MAX]; // 图标片段着色器

    char font_vert[PATH_MAX]; // 默认字体着色器
    char font_frag[PATH_MAX]; // 默认字体片段着色器

    char cc_vert[PATH_MAX]; // 圆颜色顶点着色器
    char cc_frag[PATH_MAX]; // 圆颜色片段着色器

    char cc2_vert[PATH_MAX]; // 圆颜色2顶点着色器
    char cc2_frag[PATH_MAX]; // 圆颜色2片段着色器

    char rrc2_vert[PATH_MAX]; // 圆角颜色2顶点着色器
    char rrc2_frag[PATH_MAX]; // 圆角颜色2片段着色器

    char user_icon_path[PATH_MAX]; // 用户图标路径
    char pass_icon_path[PATH_MAX]; // 密码图标路径
} cfg;

extern cfg config;

/**
 * \brief 初始化加载配置文件
 * \param configFile 配置文件路径
 */
bool confInit(const char *configFile);

#endif // CONFIG_H