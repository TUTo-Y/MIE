#include "gui.h"
#include "resource.h"
#include "config.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"

#include "rand.h"
#include "RDH.h"
#include "vector.h"
#include <math.h>
#include <string.h>
#include <locale.h>
#include <semaphore.h>

#include <stdio.h>
#if 0
int main()
{
    int w, h;
    unsigned char *data = stbi_load("anime-girl-3840x2160-cake-cafe-25376.jpeg", &w, &h, NULL, 3);
    printf("w = %d, h = %d, n = %d\n", w, h);
    // 将data解析成data1, data2, data3
    unsigned char *data1 = (unsigned char *)malloc(w * h);
    unsigned char *data2 = (unsigned char *)malloc(w * h);
    unsigned char *data3 = (unsigned char *)malloc(w * h);
    for (int i = 0; i < w * h; i++)
    {
        data1[i] = data[i * 3];
        data2[i] = data[i * 3 + 1];
        data3[i] = data[i * 3 + 2];
    }

    // 保存data1, data2, data3
    stbi_write_png("r.png", w, h, 1, data1, w);
    stbi_write_png("g.png", w, h, 1, data2, w);
    stbi_write_png("b.png", w, h, 1, data3, w);

    // 分别嵌入数据到data1, data2, data3
    unsigned char *datac[3] = {data1, data2, data3};
    unsigned char *d = malloc(w * h * 3);
    for (int i = 0; i < 3; i++)
    {
        // 生成随机数
        for (int i = 0; i < w * h * 3; i++)
            d[i] = rand() % 256;

        // 分割图像
        unsigned char *img1, *img2;
        rdhSplitImage(datac[i], w * h, &img1, &img2);

        // 嵌入数据
        unsigned char *m = NULL;
        int mSize = 0;
        rdhEmbedData(img1, img2, w, h, &m, &mSize, d, w * h * 3);

        // 合并图像
        rdhCombineImage(img1, img2, w * h, &datac[i]);
    }
    free(d);
    
    stbi_write_png("r1.png", w, h, 1, data1, w);
    stbi_write_png("g1.png", w, h, 1, data2, w);
    stbi_write_png("b1.png", w, h, 1, data3, w);

    // 合并data1, data2, data3
    for (int i = 0; i < w * h; i++)
    {
        data[i * 3] = data1[i];
        data[i * 3 + 1] = data2[i];
        data[i * 3 + 2] = data3[i];
    }

    // 保存合并后的图像
    stbi_write_png("out.png", w, h, 3, data, w * 3);

    // 释放
    free(data1);
    free(data2);
    free(data3);
    free(data);
    return 0;
}
#endif
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
    glfwWindowHint(GLFW_SAMPLES, 4);
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
    resInit();

    // 加载字体
    font_default = guiTTFCreate(resGetFile(config.default_ttf_path, NULL, NULL, false), 8, 16, 32, 40, 48, 64, 96, 0);
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

    // 初始化gui
    if (guiInit() == false)
        goto quit;

    // 渲染窗口
    guiPlay(window);

    // 销毁窗口
    glfwDestroyWindow(window);

    guiQuit();

quit:
    Quit();
    return 0;
}

void guiPlay(GLFWwindow *window)
{
    // 初始化控件
    guiWidgetInit(guiIDRegister(GUI_ID_MOUSEMOVE), NULL, NULL, NULL, NULL, NULL, gui_widget_mousemove_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOG), gui_widget_log_registerCall, gui_widget_log_logoffCall, NULL, NULL, gui_widget_log_drawCall, NULL);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGINBACK),
                  gui_widget_loginback_registerCall, gui_widget_loginback_logoffCall,
                  gui_widget_loginback_loadCall, gui_widget_loginback_uploadCall,
                  gui_widget_loginback_drawCall, gui_widget_loginback_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGIN_USER), gui_widget_input_registerCall, gui_widget_input_logoffCall, gui_widget_input_loadCall, gui_widget_input_uploadCall, gui_widget_input_drawCall, gui_widget_input_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGIN_PASS), gui_widget_input_registerCall, gui_widget_input_logoffCall, gui_widget_input_loadCall, gui_widget_input_uploadCall, gui_widget_input_drawCall, gui_widget_input_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGIN_BUTTON1), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGIN_BUTTON2), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);

    // 创建窗口
    GUIid winID = guiWindowCreate(guiIDRegister(GUI_ID_WINDOW), window);

    // 将控件添加进窗口
    // 窗口移动控件
    guiWidgetToControl((winID), GUI_ID_MOUSEMOVE,
                       GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       0, 0, true);
    // 日志控件
    guiWidgetToControl((winID), GUI_ID_LOG,
                       GUI_WIDGET_CALLFLAG_DRAW,
                       0, 0, true);
    // 登录窗口背景控件
    guiWidgetToControl((winID), GUI_ID_LOGINBACK,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       1000, 1000, true);
    // 用户名输入框
    guiWidgetToControl((winID), GUI_ID_LOGIN_USER,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);
    guiWidgetInputSetDefaultText(GUI_ID_LOGIN_USER, L"请输入用户名");
    guiWidgetInputSetPos(GUI_ID_LOGIN_USER, (vec3){0.0f, 150.0f, 0.0f});
    // 密码输入框
    guiWidgetToControl((winID), GUI_ID_LOGIN_PASS,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);
    guiWidgetInputSetDefaultText(GUI_ID_LOGIN_PASS, L"请输入密码");
    guiWidgetInputSetPasswd(GUI_ID_LOGIN_PASS, true);
    guiWidgetInputSetPos(GUI_ID_LOGIN_PASS, (vec3){0.0f, 0.0f, 0.0f});

    // 登录按钮
    guiWidgetToControl((winID), GUI_ID_LOGIN_BUTTON1, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);
    guiWidgetButtonSetText(GUI_ID_LOGIN_BUTTON1, L"登录");
    guiWidgetButtonSetPos(GUI_ID_LOGIN_BUTTON1, -250, -100, 200, 50);
    // 注册按钮
    guiWidgetToControl((winID), GUI_ID_LOGIN_BUTTON2, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);
    guiWidgetButtonSetText(GUI_ID_LOGIN_BUTTON2, L"注册");
    guiWidgetButtonSetPos(GUI_ID_LOGIN_BUTTON2, 50, -100, 200, 50);

    // 开始渲染
    guiWindowStart(winID);

    // 退出窗口
    guiWindowDestroy(winID, true);
}

#endif