/**
 * @file gui_type.h
 * @brief GUI 控件类型
 */

#ifndef GUI_TYPE_H
#define GUI_TYPE_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stb_truetype.h>

#include "list.h"

// ID控件
enum
{
    GUI_WIDGET_ID_START,
    GUI_WIDGET_ID_LOGIN_BACK, // 登录背景
    GUI_WIDGET_ID_MOUSEMOVE, // 鼠标移动窗口控件
    GUI_WIDGET_ID_MAX
};

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
 * \brief 窗口处理优先级
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

typedef struct _GUIwidget GUIwidget;
typedef struct _GUIwidget
{
    uint64_t flag; // 控件标志
    uint64_t id;   // 控件ID

    GUIwin *win; // 绑定的窗口

    // 创建和销毁调用函数
    void (*StartCall)(GUIwidget *widget);   // 注册控件时的函数
    void (*DestroyCall)(GUIwidget *widget); // 销毁控件时的函数

    // 回调函数
    void (*init)(GUIwin *win, GUIwidget *widget);                           // 添加控件时
    void (*destroy)(GUIwin *win, GUIwidget *widget);                        // 移除控件时
    void (*msg)(GUIwin *win, GUIwidget *widget, uint64_t type, void *data); // 控件消息

    bool (*callDraw)(GUIwin *win, GUIwidget *widget);                         // 绘制, 返回值为是否继续调用其他响应函数
    bool (*callEvent)(GUIwin *win, GUIwidget *widget, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

    // 设置回调函数优先级
    int priorityDraw;             // 绘制优先级
    int priorityEventMouseButton; // 鼠标事件优先级, -1表示不处理
    int priorityEventCursorPos;   // 光标事件优先级, -1表示不处理
    int priorityEventCharMods;    // 字符事件优先级, -1表示不处理
    int priorityEventScroll;      // 滚轮事件优先级, -1表示不处理

    // 空间其他数据
    void *data;  // 通常用于存储控件数据
    void *data2; // 通常不调用

} GUIwidget;

#endif // GUI_TYPE_H