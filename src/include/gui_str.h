/**
 * @file gui_str.h
 * @brief GUI 字符串
 */
#ifndef GUI_STR_H
#define GUI_STR_H

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stb_truetype.h>

#include "log.h"
#include "gui_ttf.h"
#include "gui_shader.h"

/**
 * \brief 字符串结构
 */
typedef struct
{
    GUIttf *ttf; // 字体
    int pixels;  // 字号

    GLuint program;  // 字符串的渲染器
    mat4 model;      // 模型矩阵
    mat4 view;       // 视图矩阵
    mat4 projection; // 投影矩阵
    vec4 color;      // 颜色

    wchar_t *strText;  // 字符串文本
    GUIchar **strChar; // 字符串渲染
    int count;         // 字符串中字符数量
    int countMax;      // 字符串中字符最大数量
} GUIstr;

/**
 * \brief 设置字符串矩阵
 * \param str 字符串
 * \param model 模型矩阵, 为NULL则不更新
 * \param view 视图矩阵, 为NULL则不更新
 * \param projection 投影矩阵, 为NULL则不更新
 * \param appMat 是否更新着色器中的model, view和projection矩阵数据
 */
void guiStrSetMat(GUIstr *str, mat4 model, mat4 view, mat4 projection, bool appMat);

/**
 * \brief 设置字符串文本颜色
 * \param str 字符串
 * \param color 颜色
 * \param appColor 是否更新着色器中的颜色数据
 */
void guiStrSetColor(GUIstr *str, vec4 color, bool appColor);

/**
 * \brief 创建一个字符串
 * \param ttf 字体
 * \param pixels 字号
 * \param program 渲染器
 * \param model 模型矩阵
 * \param view 视图矩阵
 * \param projection 投影矩阵
 * \return GUIstr* 字符串
 */
GUIstr *guiStrCreate(GUIttf *ttf, int pixels, GLuint program, mat4 model, mat4 view, mat4 projection);

/**
 * \brief 设置字符串文本
 * \param str 字符串
 * \param text 文本
 */
void guiStrCpy(GUIstr *str, const wchar_t *text);

/**
 * \brief 追加字符串文本
 * \param str 字符串
 * \param text 文本
 */
void guiStrCat(GUIstr *str, const wchar_t *text);

/**
 * \brief 追加字符串字符
 * \param str 字符串
 * \param text 字符
 */
void guiStrCatC(GUIstr *str, const wchar_t text);

/**
 * \brief 删除最后一个字符
 * \param str 字符串
 */
void guiStrBack(GUIstr *str);

/**
 * \brief 删除N个字符
 * \param str 字符串
 * \param n 删除字符数量
 */
void guiStrBackN(GUIstr *str, int n);

/**
 * \brief 渲染字符串
 * \param str 字符串
 * \param model 模型矩阵, 不会更新str中的模型，为NULL则不使用
 * \param appMat 是否更新view和projection矩阵数据
 * \param appColor 是否更新颜色数据
 * \note 对于view和projection矩阵，需要在调用此函数之前设置
 */
void guiStrRender(GUIstr *str, mat4 model, bool appMat, bool appColor);

/**
 * \brief 删除字符串
 */
void guiStrDelete(GUIstr *str);

#endif // GUI_STR_H