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

#include "list.h"
#include "vector.h"

#include "gui_base.h"

// ID类型
typedef uint64_t GUIid;

// GUI事件
#define GUI_EVENT_TYPE_MOUSE_BUTTON 0x2 // 鼠标点击事件
#define GUI_EVENT_TYPE_CURSOR_POS 0x4   // 光标事件
#define GUI_EVENT_TYPE_CHAR_MODS 0x8    // 字符事件
#define GUI_EVENT_TYPE_KEY 0x10         // 键盘事件
#define GUI_EVENT_TYPE_SCROLL 0x20      // 滚轮事件
#define GUI_EVENT_TYPE_CUSTOM 0x40      // 自定义事件
typedef struct
{
    uint64_t type; // 事件类型
    union
    {
        struct
        {
            int button; // 按键
            int action; // 动作
            int mods;   // 修饰键
        } MouseButton;  // 鼠标点击事件

        struct
        {
            double xpos; // 鼠标X坐标, 中心坐标系, 同cursorPos
            double ypos; // 鼠标Y坐标, 中心坐标系, 同cursorPos
        } CursorPos;     // 光标事件

        struct
        {
            unsigned int codepoint; // 字符
            int mods;               // 修饰键
        } CharMods;                 // 字符事件

        struct
        {
            int key;      // 键
            int scancode; // 扫描码
            int action;   // 动作
            int mods;     // 修饰键
        } Key;            // 键盘事件

        struct
        {
            double xoffset; // X轴偏移
            double yoffset; // Y轴偏移
        } Scroll;           // 滚轮事件

        struct
        {
            void *data1; // 数据1
            void *data2; // 数据2
        } Custom;        // 自定义事件
    };

    GUIpos mousePos;   // 鼠标位置，中心坐标系，CursorPos
    GUIsize fatherSize; // 父控件大小
} GUIevent;

// 回调函数列表
typedef struct
{
    GUIid id;          // 控件ID
    uint64_t priority; // 优先级
    bool enable;       // 是否启用

    // size_t type;       // 回调函数的类型
    union // 渲染中用于快速调用回调函数，不需要再根据id和type查找回调函数
    {
        bool (*drawCall)(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
        bool (*eventCall)(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数
    };

} GUIcallback;

// 控制回调
typedef struct
{
    // 渲染回调
    Vector Draw; // 绘制回调

    // 事件回调
    Vector EventMouseButton; // 鼠标事件回调
    Vector EventCursorPos;   // 光标事件回调
    Vector EventCharMods;    // 字符事件回调
    Vector EventKey;         // 键盘事件回调
    Vector EventScroll;      // 滚轮事件回调

    // 控件列表，存储指针指向GUItype
    list widget;
} GUIControl;

typedef struct
{
    GLFWwindow *window; // 窗口

    // 任务
    sem_t semTask;             // 任务信号量
    list listTask;             // 任务列表
    pthread_mutex_t mutexTask; // 任务锁

} GUIwin;

// 回调函数
#define GUI_WIDGET_CALLFLAG_DRAW 0x1                                  // 绘制回调
#define GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON 0x2                    // 鼠标事件回调
#define GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS 0x4                      // 光标事件回调
#define GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS 0x8                       // 字符事件回调
#define GUI_WIDGET_CALLFLAG_EVENT_KEY 0x10                            // 键盘事件回调
#define GUI_WIDGET_CALLFLAG_EVENT_SCROLL 0x20                         // 滚轮事件回调
#define GUI_WIDGET_CALLFLAG_EVENT (0x2 | 0x4 | 0x8 | 0x10 | 0x20)     // 所有事件回调
#define GUI_WIDGET_CALLFLAG_ALL (0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20) // 所有回调

typedef struct _GUIwidget GUIwidget;
typedef struct _GUIwidget
{
    uint64_t fid; // 父控件ID

    size_t flag; // 控件标志

    // 创建和销毁调用函数
    void (*registerCall)(GUIid id); // 注册控件时的函数
    void (*logoffCall)(GUIid id);   // 销毁控件时的函数

    // 回调函数
    void (*loadCall)(GUIid id);   // 添加控件时
    void (*uploadCall)(GUIid id); // 移除控件时

    bool (*drawCall)(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
    bool (*eventCall)(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

    // 空间其他数据
    void *data1; // 通常用于存储控件数据
    void *data2; // 通常不调用
} GUIwidget;

// GUI类型
typedef struct
{
    union
    {
        GUIwin win;
        GUIwidget widget;
    };

    uint64_t id;        // 控件ID
    GUIControl control; // 控制回调
} GUItype;

// 获取控件的ID
#define GUI_GETID(widget) (((GUItype *)widget)->id)

// 获取控件的control地址
#define GUI_GETCONTROLP(widget) (&((GUItype *)widget)->control)

// 获取窗口GUIwin
#define GUI_GETWINDOW() ((GUIwin *)guiIDGet(GUI_ID_WINDOW))

// 通过ID获取控件的地址
#define GUI_ID2WIDGET(id) ((GUIwidget *)guiIDGet(id))

// 通过ID获取控件的control地址
#define GUI_ID2CONTROLP(id) (GUI_GETCONTROLP(guiIDGet(id)))

// 通过ID查找父ID
#define GUI_ID2FID(id) (((GUIwidget *)guiIDGet(id))->fid)

// 通过ID获取父控件的control地址
#define GUI_ID2FCONTROLP(id) (GUI_GETCONTROLP(guiIDGet(GUI_ID2FID(id))))

#endif // GUI_TYPE_H