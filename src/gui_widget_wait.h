#ifndef GUI_WIDGET_WAIT_H
#define GUI_WIDGET_WAIT_H

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "gui.h"
#include "gui_str.h"
#include "gui_draw.h"
#include "stb_image.h"

typedef struct
{
    stbi_uc *imgData; // 图片数据
    int *delays;      // 每一帧的延迟
    int x, y;         // 图片大小
    int count;        // 帧数

    GLuint VAO, VBO, EBO; // VAO VBO EBO
    GLuint *texture;      // 纹理

    mat4 model; // 模型矩阵

    double time; // 已间隔时间
    int index;   // 当前帧

} GUIwidgetwait;

// 创建和销毁调用函数
void gui_widget_wait_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_wait_logoffCall(GUIid id);   // 销毁控件时的函数

// 回调函数
void gui_widget_wait_loadCall(GUIid id);   // 添加控件时
void gui_widget_wait_uploadCall(GUIid id); // 移除控件时

bool gui_widget_wait_drawCall(GUIid id); // 绘制, 返回值为是否继续调用其他响应函数

// 设置模型矩阵
static inline void guiWidgetWaitSetModel(GUIid id, mat4 model)
{
    GUIwidgetwait *st = GUI_ID2WIDGET(id)->data1;
    glm_mat4_copy(model, st->model);
}

#endif // GUI_WIDGET_WAIT_H