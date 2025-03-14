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

#define GUI_WIDGET_LOGINBACK_DRAW_ICON 0x20

typedef struct
{
    // 基础数据
    GLuint loginbackTex;   // 背景纹理
    GLuint loginbackr2Tex; // 圆角背景纹理
    int imageW, imageH;    // 图像大小
    float disW, disH;      // 图像边距

    // 渲染数据
    GUIdrawr loginback;    // 背景
    GUIdrawrr loginbackr2; // 圆角背景

    GUIicon user;    // 用户图标
    GUIicon pass;    // 密码图标
    GLuint userIcon; // 用户图标
    GLuint passIcon; // 密码图标
    bool drawIcon;   // 是否渲染图标

    // 物理数据
    int movX, movY;   // 当前位置
    int movDX, movDY; // 要移动的目标
    double animTime;  // 动画时间

} gui_widget_loginback_struct;

void gui_widget_loginback_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_loginback_logoffCall(GUIid id);   // 销毁控件时的函数

void gui_widget_loginback_loadCall(GUIid id);   // 添加控件时
void gui_widget_loginback_uploadCall(GUIid id); // 移除控件时

bool gui_widget_loginback_drawCall(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_loginback_eventCall(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

#endif // GUI_WIDGET_LOGINBACK_H