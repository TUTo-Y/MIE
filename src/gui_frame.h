/**
 * \file guiframe.h
 * \brief 帧率控制
 * 
 * 通过定时发送信号量的方式控制帧率，同时会抛出空事件，调用者只需要等待事件即可，根据信号进行渲染
 * 只能在主线程中调用
 */
#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <GLFW/glfw3.h>


typedef struct
{
    sem_t semWait; // 时间信号量
    sem_t semQuit; // 退出信号量

    double frameRate; // 帧率时间, 1.0 / frameRate

    pthread_mutex_t lock;  // 互斥锁，用于修改帧率
    pthread_t guiFrameTID; // 线程ID
} GUIFrame;

// 获取信号量
#define guiFrameGetSemWait(param) &((param)->semWait)

// 检查是否需要渲染
#define guiFrameCheck(param) (0 == sem_trywait(&((param)->semWait)))

// 帧率控制
GUIFrame *guiFrameCreate(int frameRate);

// 销毁帧率控制
void guiFrameDestroy(GUIFrame *param);

// // 获取等待信号量
// sem_t *guiFrameGetSemWait(GUIFrame *param);

// 设置帧率
void guiFrameSet(GUIFrame *param, int frameRate);
#endif // GUI_FRAME_H
