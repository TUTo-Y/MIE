/**
 * @file gui_ttf.h
 * @brief 渲染TTF格式文本
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
#include "gui_texture.h"

/**
 * @brief 单个文字结构
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
 * @brief 单个字号结构
 */
typedef struct
{
    int pixels;  // 字号
    float scale; // 缩放

    int ascent;  // 上升
    int descent; // 下降
    int lineGap; // 行间距

    wchar_t *textList; // 文本
    GUIchar *textRend; // 文本渲染列表
    int textCount;     // 文本数量
    int textMax;       // 最大文本数量
} GUIfont;

/**
 * @brief TTF字体
 */
typedef struct
{
    stbtt_fontinfo fontInfo; // 字体信息

    GUIfont *fontList; // 字号列表
    int fontCount;     // 字号数量
} GUIttf;

/**
 * \brief 初始化TTF字体
 * \param fontData TTF字体数据(二进制数据)
 * \param ... 字号, 0表示结束
 * \return GUIttf TTF字体
 * \note GUIttf *ttf = guiTTFCreate(const unsigned char *fontData, 16, 32, 64, 0); // 初始化字号16, 32, 64
 */
GUIttf *guiTTFCreate(const unsigned char *fontData, ...);

/**
 * \brief 创建一个文字
 * \param ttf TTF字体
 * \param text 文本内容
 * \param pixels 字号
 * \return GUIchar 单个文字
 */
GUIchar *guiTTFCreateChar(GUIttf *ttf, const wchar_t text, int pixels);

/**
 * \brief 渲染一个文字
 * \param c 文字
 */
void guiTTFRenderChar(GUIchar *c);

/**
 * \brief 清除TTF缓存
 */
void guiTTFDelete(GUIttf *ttf);

#endif // GUI_TTF_H