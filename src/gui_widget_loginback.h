#ifndef GUI_WIDGET_LOGINBACK_H
#define GUI_WIDGET_LOGINBACK_H

#include <stb_image.h>
#include <cglm/cglm.h>

#include "gui.h"
#include "config.h"
#include "resource.h"

void gui_widget_loginback_StartCall(GUIwidget *widget);   // 注册控件时的函数
void gui_widget_loginback_DestroyCall(GUIwidget *widget); // 销毁控件时的函数

void gui_widget_loginback_init(GUIwin *win, GUIwidget *widget);                           // 添加控件时
void gui_widget_loginback_destroy(GUIwin *win, GUIwidget *widget);                        // 移除控件时
void gui_widget_loginback_msg(GUIwin *win, GUIwidget *widget, uint64_t type, void *data); // 控件消息

bool gui_widget_loginback_callDraw(GUIwin *win, GUIwidget *widget);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_loginback_callEvent(GUIwin *win, GUIwidget *widget, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

#endif // GUI_WIDGET_LOGINBACK_H