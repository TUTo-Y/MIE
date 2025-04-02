#include "gui_widget_img.h"

// 创建和销毁调用函数
void gui_widget_img_registerCall(GUIid id)
{
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GUIwidgetimg));
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 创建
    st->img = guiDrawRTCreate(100.0f, 100.0f);

}
void gui_widget_img_logoffCall(GUIid id)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 删除
    guiDrawRTDestroy(st->img);

    free(st);
}

// 回调函数
void gui_widget_img_loadCall(GUIid id)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;
    st->texture = 0;
    st->data = NULL;
    st->w = st->h = 0;
    st->isTexture = false;
    memset(&st->rect, 0, sizeof(st->rect));

    guiWidgetImgSetEmpty(id); // 设置空纹理
}
void gui_widget_img_uploadCall(GUIid id)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 如果存在纹理则删除
    if (st->texture)
        glDeleteTextures(1, &st->texture);
}

bool gui_widget_img_drawCall(GUIid id)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 渲染
    guiDrawRTRender(st->img);
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
    if (st->texture && st->isTexture)
        glDeleteTextures(1, &st->texture);

    // 创建新的纹理
    st->texture = guiTextureCreate2Gary(data, w, h);
    st->data = data;
    st->w = w;
    st->h = h;

    // 绑定新的纹理
    guiDrawRTBindTexture(st->img, st->texture);

    st->isTexture = true;
}

void guiWidgetImgSetTexture2(GUIid id, GLuint texture, int w, int h)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;

    // 释放原始数据
    if (st->texture && st->isTexture)
        glDeleteTextures(1, &st->texture);

    // 创建新的纹理
    st->texture = texture;
    st->data = NULL;
    st->w = w;
    st->h = h;

    // 绑定新的纹理
    guiDrawRTBindTexture(st->img, st->texture);

    st->isTexture = false;
}

void guiWidgetImgSetRect(GUIid id, GUIrect rect)
{
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(id)->data1;
    memcpy(&st->rect, &rect, sizeof(GUIrect));

    // 设置位置和大小
    mat4 model;
    glm_translate_make(model, (vec3){(float)rect.x + rect.width / 2.0f, (float)rect.y - rect.height / 2.0f, 0.0f});
    guiDrawRTSetWH(st->img, (float)rect.width, (float)rect.height);
    guiDrawRTSetModel(st->img, model);
}