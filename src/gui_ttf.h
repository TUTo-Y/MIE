/**
 * \file gui_ttf.h
 * \brief 渲染TTF格式文本
 *
 * // 创建TTF字体
 * GUIttf *ttf = guiTTFCreateTTF(fontData, 16, 32, 0);
 * GUIchar *c1 = guiCharGetFromTTF(ttf, L'中', 64);
 * GUIchar *c2 = guiCharGetFromTTF(ttf, L'国', 64);
 *
 *
 * // 渲染
 * guiCharRender(c1);
 * guiCharRender(c2);
 *
 */
#ifndef GUI_TTF_H
#define GUI_TTF_H

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
#include "gui_texture.h"

/**
 * \brief 单个文字结构
 */
typedef struct
{
    wchar_t text; // 文本内容

    int ascent;  // 上升
    int descent; // 下降
    int lineGap; // 行间距

    int advance; // 宽度

    int x; // 位图左边距
    int y; // 位图顶部
    int w; // 位图宽度
    int h; // 位图高度

    GLuint VAO, VBO, EBO; // 渲染
    GLuint texture;       // 纹理
} GUIchar;

/**
 * \brief 单个字号结构
 */
typedef struct
{
    int pixels;  // 字号
    float scale; // 缩放

    int ascent;  // 上升
    int descent; // 下降
    int lineGap; // 行间距

    wchar_t *textList; // 文文字列表
    GUIchar *textRend; // 文本渲染列表
    int textCount;     // 文字数量
    int textMax;       // 最大文字数量
} GUIfont;

/**
 * \brief TTF字体结构
 */
typedef struct
{
    stbtt_fontinfo fontInfo; // 字体信息

    GUIfont *fontList; // 字号列表
    int fontCount;     // 字号数量
} GUIttf;

extern GUIttf *font_default;

/**
 * \brief 创建一个文字到Font中
 * \param ttf TTF字体
 * \param font 字号列表
 * \param text 文本内容
 * \return GUIchar 单个文字
 */
GUIchar *guiCharCreate(GUIttf *ttf, GUIfont *font, wchar_t text);

/**
 * \brief 从Font中获取一个文字，如果没有则创建
 * \param ttf TTF字体
 * \param font 字号列表
 * \param text 文本内容
 * \return GUIchar 单个文字
 */
GUIchar *guiCharGetFromFont(GUIttf *ttf, GUIfont *font, wchar_t text);

/**
 * \brief 从TTF中获取一个文字，如果没有则创建
 * \param ttf TTF字体
 * \param text 文本内容
 * \param pixels 字号
 * \return GUIchar 单个文字
 */
GUIchar *guiCharGetFromTTF(GUIttf *ttf, wchar_t text, int pixels);

/**
 * \brief 渲染一个文字
 * \param c 文字
 */
void guiCharRender(GUIchar *c);

/**
 * \brief 初始化字号结构体，不能将font添加进ttf，如果想要添加，应当使用guiFontGet
 * \param ttf TTF字体
 * \param font 字体结构体
 * \param pixels 字号
 */
void guiFontInit(GUIttf *ttf, GUIfont *font, int pixels);

/**
 * \brief 从ttf中查找一个字号，如果没有则创建
 * \param ttf TTF字体
 * \param pixels 字号
 * \return GUIfont字号
 */
GUIfont *guiFontGet(GUIttf *ttf, int pixels);

/**
 * \brief 初始化TTF字体
 * \param fontData TTF字体数据(二进制数据)
 * \param ... 字号, 0表示结束
 * \return GUIttf TTF字体
 * \note GUIttf *ttf = guiTTFInit(const unsigned char *fontData, 16, 32, 0); // 初始化字号16, 32
 */
GUIttf *guiTTFCreate(const unsigned char *fontData, ...);

/**
 * \brief 清除TTF缓存
 * \param ttf TTF字体
 */
void guiTTFDelete(GUIttf *ttf);

#endif // GUI_TTF_H