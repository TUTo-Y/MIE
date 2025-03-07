#ifndef GUI_WIDGET_LOG_H
#define GUI_WIDGET_LOG_H

#include "list.h"
#include "vector.h"

#include "gui_ID.h"
#include "gui_base.h"
#include "gui_draw.h"
#include "gui_str.h"
#include "gui_type.h"

#define GUI_WIDGET_LOG_MSG 0   // 消息
#define GUI_WIDGET_LOG_WARN 1  // 警告
#define GUI_WIDGET_LOG_ERROR 2 // 错误

typedef struct
{
    wchar_t *msg; // 消息
    size_t type;  // 类型
    double time;  // 加入的时间
} GUIlogmsg;

typedef struct
{
    list listA; // 消息列表, 渲染列表
    list listB; // 消息列表, 添加列表
    list listT; // 临时列表

    pthread_mutex_t mutex; // 互斥锁, 用于其他线程向listB添加消息

    vec4 color[3]; // 颜色, 消息颜色，警告颜色，错误颜色

    GUIdrawrr drawrr; // 渲染器
} GUIwidgetLog;

// 创建和销毁调用函数
void gui_widget_log_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_log_logoffCall(GUIid id);   // 销毁控件时的函数
bool gui_widget_log_drawCall(GUIid id);     // 绘制, 返回值为是否继续调用其他响应函数

// 添加消息
void guiWidgetLogAddMsg(GUIid id, const wchar_t *msg, size_t type);

// 清空消息
void freeLogmsg(void *arg);

#endif // GUI_WIDGET_LOG_H