#ifndef GUI_WIDGET_INPUT_H
#define GUI_WIDGET_INPUT_H

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "gui.h"
#include "gui_str.h"
#include "gui_draw.h"

#define GUI_WIDGET_INPUT_PASSWD 0x1 // 使用密码模式
#define GUI_WIDGET_INPUT_FOCUS 0x2  // 获得焦点
#define GUI_WIDGET_INPUT_ANIM1 0x4  // 动画1
#define GUI_WIDGET_INPUT_ANIM2 0x8  // 动画2

typedef struct
{
    // 当前文本框状态
    size_t status;

    // 当前渲染位置
    vec3 pos;     // 渲染位置
    GUIrect rect; // 矩形, 用于快速检查鼠标是否在文本框内

    // 渲染的下划线
    GUIdrawrr lineBack; // 背景线
    GUIdrawrr linePre;  // 前景线
    size_t lineSize;    // 线长度

    // 文本框的文本
    GUIstr *textDefault; // 默认渲染文本
    GUIstr *textRender;  // 渲染文本
    wchar_t *textplain;  // 明文(实际内容)
    size_t textMax;      // 文本最大长度

    // 动画
    double linePreTime; // 动画时间
} GUIwidgetInputData;  // data1是渲染数据

typedef struct
{
    char *textRender;  // 文本
    size_t size;       // 大小
} GUIwidgetInputData2; // data2是文本数据

// 创建和销毁调用函数
void gui_widget_input_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_input_logoffCall(GUIid id);   // 销毁控件时的函数

// 回调函数
void gui_widget_input_loadCall(GUIid id);   // 添加控件时
void gui_widget_input_uploadCall(GUIid id); // 移除控件时

bool gui_widget_input_drawCall(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_input_eventCall(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

/**
 * \brief 设置文本框是否启用密码模式
 * \param id 控件ID
 * \param passwd 是否使用密码模式
 */
void guiWidgetInputSetPasswd(GUIid id, bool passwd);

/**
 * \brief 修改渲染位置
 * \param id 控件ID
 */
void guiWidgetInputSetPos(GUIid id, vec3 pos);

/**
 * \brief 修改底线长度
 * \param id 控件ID
 * \param size 长度
 */
void guiWidgetInputSetLineSize(GUIid id, size_t size);

/**
 * \brief 修改背景和前景的颜色
 * \param id 控件ID
 * \param back 背景颜色
 * \param pre 前景颜色
 */
void guiWidgetInputSetColor(GUIid id, vec4 back, vec4 pre);

/**
 * \brief 修改文本框的最大容量
 * \param id 控件ID
 * \param size 容量
 */
void guiWidgetInputSetMax(GUIid id, size_t size);

/**
 * \brief 更新检查矩形
 * \param id 控件ID
 */
static inline void guiWidgetInputUpdateRect(GUIwidgetInputData *data1)
{
    data1->rect.height = data1->textRender->font->ascent - data1->textRender->font->descent + data1->textRender->font->lineGap;
    data1->rect.y = data1->pos[1] + data1->textRender->font->ascent;
    data1->rect.x = data1->pos[0] - data1->lineSize / 2;
    data1->rect.width = data1->lineSize;
}

/**
 * \brief 设置文本字体
 * \param id 控件ID
 * \param ttf 字体
 * \param pixels 字号
 */
void guiWidgetInputSetTextFont(GUIid id, GUIttf *ttf, int pixels);

/**
 * \brief 修改文本颜色
 * \param id 控件ID
 * \param color 颜色
 */
static inline void guiWidgetInputSetTextColor(GUIid id, vec4 color)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    guiStrSetColor(data1->textRender, color);
}

/**
 * \brief 设置默认文本内容
 * \param id 控件ID
 * \param textRender 文本内容
 */
static inline void guiWidgetInputSetDefaultText(GUIid id, const wchar_t *textRender)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    guiStrCpy(data1->textDefault, textRender);
}

/**
 * \brief 设置默认文本颜色
 * \param id 控件ID
 * \param color 颜色
 */
static inline void guiWidgetInputDefaultTextColor(GUIid id, vec4 color)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    guiStrSetColor(data1->textDefault, color);
}

/**
 * \brief 获取文本内容
 * \param id 控件ID
 * \return 文本内容
 */
static inline wchar_t *guiWidgetInputGetText(GUIid id)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    return wcsdup(data1->textplain);
}

#endif // GUI_WIDGET_INPUT_H