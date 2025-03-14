#include "gui_task.h"

void guiTaskAddTask(bool (*taskfun)(size_t flag, void *data), size_t flag, void *data)
{
    // 创建task
    GUItask task;
    task.taskfun = taskfun;
    task.flag = flag;
    task.data = data;

    // 向窗口添加任务
    GUIwin *win = (GUIwin *)guiIDGet(GUI_ID_WINDOW);
    pthread_mutex_lock(&win->mutexTask);
    listAddNodeInEnd(&win->listTask, listDataToNode(listCreateNode(), &task, sizeof(task), true));
    pthread_mutex_unlock(&win->mutexTask);

    // 通知窗口
    sem_post(&win->semTask);
    glfwPostEmptyEvent();
}

bool guiTaskRunTask(GUItask *task)
{
    if (task->taskfun)
        return task->taskfun(task->flag, task->data);
    else
        return true;
}