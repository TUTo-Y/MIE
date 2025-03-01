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

void guiPlay(GLFWwindow *window);

void Init()
{
    // 设置中文
    setlocale(LC_ALL, "zh_CN.UTF-8");
    DEBUG("测试glfw版本 : %s\n", glfwGetVersionString());

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

    // 初始化配置
    if (confInit("config") == false)
        ERROR("加载配置文件失败\n");

    // 初始化资源
    resInit(config);
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

    // 加载基础着色器
    if (guiProgramInit() == false)
        goto quit;

    // 渲染窗口
    guiPlay(window);

    // 销毁基础着色器
    guiProgramDestroy();

    // 销毁窗口
    glfwDestroyWindow(window);

quit:
    Quit();
    return 0;
}

#endif

void guiPlay(GLFWwindow *window)
{
    // 创建窗口移动控件
    GUIwidget WidgetWindowMove;
    guiWidgetInit(&WidgetWindowMove,
                  0,
                  GUI_WIDGET_ID_MOUSEMOVE,
                  NULL, NULL, NULL, NULL,
                  &guiWidgetMouseMoveEvent, -1, GUI_CALL_PRIORITY_2, GUI_CALL_PRIORITY_2, -1, -1, NULL, NULL, NULL, NULL);

    // 初始化登录背景控件
    GUIwidget WidgetLoginBack;
    guiWidgetInit(&WidgetLoginBack, 0, GUI_WIDGET_ID_LOGIN_BACK,
                  gui_widget_loginback_init, gui_widget_loginback_destroy, gui_widget_loginback_msg,
                  gui_widget_loginback_callDraw, gui_widget_loginback_callEvent,
                  GUI_CALL_PRIORITY_0, -1, GUI_CALL_PRIORITY_2, -1, -1, NULL, NULL, gui_widget_loginback_StartCall, gui_widget_loginback_DestroyCall);

    // 创建窗口
    GUIwin win;
    guiWindowInit(&win, window);

    // 添加控件
    guiWindowAddWidget(&win, GUI_WIDGET_ID_MOUSEMOVE);
    guiWindowAddWidget(&win, GUI_WIDGET_ID_LOGIN_BACK);

    // 运行窗口
    guiWindowStart(&win);
    guiWindowQuit(&win);

    // 释放控件
    guiWidgetDestroy(&WidgetWindowMove);
}