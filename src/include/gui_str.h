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

#define guiStrSetMatModel(str, model) glm_mat4_copy(model, (str)->model)                     // 设置模型矩阵
#define guiStrSetMatView(str, view) glm_mat4_copy(view, (str)->view)                         // 设置视图矩阵
#define guiStrSetMatProjection(str, projection) glm_mat4_copy(projection, (str)->projection) // 设置投影矩阵
#define guiStrSetColor(str, c) glm_vec4_copy(c, (str)->color)                                // 设置颜色

// 更新矩阵数据
#define guiStrAppMat(str)                                                                      \
    {                                                                                          \
        guiShaderUse((str)->program);                                                          \
        guiShaderUniformMatrix((str)->program, "model", 4fv, (float *)(str)->model);           \
        guiShaderUniformMatrix((str)->program, "view", 4fv, (float *)(str)->view);             \
        guiShaderUniformMatrix((str)->program, "projection", 4fv, (float *)(str)->projection); \
    }
// 更新颜色数据
#define guiStrAppColor(str)                           \
    {                                                 \
        guiShaderUse((str)->program);                 \
        guiShaderUniform((str)->program, "color", 4f, \
                         (str)->color[0],             \
                         (str)->color[1],             \
                         (str)->color[2],             \
                         (str)->color[3]);            \
    }

/**
 * \brief 字符串结构
 */
typedef struct
{
    GUIttf *ttf;   // 字体
    GUIfont *font; // 字号

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

enum
{
    GUI_STR_MOD_LEFT = 0,    // 左对齐
    GUI_STR_MOD_CENTER = 1,  // 居中对齐
    GUI_STR_MOD_RIGHT = 2,   // 右对齐
    GUI_STR_MOD_LEFT_TOP = 3 // 左上对其
};

/**
 * \brief 渲染字符串
 * \param str 字符串
 * \param model 模型矩阵
 * \param appMatView 是否更新view矩阵数据
 * \param appMatProjection 是否更新projection矩阵数据
 * \param appColor 是否更新颜色数据
 * \note 对于view和projection矩阵，需要在调用此函数之前设置
 */
void guiStrRender(GUIstr *str, mat4 model, bool appMatView, bool appMatProjection, bool appColor);

/**
 * \brief 设置字符串对齐模式(模型矩阵)
 * \param str 字符串
 * \param mod 对齐模式
 */
void guiStrSetMod(GUIstr *str, int mod);

/**
 * \brief 创建一个字符串
 * \param ttf 字体
 * \param pixels 字号
 * \param program 渲染器
 * \param model 模型矩阵
 * \param view 视图矩阵
 * \param projection 投影矩阵
 * \param color 颜色
 * \return GUIstr* 字符串
 */
GUIstr *guiStrCreate(GUIttf *ttf, int pixels, GLuint program, mat4 model, mat4 view, mat4 projection, vec4 color);

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
 * \brief 删除字符串
 */
void guiStrDelete(GUIstr *str);

#endif // GUI_STR_H