/**
 * \file gui_window.h
 * \brief GUI window 控制器
 */
#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stb_truetype.h>

#include "list.h"

#include "gui_frame.h"

#include "gui.h"
#include "gui_type.h"
#include "gui_widget.h"
#include "gui_widgetID.h"

/**
 * \brief 初始化窗口控制器
 * \param win 窗口控制器
 */
void guiWindowInit(GUIwin *win, GLFWwindow *window);

/**
 * \brief 将控件添加到窗口
 * \param win 窗口控制器
 * \param widget 控件ID
 */
void guiWindowAddWidget(GUIwin *win, uint64_t id);

/**
 * \brief 从窗口中移除控件
 * \param win 窗口控制器
 * \param widget 控件ID
 */
void guiWindowRemoveWidget(GUIwin *win, uint64_t id);

/**
 * \brief 启动窗口
 * \param win 窗口控制器
 */
void guiWindowStart(GUIwin *win);

/**
 * \brief 退出窗口
 */
void guiWindowQuit(GUIwin *win);

#endif // GUI_WINDOW_H
