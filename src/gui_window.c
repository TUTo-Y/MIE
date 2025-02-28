#include "gui_window.h"

void guiWindowInit(GUIwin *win, GLFWwindow *window)
{
    memset(win, 0, sizeof(GUIwin));

    // 设置窗口的额外数据
    win->window = window;
    glfwSetWindowUserPointer(window, win);

    // 任务初始化
    sem_init(&win->semTask, 0, 0);
    listInitList(&win->listTask);
    pthread_mutex_init(&win->mutexTask, NULL);

    // 列表初始化
    for (int i = 0; i < GUI_CALL_PRIORITY_NUM; i++)
    {
        listInitList(&win->listDraw[i]);
        listInitList(&win->listEventMouseButton[i]);
        listInitList(&win->listEventCursorPos[i]);
        listInitList(&win->listEventCharMods[i]);
        listInitList(&win->listEventScroll[i]);
    }

    // 控件列表初始化
    listInitList(&win->listWidget);
}

void guiWindowAddWidget(GUIwin *win, uint64_t id)
{
    // 通过ID获取控件
    GUIwidget *widget = guiIDGetFromID(id);
    if (widget == NULL)
        return;

    // 检查ID是否已经绑定
    if (widget->win != NULL)
        return;

    // 绑定窗口
    widget->win = win;

    // 添加到控件列表
    listAddNodeInEnd(&win->listWidget, listDataToNode(listCreateNode(), widget, 0, false));

    // 添加渲染列表
    if (widget->priorityDraw >= 0)
        listAddNodeInEnd(&win->listDraw[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityDraw)],
                         listDataToNode(listCreateNode(), widget, 0, false));

    // 添加鼠标事件列表
    if (widget->priorityEventMouseButton >= 0)
        listAddNodeInEnd(&win->listEventMouseButton[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityEventMouseButton)],
                         listDataToNode(listCreateNode(), widget, 0, false));

    // 添加光标事件列表
    if (widget->priorityEventCursorPos >= 0)
        listAddNodeInEnd(&win->listEventCursorPos[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityEventCursorPos)],
                         listDataToNode(listCreateNode(), widget, 0, false));

    // 添加字符事件列表
    if (widget->priorityEventCharMods >= 0)
        listAddNodeInEnd(&win->listEventCharMods[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityEventCharMods)],
                         listDataToNode(listCreateNode(), widget, 0, false));

    // 添加滚轮事件列表
    if (widget->priorityEventScroll >= 0)
        listAddNodeInEnd(&win->listEventScroll[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityEventScroll)],
                         listDataToNode(listCreateNode(), widget, 0, false));

    // 调用初始化函数
    CALL(widget->init, win, widget);
}

void guiWindowRemoveWidget(GUIwin *win, uint64_t id)
{
    // 通过ID获取控件
    GUIwidget *widget = guiIDGetFromID(id);
    if (widget == NULL)
        return;

    // 检查ID是否已经绑定
    if (widget->win != win)
        return;

    // 解绑窗口
    widget->win = NULL;

    // 移除控件列表
    list *l = &win->listWidget;
    list *node = listSearchDataAddr(l, widget);
    if (node != NULL)
        listDeleteNode(l, node, NULL);
    else
        ERROR("没有找到ID为:%lld的控件\n", id);

    // 移除渲染列表
    if (widget->priorityDraw >= 0)
    {
        list *l = &win->listDraw[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityDraw)];
        list *tmp = l->fd;
        while (tmp != l)
        {
            if (tmp->data == widget)
            {
                listDeleteNode(l, tmp, NULL);
                break;
            }
            tmp = tmp->fd;
        }
    }

    // 移除鼠标事件列表
    if (widget->priorityEventMouseButton >= 0)
    {
        list *l = &win->listEventMouseButton[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityEventMouseButton)];
        list *node = listSearchDataAddr(l, widget);
        if (node != NULL)
            listDeleteNode(l, node, NULL);
        else
            ERROR("没有找到ID为:%lld的控件的鼠标事件回调函数\n", id);
    }

    // 移除光标事件列表
    if (widget->priorityEventCursorPos >= 0)
    {
        list *l = &win->listEventCursorPos[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityEventCursorPos)];
        list *node = listSearchDataAddr(l, widget);
        if (node != NULL)
            listDeleteNode(l, node, NULL);
        else
            ERROR("没有找到ID为:%lld的控件的光标事件回调函数\n", id);
    }

    // 移除字符事件列表
    if (widget->priorityEventCharMods >= 0)
    {
        list *l = &win->listEventCharMods[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityEventCharMods)];
        list *node = listSearchDataAddr(l, widget);
        if (node != NULL)
            listDeleteNode(l, node, NULL);
        else
            ERROR("没有找到ID为:%lld的控件的字符事件回调函数\n", id);
    }

    // 移除滚轮事件列表
    if (widget->priorityEventScroll >= 0)
    {
        list *l = &win->listEventScroll[GUI_CALL_PRIORITY_SAFE_GET(widget->priorityEventScroll)];
        list *node = listSearchDataAddr(l, widget);
        if (node != NULL)
            listDeleteNode(l, node, NULL);
        else
            ERROR("没有找到ID为:%lld的控件的滚轮事件回调函数\n", id);
    }

    // 调用销毁函数
    CALL(widget->destroy, win, widget);
}

void guiWindowDrawCallBack(list *group, GUIwin *win)
{
    bool over = false;
    for (int i = 0; i < GUI_CALL_PRIORITY_NUM && over == false; i++)
    {
        // 如果没有控件，直接跳过
        if (group[i].count == 0)
            continue;

        // 遍历所有控件
        list *l = &group[i];
        list *node = l->bk;
        while (node != l)
        {
            list *tmp = node;
            node = node->bk;

            GUIwidget *widget = (GUIwidget *)tmp->data;
            if (widget->callDraw(win, widget) == false)
                over = true; // 相同优先级的都需要调用
        }
    }
}

void guiWindowEventCallBack(list *group, GUIwin *win, const GUIevent *event)
{
    bool over = false;
    for (int i = 0; i < GUI_CALL_PRIORITY_NUM && over == false; i++)
    {
        // 如果没有控件，直接跳过
        if (group[i].count == 0)
            continue;

        // 遍历所有控件
        list *l = &group[i];
        list *node = l->bk;
        while (node != l)
        {
            list *tmp = node;
            node = node->bk;

            GUIwidget *widget = (GUIwidget *)tmp->data;
            if (widget->callEvent(win, widget, event) == false)
                over = true; // 相同优先级的都需要调用
        }
    }
}

void guiWindowMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);
    if (win == NULL)
        return;

    // 复制事件
    GUIevent event = {0};
    event.type = GUI_EVENT_TYPE_MOUSE_BUTTON;
    event.MouseButton.button = button;
    event.MouseButton.action = action;
    event.MouseButton.mods = mods;

    // 调用事件回调函数
    guiWindowEventCallBack(win->listEventMouseButton, win, (const GUIevent *)&event);
}

void guiWindowCursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);
    if (win == NULL)
        return;

    // 复制事件
    GUIevent event = {0};
    event.type = GUI_EVENT_TYPE_CURSOR_POS;
    event.CursorPos.xpos = xpos;
    event.CursorPos.ypos = ypos;

    // 调用事件回调函数
    guiWindowEventCallBack(win->listEventCursorPos, win, (const GUIevent *)&event);
}

void guiWindowCharModsCallback(GLFWwindow *window, unsigned int codepoint, int mods)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);
    if (win == NULL)
        return;

    // 复制事件
    GUIevent event = {0};
    event.type = GUI_EVENT_TYPE_CHAR_MODS;
    event.CharMods.codepoint = codepoint;
    event.CharMods.mods = mods;

    // 调用事件回调函数
    guiWindowEventCallBack(win->listEventCharMods, win, (const GUIevent *)&event);
}

void guiWindowScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    GUIwin *win = (GUIwin *)glfwGetWindowUserPointer(window);
    if (win == NULL)
        return;

    // 复制事件
    GUIevent event = {0};
    event.type = GUI_EVENT_TYPE_SCROLL;
    event.Scroll.xoffset = xoffset;
    event.Scroll.yoffset = yoffset;

    // 调用事件回调函数
    guiWindowEventCallBack(win->listEventScroll, win, (const GUIevent *)&event);
}

void guiWindowStart(GUIwin *win)
{
    // 设置各个回调函数
    glfwSetMouseButtonCallback(win->window, guiWindowMouseButtonCallback);
    glfwSetCursorPosCallback(win->window, guiWindowCursorPosCallback);
    glfwSetCharModsCallback(win->window, guiWindowCharModsCallback);
    glfwSetScrollCallback(win->window, guiWindowScrollCallback);

    // 帧率控制
    GUIFrame *frame = guiFrameCreate(30);

    glClearColor(0.8f, 0.95f, 0.95f, 1.0f);
    glfwShowWindow(win->window);
    while (!glfwWindowShouldClose(win->window))
    {
        glfwWaitEvents();

        // 处理任务
        if (0 == sem_trywait(&win->semTask))
        {
            pthread_mutex_lock(&win->mutexTask);
            list *node;
            while (node = listGetNodeFromStart(&win->listTask))
            {
                // 获取任务

                // 处理任务

                // 释放任务
                listDeleteNode(NULL, node, NULL);
            }
            pthread_mutex_unlock(&win->mutexTask);
        }

        // 渲染界面
        if (guiFrameCheck(frame))
        {
            // 清空颜色缓冲区
            glClear(GL_COLOR_BUFFER_BIT);

            // 调用渲染回调函数
            guiWindowDrawCallBack(win->listDraw, win);

            // 交换缓冲区
            glfwSwapBuffers(win->window);
        }
    }

    guiFrameDestroy(frame);
}

void guiWindowQuit(GUIwin *win)
{
    // 调用每一个控件的销毁函数
    list *node = win->listWidget.fd;
    while (node != &win->listWidget)
    {
        // 获取控件
        GUIwidget *widget = (GUIwidget *)node->data;
        node = node->fd;

        // 解绑窗口
        widget->win = NULL;

        // 调用销毁函数
        CALL(widget->destroy, win, widget);
    }

    // 释放所有list
    for (int i = 0; i < GUI_CALL_PRIORITY_NUM; i++)
    {
        listDeleteList(&win->listDraw[i], NULL);
        listDeleteList(&win->listEventMouseButton[i], NULL);
        listDeleteList(&win->listEventCursorPos[i], NULL);
        listDeleteList(&win->listEventCharMods[i], NULL);
        listDeleteList(&win->listEventScroll[i], NULL);
    }
    listDeleteList(&win->listWidget, NULL);
    listDeleteList(&win->listTask, NULL);

    // 释放信号量
    sem_destroy(&win->semTask);

    // 释放锁
    pthread_mutex_destroy(&win->mutexTask);
}