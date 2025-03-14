/**
 * \file gui_ID.h
 * \brief 为GUI创建和绑定唯一的ID
 */
#ifndef GUI_ID_H
#define GUI_ID_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "gui_type.h"

// GUI 的静态ID
enum
{
    GUI_ID_WINDOW = 0, // 窗口ID

    GUI_ID_MOUSEMOVE, // 鼠标移动窗口ID
    GUI_ID_LOG,       // 日志ID
    GUI_ID_LOGINBACK, // 登录背景ID

    GUI_ID_LOGIN_USER,    // 登录用户名文本框ID
    GUI_ID_LOGIN_PASS,    // 登录密码文本框ID
    GUI_ID_LOGIN_BUTTON1, // 登录提交按钮ID
    GUI_ID_LOGIN_BUTTON2, // 登录注册按钮ID
    GUI_ID_LOGIN_CHOICE,  // 注册选择用户医生类型

    GUI_ID_END
};

// 本想做成外部不可见，但是为了将获取ID转变成内联函数，所以暴露出来
typedef struct
{
    GUItype **id; // ID数组
    size_t count; // ID数组元素数量
    size_t index; // 动态ID的index, 表示下一个要创建的动态ID的起始地址
} GUIidList_t;
extern GUIidList_t GUIidList;

// 初始化ID列表
void guiIDInit();

// 删除ID列表
void guiIDQuit();

// 获取一个动态ID
GUIid guiIDCreateID();

/**
 * \brief 注册ID
 * \param id ID
 * \return ID
 */
GUIid guiIDRegister(GUIid id);

/**
 * \brief 注销ID
 * \param id ID
 */
void guiIDLogoff(GUIid id);

/**
 * \brief 获取ID
 * \param id ID
 */
static inline GUItype *guiIDGet(GUIid id)
{

    return GUIidList.id[id];
}

#endif // GUI_ID_H