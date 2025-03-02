#ifndef GUI_PROGRAM_H
#define GUI_PROGRAM_H

#include <stdbool.h>

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#include "log.h"
#include "config.h"
#include "resource.h"

#include "gui_shader.h"

#include "gui.h"

// 存放所有的着色器程序
typedef struct
{
    GLuint img;
    GLuint rrt;        // 圆角纹理矩形
    GLuint rrc;        // 圆角颜色矩形
    GLuint rt;         // 纹理矩形
    GLuint rc;         // 颜色矩形
    GLuint gaussblur;  // 高斯模糊
    GLuint icon;       // 图标
} GUIprogram;

extern GUIprogram program;

/**
 * \brief 初始化所有的着色器程序
 */
bool guiProgramInit();

/**
 * \brief 销毁所有的着色器程序
 */
void guiProgramDestroy();

/**
 * \brief 初始化PV矩阵
 * \param w 窗口宽度
 * \param h 窗口高度
 * \param PV 保存的PV矩阵
 */
void guiProgramSetPV(int w, int h, mat4 PV);

#endif // GUI_PROGRAM_H