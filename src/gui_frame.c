#include "gui_frame.h"

void guiFrameControl(GUIFrame *param)
{
    double start = glfwGetTime();

    pthread_mutex_lock(&param->lock);
    while (0 != sem_trywait(&param->semQuit))
    {
        double intervalTime = glfwGetTime() - start;

        if (intervalTime < param->frameRate)
        {
            pthread_mutex_unlock(&param->lock);

            // 发送信号
            sem_post(&param->semWait);

            // 抛出空事件
            glfwPostEmptyEvent();

            // 等待事件
            struct timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = (long)((param->frameRate - intervalTime) * 1e9);
            nanosleep(&ts, NULL);

            pthread_mutex_lock(&param->lock);
        }

        // 更新开始时间
        start += param->frameRate;

        // 如果实际时间超过了预期时间，调整开始时间
        double checkTime = glfwGetTime() - start - param->frameRate;
        if (checkTime > 0)
        {
            start += checkTime;
        }
    }
    pthread_mutex_unlock(&param->lock);
}

GUIFrame *guiFrameCreate(int frameRate)
{
    GUIFrame *param = (GUIFrame *)malloc(sizeof(GUIFrame));
    if (param == NULL)
    {
        return NULL;
    }

    param->frameRate = 1.0 / frameRate;

    pthread_mutex_init(&param->lock, NULL);
    sem_init(&param->semWait, 0, 0);
    sem_init(&param->semQuit, 0, 0);

    pthread_create(&param->guiFrameTID, NULL, (void *(*)(void *))guiFrameControl, param);

    return param;
}

void guiFrameDestroy(GUIFrame *param)
{
    // 通知线程退出
    sem_post(&param->semQuit);

    // 等待线程退出
    pthread_join(param->guiFrameTID, NULL);

    // 释放资源
    sem_destroy(&param->semWait);
    sem_destroy(&param->semQuit);
    pthread_mutex_destroy(&param->lock);
    free(param);
}

void guiFrameSet(GUIFrame *param, int frameRate)
{
    pthread_mutex_lock(&param->lock);
    param->frameRate = 1.0 / frameRate;
    pthread_mutex_unlock(&param->lock);
}
