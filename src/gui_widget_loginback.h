#ifndef GUI_WIDGET_LOGINBACK_H
#define GUI_WIDGET_LOGINBACK_H

#include <stb_image.h>
#include <cglm/cglm.h>
#include <stb_image_resize2.h>
#include <stb_image_write.h>

#include "gui.h"
#include "gui_draw.h"

#include "config.h"
#include "resource.h"

void gui_widget_loginback_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_loginback_logoffCall(GUIid id);   // 销毁控件时的函数

void gui_widget_loginback_loadCall(GUIid id);   // 添加控件时
void gui_widget_loginback_uploadCall(GUIid id); // 移除控件时

bool gui_widget_loginback_drawCall(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_loginback_eventCall(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

#endif // GUI_WIDGET_LOGINBACK_H