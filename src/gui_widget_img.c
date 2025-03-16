#include "gui_widget_img.h"

// 创建和销毁调用函数
void gui_widget_img_registerCall(GUIid id)
{
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GUIwidgetimg));
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 创建
    st->img1 = guiDrawRRTCreate(100.0f, 100.0f, 50.0f, 0.015f);
    st->img2 = guiDrawRTCreate(100.0f, 100.0f);
}
void gui_widget_img_logoffCall(GUIid id)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 删除
    guiDrawRRTDestroy(st->img1);
    guiDrawRTDestroy(st->img2);

    free(st);
}

// 回调函数
void gui_widget_img_loadCall(GUIid id)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;
    st->texture = 0;
    st->data = NULL;
    st->w = st->h = 0;
    memset(&st->rect, 0, sizeof(st->rect));
}
void gui_widget_img_uploadCall(GUIid id)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 如果存在纹理则删除
    if (st->texture)
        glDeleteTextures(1, &st->texture);

    // 如果存在原始数据则删除
    if (st->data)
        free(st->data);
}

bool gui_widget_img_drawCall(GUIid id)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 渲染img1
    guiDrawRRTRender(st->img1);

    return true;
}
bool gui_widget_img_eventCall(GUIid id, const GUIevent *event)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    return true;
}

void guiWidgetImgSetTexture(GUIid id, void *data, int w, int h)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 释放原始数据
    if (st->texture)
        glDeleteTextures(1, &st->texture);
    if (st->data)
        free(st->data);

    // 创建新的纹理
    st->texture = guiTextureCreate2Gary(data, w, h);
    // st->texture = guiTextureCreate(data, w, h, 1, GL_RED);
    st->data = data;
    st->w = w;
    st->h = h;

    // 绑定新的纹理
    guiDrawRRTBindTexture(st->img1, st->texture);
    guiDrawRTBindTexture(st->img2, st->texture);
}
void guiWidgetImgSetRect(GUIid id, GUIrect rect)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;
    memcpy(&st->rect, &rect, sizeof(GUIrect));

    // 设置位置和大小
    mat4 model;
    glm_translate_make(model, (vec3){(float)rect.x + rect.width / 2.0f, (float)rect.y - rect.height / 2.0f, 0.0f});
    guiDrawRRTSetWH(st->img1, (float)rect.width, (float)rect.height);
    guiDrawRRTSetModel(st->img1, model);
}