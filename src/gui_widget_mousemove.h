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

#include "gui_type.h"
#include "gui_ID.h"
#include "gui_str.h"
#include "gui_draw.h"

bool gui_widget_mousemove_eventCall(GUIid id, const GUIevent *event);

#endif // GUI_WIDGET_MOUSEMOVE_H