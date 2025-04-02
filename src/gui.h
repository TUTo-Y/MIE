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

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <Commctrl.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>


#include "log.h"
#include "list.h"
#include "config.h"

#include "gui_base.h"
#include "gui_fb.h"
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

#include "gui_widget_log.h"
#include "gui_widget_input.h"
#include "gui_widget_button.h"
#include "gui_widget_mousemove.h"
#include "gui_widget_loginback.h"
#include "gui_widget_login_choice.h"
#include "gui_widget_img.h"
#include "gui_widget_wait.h"
#include "gui_widget_text.h"
#include "gui_widget_img_choice.h"

extern HWND hEdit;  // 编辑框句柄
extern HWND hEdit2;  // 编辑框句柄
extern HFONT hFont; // 字体句柄

bool guiInit();

void guiQuit();

void guiSetDefaultClearColor(float r, float g, float b, float a);

#endif // GUI_H