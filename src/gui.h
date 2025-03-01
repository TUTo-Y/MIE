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
#include "gui_draw.h"
#include "gui_frame.h"
#include "gui_shader.h"
#include "gui_texture.h"
#include "gui_program.h"


#include "gui_type.h"
#include "gui_window.h"
#include "gui_widget.h"
#include "gui_widgetID.h"

#include "gui_widget_mousemove.h"
#include "gui_widget_loginback.h"

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 954

#define WINDOW_POS_2_GL_POS_x(x) (-((WINDOW_WIDTH / 2) - (x)))
#define WINDOW_POS_2_GL_POS_y(y) ((WINDOW_HEIGHT / 2) - (y))
#define GL_POS_2_WINDOW_POS_x(x) ((x) + (WINDOW_WIDTH / 2))
#define GL_POS_2_WINDOW_POS_y(y) ((WINDOW_HEIGHT / 2) - (y))

#define PI 3.14159265358979323846

#define CALL(fun, ...) ((fun) ? (fun)(__VA_ARGS__) : false)



#endif // GUI_H