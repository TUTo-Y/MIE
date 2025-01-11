/**
 * @file gui_ttf.h
 * @brief 渲染TTF格式文本
 */
#ifndef GUI_TTF_H
#define GUI_TTF_H

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdbool.h>

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stb_truetype.h>

#include "log.h"

typedef struct
{
    wchar_t c;   // 文本内容
    int advance; // 前进宽度
    int lsb;     // 左边距

    GLuint VBO; // 顶点缓冲区
    GLuint VAO; // 顶点数组对象

    int *VBOs;    // 顶点缓冲区列表
    int VBOcount; // 顶点渲染节点数量
} GUIchar;

/**
 * \brief 初始化TTF字体
 * \param fontData TTF字体数据
 */
void guiTTFInit(const unsigned char *fontData);

/**
 * \brief 清除TTF缓存
 */
void guiTTFQuit();

/**
 * \brief 渲染一个文字的顶点
 * \param text 文本内容
 * \return 渲染文本
 */
GUIchar *guiTTFGetChar(const wchar_t text);

/**
 * \brief 渲染一个文字
 * \param ttf 文本
 */
void guiTTFRenderChar(GUIchar *ttf);

#endif // GUI_TTF_H