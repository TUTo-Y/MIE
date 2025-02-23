/**
 * \file gui_widgetID.h
 * \brief GUI 控件ID
 */

#ifndef GUI_WIDGETID_H
#define GUI_WIDGETID_H

#include "log.h"

#include "gui_widget.h"

enum
{
    GUI_WIDGET_ID_START,
    GUI_WIDGET_ID_MOUSEMOVE, // 鼠标移动窗口控件
    GUI_WIDGET_ID_MAX
};

typedef struct _GUIwidget GUIwidget;

/**
 * \brief 注册控件ID
 */
bool guiIDRegister(uint64_t id, GUIwidget *widget);

/**
 * \brief 根据控件ID获取控件
 */
GUIwidget *guiIDGetFromID(uint64_t id);

/**
 * \brief 根据控件地址获取控件ID
 * \param widget 控件地址
 */
uint64_t guiIDGetFromWidget(GUIwidget *widget);

#endif // GUI_WIDGETID_H