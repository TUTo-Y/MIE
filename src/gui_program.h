#ifndef GUI_PROGRAM_H
#define GUI_PROGRAM_H

#include <stdbool.h>

#include "log.h"
#include "config.h"
#include "resource.h"

#include "gui_base.h"
#include "gui_fb.h"
#include "gui_shader.h"
#include "gui_texture.h"

// 存放所有的着色器程序
typedef struct
{
    GLuint img;
    GLuint rrt;       // 圆角纹理矩形
    GLuint rrc;       // 圆角颜色矩形
    GLuint rt;        // 纹理矩形
    GLuint rc;        // 颜色矩形
    GLuint gaussblur; // 高斯模糊
    GLuint icon;      // 图标
    GLuint font;      // 默认字体
    GLuint cc;        // 圆颜色
    GLuint cc2;       // 圆颜色2
    GLuint rrc2;      // 圆角颜色2
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
 * \brief 更新使用新的PV块
 * \param PV 新的PV块, 也就是mat4 pv;
 */
void guiProgramUpdatePV(mat4 PV);

/**
 * \brief 初始化PV矩阵
 * \param w 窗口宽度
 * \param h 窗口高度
 * \param PV 保存的PV矩阵
 */
void guiProgramSetPV(int w, int h, mat4 PV);

#endif // GUI_PROGRAM_H