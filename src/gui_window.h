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
 * \brief 窗口处理任务函数
 */
typedef void (*GUIwindowtask)(GUIwin *win, void *data, void *data2);

typedef struct
{
    GUIwindowtask task; // 任务
    void *data;         // 数据
    void *data2;        // 数据2
}GUItask;

/**
 * \brief 向窗口添加任务
 */
void guiWindowAddTask(GUIwin *win, GUIwindowtask task, void *data, void *data2);

/**
 * \brief 初始化窗口控制器
 * \param win 窗口控制器
 */
void guiWindowInit(GUIwin *win, GLFWwindow *window);

/**
 * \brief 将控件添加到窗口
 * \param win 窗口控制器
 * \param widget 控件ID
 * \param priorityDraw 相同优先级的控件优先绘制
 * \param priorityEventMouseButton 相同优先级的控件优先处理鼠标事件
 * \param priorityEventCursorPos 相同优先级的控件优先处理光标事件
 * \param priorityEventCharMods 相同优先级的控件优先处理字符事件
 * \param priorityEventScroll 相同优先级的控件优先处理滚动事件
 * \note 
 */
void guiWindowAddWidget(GUIwin *win, uint64_t id, bool priorityDraw, bool priorityEventMouseButton, bool priorityEventCursorPos, bool priorityEventCharMods, bool priorityEventScroll);

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
