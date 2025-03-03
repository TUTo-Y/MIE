/**
 * \file gui_window.h
 * \brief GUI window 控制器
 */
#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "gui_type.h"
#include "gui_ID.h"
#include "gui_control.h"
#include "gui.h"

/**
 * \brief 创建一个窗口控件
 * \param 窗口ID
 * \param window 窗口
 * \note 该函数不会自动调用注册函数，需要使用guiWindowCreate(guiIDRegister(GUI_ID_WINDOW), ...)手动调用;
 */
GUIid guiWindowCreate(GUIid id, GLFWwindow *window);

/**
 * \brief 销毁一个窗口控件
 * \param id 窗口ID
 * \param isDelete 是否使用guiIDLogoff删除ID
 */
GUIid guiWindowDestroy(GUIid id, bool isDelete);

/**
 * \brief 开始渲染窗口
 * \param id 窗口ID
 */
void guiWindowStart(GUIid id);

#endif // GUI_WINDOW_H
