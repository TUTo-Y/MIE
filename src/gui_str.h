/**
 * @file gui_str.h
 * @brief GUI 字符串
 *
 * // 创建字符串
 * GUIstr *s1 = guiStrCreate(ttf, 64, GUI_STR_MOD_CENTER, program, NULL, (vec4){1.0f, 0.0f, 0.0f, 1.0f});
 * guiStrCpy(s1, L"我shi一个");
 *
 * // 修改model和color
 * guiStrSetModel(s1, model);
 * guiStrSetColor(s1, color);
 *
 * // 渲染字符串
 * guiStrRender();
 *
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

#include "gui_base.h"
#include "gui_ttf.h"
#include "gui_shader.h"
#include "gui_texture.h"

#define guiStrUseProgram(str) guiShaderUse((str)->program) // 使用字符串渲染器

#define guiStrGetStr(str) ((str)->strText)
#define guiStrGetStrLen(str) wcslen(guiStrGetStr(str))
#define guiStrGetFix(str) ((str)->fix)
#define guiStrGetModel(str) ((str)->model)
#define guiStrGetColor(str) ((str)->color)

#define guiStrSetFix(str, fix) glm_mat4_copy(fix, guiStrGetFix(str))         // 设置修正矩阵
#define guiStrSetModel(str, model) glm_mat4_copy(model, guiStrGetModel(str)) // 设置模型矩阵
#define guiStrSetColor(str, color) glm_vec4_copy(color, guiStrGetColor(str)) // 设置颜色
#define guiStrSetMode(str, mode) ((str)->mode = mode)                        // 设置字符串对齐模式

#define guiStrAppFix(str, fix) guiShaderUniformMatrixFromID((str)->fixLoc, 4fv, (float *)(fix))              // 更新修正矩阵数据
#define guiStrAppModel(str) guiShaderUniformMatrixFromID((str)->modelLoc, 4fv, (float *)guiStrGetModel(str)) // 更新模型矩阵数据
#define guiStrAppColor(str) guiShaderUniformFromID((str)->colorLoc, 4f,    \
                                                   guiStrGetColor(str)[0], \
                                                   guiStrGetColor(str)[1], \
                                                   guiStrGetColor(str)[2], \
                                                   guiStrGetColor(str)[3]) // 更新颜色数据

/**
 * \brief 字符串结构
 */
typedef struct
{
    GUIttf *ttf;   // 字体
    GUIfont *font; // 字号

    wchar_t *strText;  // 字符串文本
    GUIchar **strChar; // 字符串渲染
    int count;         // 字符串中字符数量
    int countMax;      // 字符串中字符最大数量

    int mode; // 对齐模式

    GLuint program; // 字符串的渲染器

    mat4 fix;     // 修正矩阵
    GLint fixLoc; // 修正矩阵位置

    mat4 model;     // 模型矩阵
    GLint modelLoc; // 模型矩阵位置

    vec4 color;     // 颜色
    GLint colorLoc; // 颜色位置

} GUIstr;

enum
{
    GUI_STR_MOD_LEFT = 0,     // 左对齐
    GUI_STR_MOD_LEFT_TOP = 1, // 左上对其
    GUI_STR_MOD_CENTER = 2,   // 居中对齐
    GUI_STR_MOD_CENTER2 = 3,  // 完全居中对齐
    GUI_STR_MOD_RIGHT = 4     // 右对齐
};

/**
 * \brief 创建一个字符串
 * \param ttf 字体
 * \param pixels 字号
 * \param mode 对齐模式
 * \param program 渲染器
 * \param model 模型矩阵
 * \param PV 投影视图矩阵
 * \param color 颜色
 * \return GUIstr* 字符串
 */
GUIstr *guiStrCreate(GUIttf *ttf, int pixels, int mode, GLuint program, mat4 model, vec4 color);

/**
 * \brief 渲染字符串
 * \param str 字符串
 */
void guiStrRender(GUIstr *str);

/**
 * \brief 删除字符串
 */
void guiStrDelete(GUIstr *str);

/**
 * \brief 设置字符串对齐模式(模型矩阵)
 * \param str 字符串
 */
void guiStrAppMode(GUIstr *str);

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

#endif // GUI_STR_H