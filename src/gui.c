#include "gui.h"

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
}