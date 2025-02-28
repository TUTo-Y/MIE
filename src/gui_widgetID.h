/**
 * \file gui_widgetID.h
 * \brief GUI 控件ID
 */

#ifndef GUI_WIDGETID_H
#define GUI_WIDGETID_H


#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stb_truetype.h>

#include "log.h"

#include "gui.h"
#include "gui_type.h"
#include "gui_window.h"
#include "gui_widget.h"

/**
 * \brief 注册控件ID
 */
bool guiIDRegister(uint64_t id, GUIwidget *widget);

/**
 * \brief 根据控件ID获取控件
 */
GUIwidget *guiIDGetFromID(uint64_t id);

#endif // GUI_WIDGETID_H