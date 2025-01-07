#include "guiframe.h"
#include <time.h>

void guiFrameInit(GUIframe *frame, int frameRate)
{
    frame->start = glfwGetTime();
    frame->frameRate = 1.0 / frameRate;
}

void guiFrameControl(GUIframe *frame)
{
    double currentTime = glfwGetTime();
    double elapsedTime = currentTime - frame->start;

    if (elapsedTime < frame->frameRate)
    {
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = (long)((frame->frameRate - elapsedTime) * 1e9);
        nanosleep(&ts, NULL);
    }

    // 更新开始时间
    frame->start += frame->frameRate;

    // 如果实际时间超过了预期时间，调整开始时间
    while (glfwGetTime() - frame->start > frame->frameRate)
    {
        frame->start += frame->frameRate;
    }
}