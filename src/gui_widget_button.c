#include "gui_widget_button.h"

#define GUI_WIDGET_BUTTON_SEEP 0.18f // 动画速度

// 创建和销毁调用函数
void gui_widget_button_registerCall(GUIid id)
{
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GUIwidgetButtonData));
    GUIwidgetButtonData *data = GUI_ID2WIDGET(id)->data1;

    // 设置状态
    data->status = 0;

    // 设置位置
    data->pos = (GUIpos){0, 0};
    data->rect = (GUIrect){data->pos.x - 100.0f, data->pos.y + 25.0f, 200.0f, 50.0f};

    // 设置文本
    data->text = guiStrCreate(font_default, 40, GUI_STR_MOD_CENTER, program.font, NULL, (vec4){0.2, 0.2, 0.2, 1});
    guiStrCpy(data->text, L"Button");

    // 设置绘制矩形
    data->rr = guiDrawRRCCreate(data->rect.width, data->rect.height, 10.0f, GUI_DRAW_DEFAULT_VAGUE, (vec4){137.0 / 255.0, 207.0 / 255.0, 240.0 / 255.0, 1});
    // 绘制圆
    data->cc = guiDrawCCCreate(data->rect.width, (vec4){108.0 / 255.0, 160.0 / 255.0, 220.0 / 255.0, 1});
    guiDrawCCSetPos(data->cc, data->rect.x + data->rect.width, data->pos.y);

    // 设置回调
    data->onEnter = NULL;
}

void gui_widget_button_logoffCall(GUIid id)
{
    GUIwidgetButtonData *data = GUI_ID2WIDGET(id)->data1;

    // 释放文本
    guiStrDelete(data->text);

    // 释放绘制矩形
    guiDrawRRCDestroy(data->rr);
    
    // 释放绘制圆
    guiDrawCCDestroy(data->cc);

    free(GUI_ID2WIDGET(id)->data1);
    GUI_ID2WIDGET(id)->data1 = NULL;
}

// 回调函数
void gui_widget_button_loadCall(GUIid id)
{
}
void gui_widget_button_uploadCall(GUIid id)
{
}

bool gui_widget_button_drawCall(GUIid id)
{
    GUIwidgetButtonData *data = GUI_ID2WIDGET(id)->data1;

    // 使用模板测试需要清空值
    glClear(GL_DEPTH_BUFFER_BIT);
    glStencilMask(0xFF);

    // 设置模板测试
    glStencilFunc(GL_ALWAYS, 2, 0xFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    // 渲染矩形
    guiDrawRRCRender(data->rr);

    // 设置模板测试
    glStencilFunc(GL_EQUAL, 2, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // 渲染圆
    double time = glfwGetTime() - data->time;
    time = time > GUI_WIDGET_BUTTON_SEEP ? GUI_WIDGET_BUTTON_SEEP : time;
    if (GUI_FLAG_CHECK(data->status, GUI_WIDGET_BUTTON_STATUS_SELECT)) // 选中, 向左移
    {
        guiDrawCCSetPos(data->cc, data->rect.x + (data->rect.width * (GUI_WIDGET_BUTTON_SEEP - time) / GUI_WIDGET_BUTTON_SEEP), data->pos.y);
    }
    else // 向右移
    {
        guiDrawCCSetPos(data->cc, data->rect.x + data->rect.width * time / GUI_WIDGET_BUTTON_SEEP, data->pos.y);
    }
    guiDrawCC2Render(data->cc);

    // 渲染文本
    guiStrRender(data->text);

    // 恢复模板测试
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0x00);

    return true;
}
bool gui_widget_button_eventCall(GUIid id, const GUIevent *event)
{
    GUIwidgetButtonData *data = GUI_ID2WIDGET(id)->data1;

    bool in = event->mousePos.x >= data->rect.x && event->mousePos.x <= data->rect.x + data->rect.width &&
              event->mousePos.y <= data->rect.y && event->mousePos.y >= data->rect.y - data->rect.height;

    switch (event->type)
    {
    case GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON:
        // 在按钮上点击
        if (in == true)
        {
            if (event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT && event->MouseButton.action == GLFW_PRESS)
            {
                GUI_FLAG_SET(data->status, GUI_WIDGET_BUTTON_STATUS_ENTER);
                return false;
            }
            // 在按钮上松开
            else if (event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT && event->MouseButton.action == GLFW_RELEASE)
            {
                GUI_FLAG_UNSET(data->status, GUI_WIDGET_BUTTON_STATUS_ENTER);
                CALL(data->onEnter, id, data->flag, data->data);
                printf("button\n");

                return false;
            }
        }
        else if (event->MouseButton.action == GLFW_PRESS)
            GUI_FLAG_UNSET(data->status, GUI_WIDGET_BUTTON_STATUS_ENTER);

        break;

        // 移动
    case GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS:

        if (in == true) // 选中
        {
            if (GUI_FLAG_CHECK(data->status, GUI_WIDGET_BUTTON_STATUS_SELECT) == false) // 第一次选中
            {
                GUI_FLAG_SET(data->status, GUI_WIDGET_BUTTON_STATUS_SELECT);
                data->time = glfwGetTime();
                return false;
            }
        }
        else // 未选中
        {
            if (GUI_FLAG_CHECK(data->status, GUI_WIDGET_BUTTON_STATUS_SELECT) == true) // 第一次未选中
            {
                GUI_FLAG_UNSET(data->status, GUI_WIDGET_BUTTON_STATUS_SELECT);
                data->time = glfwGetTime();
                return false;
            }
        }

        break;
    }

    return true;
}


void guiWidgetButtonSetFont(GUIid id, GUIttf *ttf, int pixels, vec4 color)
{
    GUIwidgetButtonData *data = GUI_ID2WIDGET(id)->data1;
    GUIstr *str  = guiStrCreate(ttf, pixels, GUI_STR_MOD_CENTER2, program.font, NULL, color);
    guiStrCpy(str, data->text->strText);
    guiStrDelete(data->text);
    data->text = str;

    // 设置文本位置
    mat4 model;
    glm_translate_make(model, (vec3){data->pos.x, data->pos.y, 0});
    guiStrSetModel(data->text, model);

}

void guiWidgetButtonSetPos(GUIid id, int x, int y, int width, int height)
{
    GUIwidgetButtonData *data = GUI_ID2WIDGET(id)->data1;
    data->rect.x = x;
    data->rect.y = y;
    data->rect.width = width;
    data->rect.height = height;
    data->pos.x = x + width / 2;
    data->pos.y = y - height / 2;

    // 设置矩形位置
    mat4 model;
    glm_translate_make(model, (vec3){data->pos.x, data->pos.y, 0});
    guiDrawRRCSetModel(data->rr, model);
    guiDrawRRCSetWH(data->rr, width, height);

    // 设置字体位置
    guiStrSetModel(data->text, model);
}