#ifndef GUI_WIDGET_LOGIN_CHOICE_H
#define GUI_WIDGET_LOGIN_CHOICE_H

#include "gui.h"

// 创建和销毁调用函数
void gui_widget_login_choice_StartCall(GUIwidget *widget);   // 注册控件时的函数
void gui_widget_login_choice_DestroyCall(GUIwidget *widget); // 销毁控件时的函数

// 回调函数
void gui_widget_login_choice_init(GUIwin *win, GUIwidget *widget);                           // 添加控件时
void gui_widget_login_choice_destroy(GUIwin *win, GUIwidget *widget);                        // 移除控件时
void gui_widget_login_choice_msg(GUIwin *win, GUIwidget *widget, uint64_t type, void *data); // 控件消息

bool gui_widget_login_choice_callDraw(GUIwin *win, GUIwidget *widget);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_login_choice_callEvent(GUIwin *win, GUIwidget *widget, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

#endif // GUI_WIDGET_LOGIN_CHOICE_H