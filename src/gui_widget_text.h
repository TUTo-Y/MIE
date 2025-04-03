#ifndef GUI_WIDGET_TEXT_H
#define GUI_WIDGET_TEXT_H

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "gui_str.h"
#include "gui_draw.h"
#include "stb_image.h"
#include "gui_ID.h"

typedef struct
{
    GUIstr *str; // 文本
} GUIwidgetText;

// 创建和销毁调用函数
void gui_widget_text_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_text_logoffCall(GUIid id);   // 销毁控件时的函数

bool gui_widget_text_drawCall(GUIid id); // 绘制, 返回值为是否继续调用其他响应函数

static inline GUIstr *guiWidgetTextGetStr(GUIid id)
{
    return ((GUIwidgetText *)GUI_ID2WIDGET(id)->data1)->str;
}

static inline GUIstr **guiWidgetTextGetStrP(GUIid id)
{
    return &((GUIwidgetText *)GUI_ID2WIDGET(id)->data1)->str;
}

#endif // GUI_WIDGET_TEXT_H