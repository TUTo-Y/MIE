#ifndef GUI_WIDGET_BUTTON_H
#define GUI_WIDGET_BUTTON_H

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "gui_type.h"
#include "gui_str.h"
#include "gui_draw.h"
#include "gui_ID.h"

#define GUI_WIDGET_BUTTON_STATUS_SELECT 0x01 // 选中状态
#define GUI_WIDGET_BUTTON_STATUS_ENTER 0x02  // 鼠标进入状态

typedef struct
{
    size_t status; // 状态

    GUIrect rect; // 位置
    GUIpos pos;   // 位置

    GUIstr *text; // 文本

    GUIdrawrr rr; // 绘制矩形
    GUIdrawcc cc; // 绘制圆

    // 按钮确定回调
    void (*onEnter)(GUIid id, size_t flag, void *data);
    size_t flag; // 回调标志
    void *data;  // 回调数据

    // 动画数据
    float time; // 时间
} GUIwidgetButtonData;

// 创建和销毁调用函数
void gui_widget_button_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_button_logoffCall(GUIid id);   // 销毁控件时的函数

// 回调函数
void gui_widget_button_loadCall(GUIid id);   // 添加控件时
void gui_widget_button_uploadCall(GUIid id); // 移除控件时

bool gui_widget_button_drawCall(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_button_eventCall(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

// 设置按钮文本
static inline void guiWidgetButtonSetText(GUIid id, const wchar_t *text)
{
    GUIwidgetButtonData *data = GUI_ID2WIDGET(id)->data1;
    guiStrCpy(data->text, text);
}

// 设置按钮字体
void guiWidgetButtonSetFont(GUIid id, GUIttf *ttf, int pixels, vec4 color);

// 设置按钮回调
static inline void guiWidgetButtonSetBackCall(GUIid id, void (*onEnter)(GUIid id, size_t flag, void *data), size_t flag, void *data)
{
    GUIwidgetButtonData *data1 = GUI_ID2WIDGET(id)->data1;
    data1->onEnter = onEnter;
    data1->flag = flag;
    data1->data = data;
}

// 设置按钮位置
void guiWidgetButtonSetPos(GUIid id, int x, int y, int width, int height);

// 设置按钮颜色
static inline void guiWidgetButtonSetColor(GUIid id, vec4 color1, vec4 color2)
{
    GUIwidgetButtonData *data = GUI_ID2WIDGET(id)->data1;
    guiDrawRRCSetColor(data->rr, color1);
    guiDrawCCSetColor(data->cc, color2);
}

#endif // GUI_WIDGET_BUTTON_H
