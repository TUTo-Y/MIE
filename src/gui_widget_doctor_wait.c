#include "gui_widget_doctor_wait.h"

// 创建和销毁调用函数
void gui_widget_doctor_wait_registerCall(GUIid id)
{
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GUIwidgetDoctorWait));
    GUIwidgetDoctorWait *st = (GUIwidgetDoctorWait *)GUI_ID2WIDGET(id)->data1;
    memset(st, 0, sizeof(GUIwidgetDoctorWait));

    mat4 model;
    glm_translate_make(model, (vec3){0.0f, -250.0f, 0.0f});
    st->str = guiStrCreate(font_default, 96, GUI_STR_MOD_CENTER, program.font, model, (vec4){0.0f, 0.0f, 0.0f, 1.0f});
    guiStrCpy(st->str, L"等待患者上传文件中...");
}
void gui_widget_doctor_wait_logoffCall(GUIid id)
{
    GUIwidgetDoctorWait *st = (GUIwidgetDoctorWait *)GUI_ID2WIDGET(id)->data1;
    guiStrDelete(st->str);
    free(st);
}

// 回调函数
void gui_widget_doctor_wait_loadCall(GUIid id)
{
    // 启用等待控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, true);
}
void gui_widget_doctor_wait_uploadCall(GUIid id)
{
    // 删除等待控件
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT);
}

bool gui_widget_doctor_wait_drawCall(GUIid id)
{
    GUIwidgetDoctorWait *st = (GUIwidgetDoctorWait *)GUI_ID2WIDGET(id)->data1;
    guiStrRender(st->str);

    return true;
}