#include "gui_window.h"

GUIsize windowSize = {WINDOW_WIDTH, WINDOW_HEIGHT};
GUIpos CursorPos = {0, 0};

void guiWindowMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);

    // 事件
    GUIevent event;
    event.type = GUI_EVENT_TYPE_MOUSE_BUTTON;
    event.MouseButton.button = button;
    event.MouseButton.action = action;
    event.MouseButton.mods = mods;

    // 复制其余数据
    guiPosCopy(&CursorPos, &event.mousePos);
    guiSizeCopy(&windowSize, &event.fatherSize);

    // 事件回调
    guiControlRunEventCallback(&GUI_GETCONTROLP(win)->EventMouseButton, &event);
}

void guiWindowCursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);

    // 事件
    GUIevent event;
    event.type = GUI_EVENT_TYPE_CURSOR_POS;
    event.CursorPos.xpos = WINDOW_POS_2_GL_POS_x(xpos, windowSize.width);
    event.CursorPos.ypos = WINDOW_POS_2_GL_POS_y(ypos, windowSize.height);

    // 更新鼠标位置
    CursorPos.x = event.CursorPos.xpos;
    CursorPos.y = event.CursorPos.ypos;

    // 复制其余数据
    guiPosCopy(&CursorPos, &event.mousePos);
    guiSizeCopy(&windowSize, &event.fatherSize);

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

    // 复制其余数据
    guiPosCopy(&CursorPos, &event.mousePos);
    guiSizeCopy(&windowSize, &event.fatherSize);

    // 事件回调
    guiControlRunEventCallback(&GUI_GETCONTROLP(win)->EventCharMods, &event);
}

void guiWindowKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);

    // 事件
    GUIevent event;
    event.type = GUI_EVENT_TYPE_KEY;
    event.Key.key = key;
    event.Key.scancode = scancode;
    event.Key.action = action;
    event.Key.mods = mods;

    // 复制其余数据
    guiPosCopy(&CursorPos, &event.mousePos);
    guiSizeCopy(&windowSize, &event.fatherSize);

    // 事件回调
    guiControlRunEventCallback(&GUI_GETCONTROLP(win)->EventKey, &event);
}

void guiWindowScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);

    // 事件
    GUIevent event;
    event.type = GUI_EVENT_TYPE_SCROLL;
    event.Scroll.xoffset = xoffset;
    event.Scroll.yoffset = yoffset;

    // 复制其余数据
    guiPosCopy(&CursorPos, &event.mousePos);
    guiSizeCopy(&windowSize, &event.fatherSize);

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
#if 1
void guiWindowStart(GUIid id)
{
    GUIwin *win = (GUIwin *)guiIDGet(id);

    // 设置初始位置信息
    glfwGetCursorPos(win->window, &CursorPos.x, &CursorPos.y);
    CursorPos.x = WINDOW_POS_2_GL_POS_x(CursorPos.x, windowSize.width);
    CursorPos.y = WINDOW_POS_2_GL_POS_y(CursorPos.y, windowSize.height);

    // 设置各个回调函数
    glfwSetMouseButtonCallback(win->window, guiWindowMouseButtonCallback);
    glfwSetCursorPosCallback(win->window, guiWindowCursorPosCallback);
    glfwSetCharModsCallback(win->window, guiWindowCharModsCallback);
    glfwSetKeyCallback(win->window, guiWindowKeyCallback);
    glfwSetScrollCallback(win->window, guiWindowScrollCallback);

    // 混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 抗锯齿
    glEnable(GL_MULTISAMPLE);

    // 模板测试
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glStencilMask(0xFF);

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
            pthread_mutex_lock(&win->mutexTask);
            list *node = win->listTask.fd;
            while (node != &win->listTask)
            {
                list *tmp = node;
                node = node->fd;
                GUItask *task = tmp->data;

                pthread_mutex_unlock(&win->mutexTask);
                bool ret = guiTaskRunTask(task);
                pthread_mutex_lock(&win->mutexTask);

                if (ret)
                {
                    listDeleteNode(&win->listTask, tmp, free);
                }
                else
                {
                    sem_post(&win->semTask);
                }
            }
            pthread_mutex_unlock(&win->mutexTask);
        }

        // 渲染界面
        if (guiFrameCheck(frame))
        {
            // 清空颜色缓冲区
            glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 调用渲染回调函数
            guiControlRunEventCallback(&GUI_GETCONTROLP(win)->Draw, NULL);

            // 交换缓冲区
            glfwSwapBuffers(win->window);
        }
    }
    guiFrameDestroy(frame);
}
#else

void guiWindowStart(GUIid id)
{
    GUIwin *win = (GUIwin *)guiIDGet(id);

    GUIstr *str = guiStrCreate(font_default, 64, GUI_STR_MOD_CENTER, program.font, NULL, (vec4){1, 1, 1, 0.8f});
    guiStrCpy(str, L"Hello World!");

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

        // 渲染界面
        if (guiFrameCheck(frame))
        {
            // 清空颜色缓冲区
            glClear(GL_COLOR_BUFFER_BIT);

            guiStrRender(str);

            // 交换缓冲区
            glfwSwapBuffers(win->window);
        }
    }
    guiFrameDestroy(frame);
}
#endif