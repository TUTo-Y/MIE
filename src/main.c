#include "gui.h"
#include "resource.h"
#include "config.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"

#include "rand.h"
#include "RDH.h"

#include <math.h>
#include <string.h>
#include <locale.h>
#include <semaphore.h>
#if 1

void Init()
{
    // 设置中文
    setlocale(LC_ALL, "zh_CN.UTF-8");

    // 初始化glfw
    glfwInit();
    glfwGetError(NULL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);   // 隐藏窗口
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // 禁用窗口大小调节
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // 隐藏窗口边框

    // 为了方便opengl读取，设置图像翻转
    stbi_set_flip_vertically_on_load(true);
    stbi_flip_vertically_on_write(true);

    // 初始化随机数
    srand((unsigned int)time(NULL));

    // 初始化资源
    resInit("resource");

    // 设置PV
    guiSetPV(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Quit()
{
    /* 释放资源 */
    resQuit();

    /* 释放glfw */
    glfwTerminate();
}

int main()
{
    DEBUG("%s\n", glfwGetVersionString());
    Init();

    // 创建窗口
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MIE - 医学图像加密测试系统", NULL, NULL);
    if (NULL == window)
        goto quit;
    int xpos = (mode->width - WINDOW_WIDTH) / 2;
    int ypos = (mode->height - WINDOW_HEIGHT) / 2;

    // 设置窗口位置到屏幕中心
    glfwSetWindowPos(window, xpos, ypos);
    glfwMakeContextCurrent(window);

    // 初始化glad
    if (0 == gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        goto quit;
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // 创建控件
    GUIwidget widget;
    guiWidgetInit(&widget,
                  0,
                  GUI_WIDGET_ID_MOUSEMOVE,
                  NULL, NULL, NULL, NULL,
                  &guiWidgetMouseMoveEvent, -1, GUI_CALL_PRIORITY_4, GUI_CALL_PRIORITY_4, -1, -1, NULL, NULL, NULL, NULL);
    
    // 创建窗口
    GUIwin win;
    guiWindowInit(&win, window);
    guiWindowAddWidget(&win, GUI_WIDGET_ID_MOUSEMOVE);
    guiWindowStart(&win);
    guiWindowQuit(&win);

    glfwDestroyWindow(window);

quit:
    Quit();
    return 0;
}

#endif