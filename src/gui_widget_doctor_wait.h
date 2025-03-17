#ifndef GUI_WIDGET_DPCTOR_WAIT_H
#define GUI_WIDGET_DPCTOR_WAIT_H

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "gui.h"
#include "gui_str.h"
#include "gui_draw.h"
#include "gui_type.h"

typedef struct
{
    GUIstr *str;
} GUIwidgetDoctorWait;

// 创建和销毁调用函数
void gui_widget_doctor_wait_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_doctor_wait_logoffCall(GUIid id);   // 销毁控件时的函数

// 回调函数
void gui_widget_doctor_wait_loadCall(GUIid id);   // 添加控件时
void gui_widget_doctor_wait_uploadCall(GUIid id); // 移除控件时

bool gui_widget_doctor_wait_drawCall(GUIid id); // 绘制, 返回值为是否继续调用其他响应函数

#endif // GUI_WIDGET_DPCTOR_WAIT_H