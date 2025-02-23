/**
 * @file gui.h
 * @brief GUI界面的相关函数
 */
#ifndef GUI_H
#define GUI_H

#include <math.h>
#include <time.h>
#include <stdio.h>

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#include "gui_ttf.h"
#include "gui_str.h"
#include "gui_frame.h"
#include "gui_shader.h"
#include "gui_texture.h"

#include "gui_window.h"
#include "gui_widget.h"
#include "gui_widgetID.h"

#include "gui_widget_mousemove.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 618

#define PI 3.14159265358979323846

#define CALL(fun, ...) ((fun) ? (fun)(__VA_ARGS__) : false)

extern mat4 PV;

/**
 * \brief 初始化PV矩阵
 * \param w 窗口宽度
 * \param h 窗口高度
 */
void guiSetPV(int w, int h);

#endif // GUI_H