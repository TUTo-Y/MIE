#include <math.h>
#include <string.h>
#include <locale.h>
#include <semaphore.h>

#include "gui.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"

#include "patient.h"
#include "doctor.h"

#include "RDH.h"
#include "rand.h"
#include "login.h"
#include "vector.h"
#include "config.h"
#include "resource.h"
#include "web.h"
#include "enc.h"
#include "log.h"
#include "choice_file.h"

#if 0


int main()
{
    setlocale(LC_ALL, "zh_CN.UTF-8");
    return 0;
}

#else
void guiPlay(GLFWwindow *window);

void Init()
{
    // 设置中文
    setlocale(LC_ALL, "zh_CN.UTF-8");
    DEBUG("测试glfw版本 : %s\n", glfwGetVersionString());

    // 初始化winsock
    webInit();

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
        ERR("加载配置文件失败\n");

    // 初始化资源
    resInit();

    // 加载字体
    font_default = guiTTFCreate(resGetFile(config.default_ttf_path, NULL, NULL, false), 8, 16, 32, 40, 48, 64, 96, 0);
}

void Quit()
{
    // 释放资源
    resQuit();

    // 释放glfw
    glfwTerminate();

    // 释放winsock
    webQuit();
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

    // 销毁文本框控件
    if (hEdit)
        DestroyWindow(hEdit);
    if (hEdit2)
        DestroyWindow(hEdit2);
    // 销毁字体
    if (hFont)
        DeleteObject(hFont);

    // 销毁窗口
    glfwDestroyWindow(window);

    // 退出gui
    guiQuit();

quit:
    Quit();
    return 0;
}

void guiPlay(GLFWwindow *window)
{
    // 创建窗口
    GUIid winID = guiWindowCreate(guiIDRegister(GUI_ID_WINDOW), window);

    // 初始化全局控件
    guiWidgetInit(guiIDRegister(GUI_ID_MOUSEMOVE), NULL, NULL, NULL, NULL, NULL, gui_widget_mousemove_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOG), gui_widget_log_registerCall, gui_widget_log_logoffCall, NULL, NULL, gui_widget_log_drawCall, NULL);
    guiWidgetInit(guiIDRegister(GUI_ID_WAIT), gui_widget_wait_registerCall, gui_widget_wait_logoffCall, gui_widget_wait_loadCall, gui_widget_wait_uploadCall, gui_widget_wait_drawCall, NULL);

    // 登录注册部分控件
    guiWidgetInit(guiIDRegister(GUI_ID_LOGINBACK),
                  gui_widget_loginback_registerCall, gui_widget_loginback_logoffCall,
                  gui_widget_loginback_loadCall, gui_widget_loginback_uploadCall,
                  gui_widget_loginback_drawCall, gui_widget_loginback_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGIN_USER), gui_widget_input_registerCall, gui_widget_input_logoffCall, gui_widget_input_loadCall, gui_widget_input_uploadCall, gui_widget_input_drawCall, gui_widget_input_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGIN_PASS), gui_widget_input_registerCall, gui_widget_input_logoffCall, gui_widget_input_loadCall, gui_widget_input_uploadCall, gui_widget_input_drawCall, gui_widget_input_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGIN_BUTTON1), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGIN_BUTTON2), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_LOGIN_CHOICE), gui_widget_login_choice_registerCall, gui_widget_login_choice_logoffCall, gui_widget_login_choice_loadCall, gui_widget_login_choice_uploadCall, gui_widget_login_choice_drawCall, gui_widget_login_choice_eventCall);

    // 医生和患者公用的图像列表控件
    guiWidgetInit(guiIDRegister(GUI_ID_IMG_CHOICE_LIST), gui_widget_img_choice_registerCall, gui_widget_img_choice_logoffCall, gui_widget_img_choice_loadCall, gui_widget_img_choice_uploadCall, gui_widget_img_choice_drawCall, gui_widget_img_choice_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_TEXT_PATIENT_DOCTOR), gui_widget_text_registerCall, gui_widget_text_logoffCall, NULL, NULL, gui_widget_text_drawCall, NULL);
    
    // 患者控件
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_IMG_IMG1), gui_widget_img_registerCall, gui_widget_img_logoffCall, gui_widget_img_loadCall, gui_widget_img_uploadCall, gui_widget_img_drawCall, gui_widget_img_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_IMG_IMG2), gui_widget_img_registerCall, gui_widget_img_logoffCall, gui_widget_img_loadCall, gui_widget_img_uploadCall, gui_widget_img_drawCall, gui_widget_img_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_BUTTON_CHOICE_IMG), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_BUTTON_CHOICE_TXET), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_BUTTON_IMPLANT_TXET), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_BUTTON_UPLOAD), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_TEXT_WAIT1), gui_widget_text_registerCall, gui_widget_text_logoffCall, NULL, NULL, gui_widget_text_drawCall, NULL);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_TEXT_ALLOW), gui_widget_text_registerCall, gui_widget_text_logoffCall, NULL, NULL, gui_widget_text_drawCall, NULL);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_BUTTON_ALLOW), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_BUTTON_REFUSE), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_TEXT_WAIT2), gui_widget_text_registerCall, gui_widget_text_logoffCall, NULL, NULL, gui_widget_text_drawCall, NULL);
    guiWidgetInit(guiIDRegister(GUI_ID_PATIENT_BUTTON_RESULT), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    
    // 医生控件
    guiWidgetInit(guiIDRegister(GUI_ID_DOCTOR_TEXT_WAIT1), gui_widget_text_registerCall, gui_widget_text_logoffCall, NULL, NULL, gui_widget_text_drawCall, NULL);
    guiWidgetInit(guiIDRegister(GUI_ID_DOCTOR_IMG_IMG), gui_widget_img_registerCall, gui_widget_img_logoffCall, gui_widget_img_loadCall, gui_widget_img_uploadCall, gui_widget_img_drawCall, gui_widget_img_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_DOCTOR_BUTTON_RESULT), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_DOCTOR_BUTTON_UPLOAD), gui_widget_button_registerCall, gui_widget_button_logoffCall, gui_widget_button_loadCall, gui_widget_button_uploadCall, gui_widget_button_drawCall, gui_widget_button_eventCall);
    guiWidgetInit(guiIDRegister(GUI_ID_DOCTOR_TEXT_ADVICE), gui_widget_text_registerCall, gui_widget_text_logoffCall, NULL, NULL, gui_widget_text_drawCall, NULL);
    
    
    /* 将控件添加进窗口 */

    // 窗口移动控件
    guiWidgetAddToControl((winID), GUI_ID_MOUSEMOVE,
                          GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          0, 0, true);
    // 日志控件
    guiWidgetAddToControl((winID), GUI_ID_LOG,
                          GUI_WIDGET_CALLFLAG_DRAW,
                          0, 0, true);
    // 登录窗口背景控件
    guiWidgetAddToControl((winID), GUI_ID_LOGINBACK,
                          GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          1000, 1000, true);
    // 用户名输入框
    guiWidgetAddToControl((winID), GUI_ID_LOGIN_USER,
                          GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, true);
    guiWidgetInputSetDefaultText(GUI_ID_LOGIN_USER, L"请输入用户名");
    guiWidgetInputSetPos(GUI_ID_LOGIN_USER, (vec3){0.0f, 150.0f - 80.0f, 0.0f});
    // 密码输入框
    guiWidgetAddToControl((winID), GUI_ID_LOGIN_PASS,
                          GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, true);
    guiWidgetInputSetDefaultText(GUI_ID_LOGIN_PASS, L"请输入密码");
    guiWidgetInputSetPasswd(GUI_ID_LOGIN_PASS, true);
    guiWidgetInputSetPos(GUI_ID_LOGIN_PASS, (vec3){0.0f, 0.0f - 80.0f, 0.0f});

    // 登录按钮
    guiWidgetAddToControl((winID), GUI_ID_LOGIN_BUTTON1, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, true);
    guiWidgetButtonSetText(GUI_ID_LOGIN_BUTTON1, L"登录");
    guiWidgetButtonSetPos(GUI_ID_LOGIN_BUTTON1, -250, -100 - 80.0f, 200, 50);
    guiWidgetButtonSetBackCall(GUI_ID_LOGIN_BUTTON1, loginBackCall, 0, NULL);
    // 注册按钮
    guiWidgetAddToControl((winID), GUI_ID_LOGIN_BUTTON2, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, true);
    guiWidgetButtonSetText(GUI_ID_LOGIN_BUTTON2, L"注册");
    guiWidgetButtonSetPos(GUI_ID_LOGIN_BUTTON2, 50, -100 - 80.0f, 200, 50);
    guiWidgetButtonSetBackCall(GUI_ID_LOGIN_BUTTON2, registerBackCall, 0, NULL);

    // 开始渲染
    guiWindowStart(winID);

    // 退出窗口
    guiWindowDestroy(winID, true);
}

#endif