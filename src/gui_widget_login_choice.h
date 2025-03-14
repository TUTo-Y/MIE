#ifndef GUI_WIDGET_LOGIN_CHOICE_H
#define GUI_WIDGET_LOGIN_CHOICE_H

#include "stb_image.h"

#include "gui_base.h"
#include "gui_type.h"
#include "gui_draw.h"
#include "gui_task.h"
#include "gui_str.h"
#include "gui_ID.h"

#include "gui_widget_log.h"
#include "gui_widget_loginback.h"

#include "login.h"

#define GUI_WIDGET_LOGIN_CHOICE_DOCTOR 1  // 选择了医生
#define GUI_WIDGET_LOGIN_CHOICE_PATIENT 2 // 选择了患者
#define GUI_WIDGET_LOGIN_CHOICE_PRESS 4   // 按下
typedef struct
{
    size_t flag; // 选择标志

    GLuint iconTex[2]; // 医生和患者的图标
    GUIicon icon[2];   // 医生和患者的图标
    GUIdrawrr back[2]; // 医生和患者的圆角背景

    GUIdrawcc cc; // 圆角背景
    double time;  // 动画时间

    GUIstr *str; // 选择文本
} GUIwidgetLoginChoice;

void gui_widget_login_choice_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_login_choice_logoffCall(GUIid id);   // 销毁控件时的函数

// 回调函数
void gui_widget_login_choice_loadCall(GUIid id);   // 添加控件时
void gui_widget_login_choice_uploadCall(GUIid id); // 移除控件时

bool gui_widget_login_choice_drawCall(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_login_choice_eventCall(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

// 恢复任务


#endif // GUI_WIDGET_LOGIN_CHOICE_H