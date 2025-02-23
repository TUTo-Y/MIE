/**
 * \file gui_window.h
 * \brief GUI window 控制器
 */
#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <pthread.h>
#include <semaphore.h>

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stb_truetype.h>

#include "list.h"

#include "gui.h"
#include "gui_widget.h"
#include "gui_widgetID.h"

/**
 * \brief GUI 处理优先级
 */
#define GUI_CALL_PRIORITY_0 0
#define GUI_CALL_PRIORITY_1 1
#define GUI_CALL_PRIORITY_2 2
#define GUI_CALL_PRIORITY_3 3
#define GUI_CALL_PRIORITY_4 4
#define GUI_CALL_PRIORITY_NUM 5
#define GUI_CALL_PRIORITY_SAFE_GET(n) ((n) % GUI_CALL_PRIORITY_NUM)

typedef struct _GUIwin
{
    GLFWwindow *window; // 窗口

    // 任务
    sem_t semTask;             // 任务信号量
    list listTask;             // 任务列表
    pthread_mutex_t mutexTask; // 任务锁

    // 渲染列表
    list listDraw[GUI_CALL_PRIORITY_NUM]; // 渲染任务列表

    // 事件列表
    list listEventMouseButton[GUI_CALL_PRIORITY_NUM]; // 鼠标事件任务列表
    list listEventCursorPos[GUI_CALL_PRIORITY_NUM];   // 光标事件任务列表
    list listEventCharMods[GUI_CALL_PRIORITY_NUM];    // 字符事件任务列表
    list listEventScroll[GUI_CALL_PRIORITY_NUM];      // 滚轮事件任务列表

    // 控件列表
    list listWidget; // 控件列表
} GUIwin;

// 事件类型
enum
{
    GUI_EVENT_TYPE_MOUSE_BUTTON,
    GUI_EVENT_TYPE_CURSOR_POS,
    GUI_EVENT_TYPE_CHAR_MODS,
    GUI_EVENT_TYPE_SCROLL
};

// 事件
typedef struct _GUIevent
{
    uint64_t type; // 事件类型
    union
    {
        struct // 鼠标事件
        {
            int button; // 按键
            int action; // 动作
            int mods;   // 修饰键
        } MouseButton;
        struct // 光标事件
        {
            double xpos; // 鼠标X坐标
            double ypos; // 鼠标Y坐标
        } CursorPos;
        struct // 字符事件
        {
            unsigned int codepoint; // 字符
            int mods;               // 修饰键
        } CharMods;
        struct // 滚轮事件
        {
            double xoffset; // X轴偏移
            double yoffset; // Y轴偏移
        } Scroll;
    };

} GUIevent;

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
