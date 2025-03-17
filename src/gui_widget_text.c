#include "gui_widget_text.h"

// 创建和销毁调用函数
void gui_widget_text_registerCall(GUIid id)
{

    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GUIwidgetDoctorWait));
    GUIwidgetDoctorWait *st = (GUIwidgetDoctorWait *)GUI_ID2WIDGET(id)->data1;
    memset(st, 0, sizeof(GUIwidgetDoctorWait));

    mat4 model;
    glm_translate_make(model, (vec3){0.0f, -250.0f, 0.0f});
    st->str = guiStrCreate(font_default, 96, GUI_STR_MOD_CENTER, program.font, model, (vec4){0.0f, 0.0f, 0.0f, 1.0f});
    guiStrCpy(st->str, L"文本");
}
void gui_widget_text_logoffCall(GUIid id)
{
    GUIwidgetDoctorWait *st = (GUIwidgetDoctorWait *)GUI_ID2WIDGET(id)->data1;
    guiStrDelete(st->str);
    free(st);
}

bool gui_widget_text_drawCall(GUIid id)
{
    GUIwidgetDoctorWait *st = (GUIwidgetDoctorWait *)GUI_ID2WIDGET(id)->data1;
    guiStrRender(st->str);

    return true;
}