#ifndef GUI_WIDGET_IMG_CHOICE_H
#define GUI_WIDGET_IMG_CHOICE_H

#include "stb_image.h"

#include "gui_base.h"
#include "gui_type.h"
#include "gui_draw.h"
#include "gui_task.h"
#include "gui_str.h"
#include "gui_ID.h"
#include "gui.h"

#include "choice_file.h"
#include "gui_widget_log.h"
#include "gui_widget_img.h"

typedef struct
{
    GUIdrawrr img;  // 缩略图
    GLuint texture; // 缩略图纹理

    GLuint texture1; // 原图纹理
    GLuint texture2; // 嵌入图像纹理

    void *data; // 纹理原始数据
    int w, h;   // 纹理大小

    wchar_t *contant; // 图像中保存的数据

    uint8_t *img1; // 图像1
    uint8_t *img2; // 图像2
    size_t size;   // 嵌入的数据大小
    uint8_t *m;    // 图像额外数据m
    int mSize;     // 图像额外数据大小
} GuiwidgetImgChoiceData;

#define GUI_WIDGET_IMG_CHOICE_ADD 0x01   // 渲染和响应添加图标
#define GUI_WIDGET_IMG_CHOICE_WASTE 0x02 // 渲染和响应垃圾桶图标

#define GUI_WIDGET_IMG_PRESS 0x04 // 按下
typedef struct
{
    int flag;

    GUIdrawrr frame; // 边框

    GUIicon wasteIcon; // 垃圾桶图标
    GUIicon addIcon;   // 添加图标

    GLuint wasteIconTexture; // 垃圾桶图标纹理
    GLuint addIconTexture;   // 添加图标纹理

    list imgList; // 图片列表

    list *choice; // 选中的图片列表的元素
    int hover;    // 鼠标悬停的图片列表的元素

    void (*choiceFun)(GUIid id, list *node); // 选中的回调函数
} GuiwidgetImgChoice;

// 创建和销毁调用函数
void gui_widget_img_choice_registerCall(GUIid id); // 注册控件时的函数
void gui_widget_img_choice_logoffCall(GUIid id);   // 销毁控件时的函数

// 回调函数
void gui_widget_img_choice_loadCall(GUIid id);   // 添加控件时
void gui_widget_img_choice_uploadCall(GUIid id); // 移除控件时

bool gui_widget_img_choice_drawCall(GUIid id);                         // 绘制, 返回值为是否继续调用其他响应函数
bool gui_widget_img_choice_eventCall(GUIid id, const GUIevent *event); // 事件回调, 返回值为是否继续调用其他响应函数

static inline void guiWidgetImgChoiceFlag(GUIid id, int flag)
{
    ((GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1)->flag = flag;
}

static inline GuiwidgetImgChoiceData *guiWidgetImgChoiceGetData(GUIid id)
{
    return (GuiwidgetImgChoiceData *)((GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1)->choice->data;
}

void guiWidgetImgChoiceFreeFun(void *data); // 释放图像数据
void guiWidgetImgChoiceDelete(GUIid id);    // 删除图像列表

static inline void guiWidgetImgChoiceSetChoiceFun(GUIid id, void (*fun)(GUIid id, list *node))
{
    ((GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1)->choiceFun = fun;
}

#endif // GUI_WIDGET_IMG_CHOICE_H