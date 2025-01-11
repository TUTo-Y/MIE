/**
 * @file guiframe.h
 * @brief 帧率控制
 */
#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include <time.h>
#include <GLFW/glfw3.h>

typedef struct
{
    double start;     // 开始时间
    double frameRate; // 帧率
} GUIframe;

/**
 * @brief 初始化帧率控制
 * @param frame 帧率控制结构体
 * @param frameRate 帧率
 */
void guiFrameInit(GUIframe *frame, int frameRate);

/**
 * @brief 控制帧率
 * @param frame 帧率控制结构体
 * @param frameRate 帧率
 */
void guiFrameControl(GUIframe *frame);

#endif // GUI_FRAME_H