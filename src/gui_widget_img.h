#ifndef GUI_WIDGET_IMG_H
#define GUI_WIDGET_IMG_H

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "gui_type.h"
#include "gui_str.h"
#include "gui_draw.h"
#include "gui_ID.h"

typedef struct
{
    GUIdrawr img; // 原图像

    GLuint texture; // 纹理
    void *data;     // 纹理原始数据
    int w, h;       // 纹理大小

    bool isTexture; // 是否创建的纹理

    GUIrect rect; // 图像显示的位置
} GUIwidgetimg;

// 创建和销毁调用函数
void gui_widget_img_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_img_logoffCall(GUIid id);   // 销毁控件时的函数

// 回调函数
void gui_widget_img_loadCall(GUIid id);   // 添加控件时
void gui_widget_img_uploadCall(GUIid id); // 移除控件时

bool gui_widget_img_drawCall(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_img_eventCall(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

void guiWidgetImgSetTexture(GUIid id, void *data, int w, int h);      // 设置纹理
void guiWidgetImgSetTexture2(GUIid id, GLuint texture, int w, int h); // 设置纹理
void guiWidgetImgSetRect(GUIid id, GUIrect rect);                     // 设置mini位置

static inline void guiWidgetImgSetEmpty(GUIid id)
{
    uint64_t color = 0L;
    guiWidgetImgSetTexture(id, (void *)&color, 1, 1);
}

#endif // GUI_WIDGET_IMG_H