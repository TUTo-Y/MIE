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

#include "list.h"

#include "gui.h"
#include "gui_type.h"
#include "gui_window.h"
#include "gui_widgetID.h"

#define GUI_WIDGET_FLAG_VISIBLE 0x00000001
#define GUI_WIDGET_FLAG_ENABLE 0x00000002

/**
 * \brief 初始化并注册控件
 */
void guiWidgetInit(GUIwidget *widget,
                   uint64_t flag,
                   uint64_t id,
                   void (*init)(GUIwin *win, GUIwidget *widget),
                   void (*destroy)(GUIwin *win, GUIwidget *widget),
                   void (*msg)(GUIwin *win, GUIwidget *widget, uint64_t type, void *data),
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