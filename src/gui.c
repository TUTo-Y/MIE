#include "gui.h"

HWND hEdit = NULL; // 编辑框句柄
HWND hEdit2 = NULL; // 编辑框句柄
HFONT hFont = NULL; // 字体句柄

vec4 default_color = {0.9f, 0.9f, 0.95f, 1.0f};

bool guiInit()
{
    // 设置默认视图
    GUI_SET_DEFAULT_VIEWPORT();
    
    // 加载基础着色器
    if (guiProgramInit() == false)
        return false;

    // 初始化圆角纹理矩形
    guiDrawTInit();
    
    // 初始化圆角颜色矩形
    guiDrawCInit();

    // 控件初始化
    guiIDInit();

    return true;
}

void guiQuit()
{
    // 退出圆角纹理矩形
    guiDrawTQuit();

    // 退出圆角颜色矩形
    guiDrawCQuit();

    // 销毁基础着色器
    guiProgramDestroy();

    // 控件销毁
    guiIDQuit();
}

void guiSetDefaultClearColor(float r, float g, float b, float a)
{
    default_color[0] = r;
    default_color[1] = g;
    default_color[2] = b;
    default_color[3] = a;
}