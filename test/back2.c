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

    // 禁用窗口大小调节
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 设置窗口提示，隐藏窗口边框
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // 设置图像翻转
    stbi_set_flip_vertically_on_load(true);
    stbi_flip_vertically_on_write(true);

    // 初始化资源
    resInit("resource");

    // 设置PV
    guiSetPV(WINDOW_WIDTH, WINDOW_HEIGHT);

    // 初始化随机数
    srand((unsigned int)time(NULL));
}

void Quit()
{
    /* 释放资源 */
    resQuit();

    /* 释放glfw */
    glfwTerminate();

}

void charcallback(GLFWwindow *window, unsigned int codepoint)
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool move = false;
double xpos_old, ypos_old;

void mouseposcallback(GLFWwindow *window, double xpos, double ypos)
{
    if (move)
    {
        int win_x, win_y;
        glfwGetWindowPos(window, &win_x, &win_y);

        double dx = xpos - xpos_old;
        double dy = ypos - ypos_old;

        glfwSetWindowPos(window, win_x + (int)dx, win_y + (int)dy);
    }
}

void mouseentercallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            move = true;
            glfwGetCursorPos(window, &xpos_old, &ypos_old);
        }
        else if (action == GLFW_RELEASE)
        {
            move = false;
        }
    }
}

int main()
{
    Init();

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MIE - 医学图像加密测试系统", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }

    // 获取主显示器的尺寸
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int xpos = (mode->width - WINDOW_WIDTH) / 2;
    int ypos = (mode->height - WINDOW_HEIGHT) / 2;

    // 设置窗口位置到屏幕中心
    glfwSetWindowPos(window, xpos, ypos);
    glfwMakeContextCurrent(window);

    /* 初始化glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetCharCallback(window, charcallback);
    glfwSetCursorPosCallback(window, mouseposcallback);
    glfwSetMouseButtonCallback(window, mouseentercallback);

    // 创建着色器
    GLuint program = guiShaderCreateProgram(resGetFile("font.vert", NULL, 0, true), resGetFile("font.frag", NULL, 0, true), NULL);

    // 创建字体
    GUIttf *ttf = guiTTFCreate(resGetFile("Y-B008YeZiGongChangDanDanHei-2.ttf", NULL, 0, true), 0);

    // 创建字符串
    GUIstr *s1 = guiStrCreate(ttf, 64, GUI_STR_MOD_CENTER, program, NULL, (vec4){1.0f, 0.0f, 0.0f, 1.0f});
    guiStrCpy(s1, L"创建神话");

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_scale(model, (vec3){0.4f, 0.4f, 1.0f});
    guiStrSetModel(s1, model);

    // sem_timedwait

    GUIframe frame;
    guiFrameInit(&frame, 20);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // 事件处理
        // glfwPollEvents();
        glfwWaitEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        // 渲染字体
        guiStrRender(s1);

        // 控制帧率为30Hz
        // guiFrameControl(&frame);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    Quit();
    return 0;
}

#endif