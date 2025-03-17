#include "gui_widget_patient_bigchoice.h"

// 创建和销毁调用函数
void gui_widget_patient_bigchoice_registerCall(GUIid id)
{
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(gui_widget_patient_bigchoice));
    gui_widget_patient_bigchoice *st = (gui_widget_patient_bigchoice *)GUI_ID2WIDGET(id)->data1;
    memset(st, 0, sizeof(gui_widget_patient_bigchoice));

    // 初始化press
    st->press = 0;

    // 初始化背景圆矩形，使用渐变颜色
    st->back = guiDrawRRCCreate(600.0f, 600.0f, 50, 0.015f, (vec4){1.0f, 1.0f, 1.0f, 1.0f});

    // 初始化加号的两个矩形
    st->t1 = guiDrawRRCCreate(200.0f, 50.0f, 25.0f, 0.02f, (vec4){0.4f, 0.4f, 0.4f, 1.0f});
    st->t2 = guiDrawRRCCreate(50.0f, 200.0f, 25.0f, 0.02f, (vec4){0.4f, 0.4f, 0.4f, 1.0f});
}
void gui_widget_patient_bigchoice_logoffCall(GUIid id)
{
    gui_widget_patient_bigchoice *st = (gui_widget_patient_bigchoice *)GUI_ID2WIDGET(id)->data1;

    // 销毁背景圆矩形
    guiDrawRRCDestroy(st->back);

    // 销毁加号的两个矩形
    guiDrawRRCDestroy(st->t1);
    guiDrawRRCDestroy(st->t2);

    free(st);
}

bool gui_widget_patient_bigchoice_drawCall(GUIid id)
{
    gui_widget_patient_bigchoice *st = (gui_widget_patient_bigchoice *)GUI_ID2WIDGET(id)->data1;

    // 渲染背景
    guiDrawRRCRender(st->back);

    // 渲染加号的两个矩形
    guiDrawRRCRender(st->t1);
    guiDrawRRCRender(st->t2);

    return true;
}

bool gui_widget_patient_bigchoice_eventCall(GUIid id, const GUIevent *event)
{
    gui_widget_patient_bigchoice *st = (gui_widget_patient_bigchoice *)GUI_ID2WIDGET(id)->data1;

    int x = -300, w = 600;
    int y = 300, h = 600;
    bool in = event->mousePos.x >= x && event->mousePos.x <= x + w &&
              event->mousePos.y <= y && event->mousePos.y >= y - h;

    switch (event->type)
    {
    case GUI_EVENT_TYPE_CURSOR_POS:
        return st->press != true;

    case GUI_EVENT_TYPE_MOUSE_BUTTON:
        // 按钮内左键操作
        if (event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT)
        {
            // 按钮内按下
            if (event->MouseButton.action == GLFW_PRESS && in)
            {
                st->press = true;
            }
            // 按钮内谈起
            else if (event->MouseButton.action == GLFW_RELEASE && in)
            {
                st->press = false;

                // 选择图片
                // char *img_str = choiceImageFile();
                // if (!img_str)
                // {
                //     guiWidgetLogAddMsg(GUI_ID_LOG, L"未选择图片", GUI_WIDGET_LOG_WARN);
                //     break;
                // }

                // // 读取图片
                // int width, height, channels;
                // unsigned char *data = stbi_load(img_str, &width, &height, &channels, 1);
                // if (!data)
                // {
                //     guiWidgetLogAddMsg(GUI_ID_LOG, L"加载图片失败", GUI_WIDGET_LOG_ERROR);
                //     free(img_str);
                //     break;
                // }

                // // 选择图片
                // guiTaskAddTask(patientChoiceImage, ((size_t)width) << 32 | height, data);

                // // 释放内存
                // free(img_str);

                // 选择图片
                patientButtonChoiceBackcall(id, 0, NULL);
                
                // 检查如果选择了图片则进入下一步
                if (((GUIwidgetimg *)GUI_ID2WIDGET(GUI_ID_PATIENT_IMG)->data1)->w != 0)
                    guiTaskAddTask(patientChoiceImage, 0, NULL);
            }
            // 按钮外谈起
            else if (event->MouseButton.action == GLFW_RELEASE)
            {
                st->press = false;
            }
        }

        break;
    }

    return true;
}
