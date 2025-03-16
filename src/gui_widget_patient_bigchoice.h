/**
 * @file gui_widget_patient_bigchoice.h
 * @brief GUI 患者界面巨大的加号控件
 */
#ifndef GUI_WIDGET_PATIENT_BIGCHOICE_H
#define GUI_WIDGET_PATIENT_BIGCHOICE_H

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stb_truetype.h>

#include "choice_file.h"
#include "gui_type.h"
#include "gui_ID.h"
#include "gui_draw.h"
#include "gui.h"
#include "patient.h"

typedef struct
{
    // size_t flag; // 控件标志
    bool press;

    GUIdrawrr back; // 背景圆矩形
    GUIdrawrr t1;   // 加号的两个矩形
    GUIdrawrr t2;   // 加号的两个矩形

} gui_widget_patient_bigchoice;

// 创建和销毁调用函数
void gui_widget_patient_bigchoice_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_patient_bigchoice_logoffCall(GUIid id);   // 销毁控件时的函数

bool gui_widget_patient_bigchoice_drawCall(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_patient_bigchoice_eventCall(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

#endif // GUI_WIDGET_PATIENT_BIGCHOICE_H
