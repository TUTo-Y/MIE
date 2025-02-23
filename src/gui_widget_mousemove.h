/**
 * \file gui_widget_mousemove.h
 * \brief GUI 鼠标移动窗口控件
 */

#ifndef GUI_WIDGET_MOUSEMOVE_H
#define GUI_WIDGET_MOUSEMOVE_H

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stb_truetype.h>

#include "gui_window.h"
#include "gui_widget.h"
#include "gui_widgetID.h"

typedef struct _GUIwin GUIwin;
typedef struct _GUIevent GUIevent;
typedef struct _GUIwidget GUIwidget;

bool guiWidgetMouseMoveEvent(GUIwin *win, GUIwidget *widget, const GUIevent *event);

#endif // GUI_WIDGET_MOUSEMOVE_H