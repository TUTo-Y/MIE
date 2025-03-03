#include "gui_window.h"

void guiWindowMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);

    // 事件
    GUIevent event;
    event.type = GUI_EVENT_TYPE_MOUSE_BUTTON;
    event.MouseButton.button = button;
    event.MouseButton.action = action;
    event.MouseButton.mods = mods;

    // 事件回调
    guiControlRunEventCallback(&GUI_GETCONTROLP(win)->EventMouseButton, &event);
}

void guiWindowCursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);

    // 事件
    GUIevent event;
    event.type = GUI_EVENT_TYPE_CURSOR_POS;
    event.CursorPos.xpos = xpos;
    event.CursorPos.ypos = ypos;

    // 事件回调
    guiControlRunEventCallback(&GUI_GETCONTROLP(win)->EventCursorPos, &event);
}

void guiWindowCharModsCallback(GLFWwindow *window, unsigned int codepoint, int mods)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);

    // 事件
    GUIevent event;
    event.type = GUI_EVENT_TYPE_CHAR_MODS;
    event.CharMods.codepoint = codepoint;
    event.CharMods.mods = mods;

    // 事件回调
    guiControlRunEventCallback(&GUI_GETCONTROLP(win)->EventCharMods, &event);
}

void guiWindowScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);

    // 事件
    GUIevent event;
    event.type = GUI_EVENT_TYPE_SCROLL;
    event.Scroll.xoffset = xoffset;
    event.Scroll.yoffset = yoffset;

    // 事件回调
    guiControlRunEventCallback(&GUI_GETCONTROLP(win)->EventScroll, &event);
}

GUIid guiWindowCreate(GUIid id, GLFWwindow *window)
{
    GUIwin *win = (GUIwin *)guiIDGet(id);

    // 窗口
    win->window = window;
    glfwSetWindowUserPointer(window, win);

    // 任务初始化
    sem_init(&win->semTask, 0, 0);
    listInitList(&win->listTask);
    pthread_mutex_init(&win->mutexTask, NULL);

    // 控制回调初始化
    guiControlInit(GUI_GETCONTROLP(win));

    return id;
}

GUIid guiWindowDestroy(GUIid id, bool isDelete)
{
    GUIwin *win = (GUIwin *)guiIDGet(id);

    // 控制回调销毁
    guiControlDestroy(GUI_GETCONTROLP(win));

    // 任务销毁
    sem_destroy(&win->semTask);
    listDeleteList(&win->listTask, free);
    pthread_mutex_destroy(&win->mutexTask);

    // 窗口
    if (isDelete == true)
        guiIDLogoff(id);

    return id;
}

void guiWindowStart(GUIid id)
{
    GUIwin *win = (GUIwin *)guiIDGet(id);

    // 设置各个回调函数
    glfwSetMouseButtonCallback(win->window, guiWindowMouseButtonCallback);
    glfwSetCursorPosCallback(win->window, guiWindowCursorPosCallback);
    glfwSetCharModsCallback(win->window, guiWindowCharModsCallback);
    glfwSetScrollCallback(win->window, guiWindowScrollCallback);

    // 混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 抗锯齿
    glEnable(GL_MULTISAMPLE);

    // 帧率控制
    GUIFrame *frame = guiFrameCreate(30);

    // 设置清除颜色
    GUI_SET_DEFAULT_CLEARCOLOR();

    // 开始渲染
    glfwShowWindow(win->window);

    while (!glfwWindowShouldClose(win->window))
    {
        glfwWaitEvents();

        
        // 处理任务
        if (0 == sem_trywait(&win->semTask))
        {
            DEBUG("有任务产生\n");
            // pthread_mutex_lock(&win->mutexTask);
            // list *node;
            // while (node = listGetNodeFromStart(&win->listTask))
            // {
            //     // 获取任务
            //     GUItask *task = (GUItask *)node->data;

            //     // 处理任务
            //     pthread_mutex_unlock(&win->mutexTask);
            //     task->task(win, task->data, task->data2);

            //     // 释放任务
            //     listDeleteNode(NULL, node, free);

            //     // 重新加锁
            //     pthread_mutex_lock(&win->mutexTask);
            // }
            // pthread_mutex_unlock(&win->mutexTask);
        }

        
        // 渲染界面
        if (guiFrameCheck(frame))
        {
            // 清空颜色缓冲区
            glClear(GL_COLOR_BUFFER_BIT);

            // 调用渲染回调函数
            guiControlRunEventCallback(&GUI_GETCONTROLP(win)->Draw, NULL);

            // 交换缓冲区
            glfwSwapBuffers(win->window);
        }

    }
    guiFrameDestroy(frame);
}