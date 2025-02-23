/**
 * \file gui_widget.h
 * \brief GUI 控件
 */

#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include <pthread.h>
#include <semaphore.h>

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stb_truetype.h>

#include "gui_window.h"
#include "list.h"

#define GUI_WIDGET_FLAG_VISIBLE 0x00000001
#define GUI_WIDGET_FLAG_ENABLE 0x00000002

typedef struct _GUIwin GUIwin;
typedef struct _GUIevent GUIevent;
typedef struct _GUIwidget GUIwidget;
typedef struct _GUIwidget
{
    uint64_t flag; // 控件标志
    uint64_t id;   // 控件ID

    // 回调函数
    void (*init)(GUIwin *win, GUIwidget *widget);                            // 初始化
    void (*destroy)(GUIwin *win, GUIwidget *widget);                         // 销毁
    void (*msg)(GUIwin *win, GUIwidget *widget, uint64_t *type, void *data); // 消息

    bool (*callDraw)(GUIwin *win, GUIwidget *widget);                   // 绘制, 返回值为是否继续调用其他响应函数
    bool (*callEvent)(GUIwin *win, GUIwidget *widget, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

    // 设置回调函数优先级
    int priorityDraw;             // 绘制优先级, 如果没有该事件的响应，应当为-1
    int priorityEventMouseButton; // 鼠标事件优先级, 如果没有该事件的响应，应当为-1
    int priorityEventCursorPos;   // 光标事件优先级, 如果没有该事件的响应，应当为-1
    int priorityEventCharMods;    // 字符事件优先级, 如果没有该事件的响应，应当为-1
    int priorityEventScroll;      // 滚轮事件优先级, 如果没有该事件的响应，应当为-1

    // 空间其他数据
    void *data;  // 通常用于存储控件数据
    void *data2; // 通常不调用

    // 窗口指针
    GUIwin *win; // 绑定的窗口

    // 创建和销毁调用函数
    void (*StartCall)(GUIwidget *widget);   // 注册控件时的函数
    void (*DestroyCall)(GUIwidget *widget); // 销毁控件时的函数

} GUIwidget;

/**
 * \brief 初始化并注册控件
 */
void guiWidgetInit(GUIwidget *widget,
                   uint64_t flag,
                   uint64_t id,
                   void (*init)(GUIwin *win, GUIwidget *widget),
                   void (*destroy)(GUIwin *win, GUIwidget *widget),
                   void (*msg)(GUIwin *win, GUIwidget *widget, uint64_t *type, void *data),
                   bool (*callDraw)(GUIwin *win, GUIwidget *widget),
                   bool (*callEvent)(GUIwin *win, GUIwidget *widget, const GUIevent *event),
                   int priorityDraw,
                   int priorityEventMouseButton,
                   int priorityEventCursorPos,
                   int priorityEventCharMods,
                   int priorityEventScroll,
                   void *data,
                   void *data2,
                   void (*StartCall)(GUIwidget *widget),
                   void (*DestroyCall)(GUIwidget *widget));

/**
 * \brief 销毁控件
 */
void guiWidgetDestroy(GUIwidget *widget);

#endif // GUI_WIDGET_H