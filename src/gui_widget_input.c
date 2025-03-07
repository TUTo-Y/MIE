#include "gui_widget_input.h"

#define ANIM1_TIME 0.15
#define ANIM2_TIME 0.3

// 创建和销毁调用函数
void gui_widget_input_registerCall(GUIid id)
{
    // 创建缓冲区
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GUIwidgetInputData));
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;

    data1->status = 0;

    // 初始化文本内容
    data1->textMax = 0x10;
    data1->textRender = guiStrCreate(font_default, 64, GUI_STR_MOD_CENTER, program.font, NULL, (vec4){1, 1, 1, 1});
    data1->textDefault = guiStrCreate(font_default, 64, GUI_STR_MOD_CENTER, program.font, NULL, (vec4){1, 1, 1, 0.6});
    guiStrCpy(data1->textDefault, L"输入内容");
    data1->textplain = (wchar_t *)malloc(sizeof(wchar_t) * (data1->textMax + 1));
    data1->textplain[0] = L'\0';

    // 设置文本位置
    mat4 model;
    glm_vec3_copy((vec3){0.0, 0.0, 0.0}, data1->pos);
    glm_translate_make(model, data1->pos);
    guiStrSetModel(data1->textRender, model);
    guiStrSetModel(data1->textDefault, model);

    // 创建背景线
    data1->lineSize = 500;
    data1->lineBack = guiDrawRRCCreate(data1->lineSize, 3, 1, 0.02, (vec4){0.8f, 0.8f, 0.8f, 9.0f});
    data1->linePre = guiDrawRRCCreate(data1->lineSize, 3, 1, 0.02, (vec4){0x84 / 255.0, 0xC9 / 255.0, 0xFF / 255.0, 0.0});
    glm_translate_make(model, (vec3){0.0, data1->textRender->font->descent - data1->textRender->font->lineGap, 0.0});
    guiDrawRRCSetModel(data1->lineBack, model);
    guiDrawRRCSetModel(data1->linePre, model);

    // 更新检查矩形
    guiWidgetInputUpdateRect(data1);
}

void gui_widget_input_logoffCall(GUIid id)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;

    // 删除背景线
    guiDrawRRCDestroy(data1->lineBack);
    guiDrawRRCDestroy(data1->linePre);

    // 删除文本内容
    guiStrDelete(data1->textRender);
    guiStrDelete(data1->textDefault);
    free(data1->textplain);

    // 释放缓冲区
    free(GUI_ID2WIDGET(id)->data1);
}

// 回调函数
void gui_widget_input_loadCall(GUIid id)
{
}
void gui_widget_input_uploadCall(GUIid id)
{
}

bool gui_widget_input_drawCall(GUIid id)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;

    // 处理动画
    if (GUI_FLAG_CHECK(data1->status, GUI_WIDGET_INPUT_ANIM1)) // 动画1
    {
        double time = glfwGetTime() - data1->linePreTime;
        if (time > ANIM1_TIME)
        {
            GUI_FLAG_UNSET(data1->status, GUI_WIDGET_INPUT_ANIM1);
            time = ANIM1_TIME;
        }
        // 增加长度
        guiDrawRRCSetWH(data1->linePre, data1->lineSize * time / ANIM1_TIME, 3);
    }
    else if (GUI_FLAG_CHECK(data1->status, GUI_WIDGET_INPUT_ANIM2)) // 动画2
    {
        double time = glfwGetTime() - data1->linePreTime;
        if (time > ANIM2_TIME)
        {
            GUI_FLAG_UNSET(data1->status, GUI_WIDGET_INPUT_ANIM2);
            time = ANIM2_TIME;
        }
        data1->linePre->color.color[3] = 1.0 - time / ANIM2_TIME;
    }

    // 渲染文字
    guiStrRender(data1->textplain[0] || GUI_FLAG_CHECK(data1->status, GUI_WIDGET_INPUT_FOCUS) ? data1->textRender : data1->textDefault);

    // 渲染背景线
    guiDrawRRCRender(data1->lineBack);

    // 渲染前景线
    guiDrawRRCRender(data1->linePre);

    return true;
}

bool gui_widget_input_eventCall(GUIid id, const GUIevent *event)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    bool in;

    // 检查事件是否在控件内
    double xpos, ypos;
    xpos = event->mousePos.x;
    ypos = event->mousePos.y;
    in = (xpos >= data1->rect.x) && (xpos <= data1->rect.x + data1->rect.width) && (ypos <= data1->rect.y) && (ypos >= data1->rect.y - data1->rect.height);

    switch (event->type)
    {
        // 鼠标事件
    case GUI_EVENT_TYPE_MOUSE_BUTTON:
        if (event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT)
        {
            // 在控件内按下，并且没有焦点
            if (event->MouseButton.action == GLFW_PRESS &&
                in &&
                !GUI_FLAG_CHECK(data1->status, GUI_WIDGET_INPUT_FOCUS))
            {
                // 设置焦点和动画1
                GUI_FLAG_SET(data1->status, GUI_WIDGET_INPUT_FOCUS | GUI_WIDGET_INPUT_ANIM1);
                // 取消动画2
                GUI_FLAG_UNSET(data1->status, GUI_WIDGET_INPUT_ANIM2);

                // 设置颜色和位置
                guiDrawRRCSetWH(data1->linePre, 0, 3);
                data1->linePre->color.color[3] = 1.0;

                // 更新时间
                data1->linePreTime = glfwGetTime();
            }

            // 按钮外按下，并且有焦点
            else if (event->MouseButton.action == GLFW_PRESS &&
                     !in &&
                     GUI_FLAG_CHECK(data1->status, GUI_WIDGET_INPUT_FOCUS))
            {
                // 设置动画2
                GUI_FLAG_SET(data1->status, GUI_WIDGET_INPUT_ANIM2);
                // 取消焦点和动画1
                GUI_FLAG_UNSET(data1->status, GUI_WIDGET_INPUT_FOCUS | GUI_WIDGET_INPUT_ANIM1);

                // 设置颜色和位置
                guiDrawRRCSetWH(data1->linePre, data1->lineSize, 3);
                data1->linePre->color.color[3] = 0.0;

                // 更新时间
                data1->linePreTime = glfwGetTime();
            }

        }
        break;

        // 光标事件
    case GUI_EVENT_TYPE_CURSOR_POS:
        break;

        // 字符事件
    case GUI_EVENT_TYPE_CHAR_MODS:
        if (GUI_FLAG_CHECK(data1->status, GUI_WIDGET_INPUT_FOCUS)) // 需要有焦点
        {
            wchar_t c = event->CharMods.codepoint;

            // 检查输入的大小是否超过限制
            if (wcslen(data1->textplain) + 1 < data1->textMax)
            {
                // 添加文本
                wcsncat(data1->textplain, &c, 1);
                if (data1->status & GUI_WIDGET_INPUT_PASSWD)
                    c = L'*';
                guiStrCatC(data1->textRender, c);
            }
        }
        break;

        // 键盘事件
    case GUI_EVENT_TYPE_KEY:
        if (GUI_FLAG_CHECK(data1->status, GUI_WIDGET_INPUT_FOCUS)) // 需要有焦点
        {
            if (event->Key.key == GLFW_KEY_BACKSPACE &&                                  // 删除键
                (event->Key.action == GLFW_PRESS || event->Key.action == GLFW_REPEAT) && // 按下和按住都可以
                wcslen(data1->textplain) > 0)                                            // 有文本
            {
                // 删除文本
                guiStrBack(data1->textRender);
                data1->textplain[wcslen(data1->textplain) - 1] = L'\0';
            }
        }
        break;

    default:
        break;
    }

    return true;
}

void guiWidgetInputSetPasswd(GUIid id, bool passwd)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    if (passwd)
    {
        GUI_FLAG_SET(data1->status, GUI_WIDGET_INPUT_PASSWD);

        // 将text的内容替换为*
        wchar_t *t = malloc(sizeof(wchar_t) * (wcslen(data1->textplain) + 1));
        for (size_t i = 0; i < wcslen(data1->textplain); i++)
            t[i] = L'*';
        t[wcslen(data1->textplain)] = L'\0';
        guiStrCpy(data1->textRender, t);
        free(t);
    }
    else
    {
        GUI_FLAG_UNSET(data1->status, GUI_WIDGET_INPUT_PASSWD);

        // 将text的内容替换为明文
        guiStrCpy(data1->textRender, data1->textplain);
    }
}

void guiWidgetInputSetPos(GUIid id, vec3 pos)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    glm_vec3_copy(pos, data1->pos);

    mat4 model;
    glm_translate_make(model, data1->pos);

    // 修改文本位置
    guiStrSetModel(data1->textRender, model);
    guiStrSetModel(data1->textDefault, model);

    // 修改背景线位置
    glm_translate(model,
                  (vec3){0.0,
                         data1->textRender->font->descent - data1->textRender->font->lineGap,
                         0.0});
    guiDrawRRCSetModel(data1->lineBack, model);
    guiDrawRRCSetModel(data1->linePre, model);

    // 更新检查矩形
    guiWidgetInputUpdateRect(data1);
}

void guiWidgetInputSetLineSize(GUIid id, size_t size)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    data1->lineSize = size;
    guiDrawRRCSetWH(data1->lineBack, size, 3);
    guiDrawRRCSetWH(data1->linePre, size, 3);

    // 更新检查矩形
    guiWidgetInputUpdateRect(data1);
}

void guiWidgetInputSetTextFont(GUIid id, GUIttf *ttf, int pixels)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    vec4 color;
    glm_vec4_copy(data1->textRender->color, color);
    // 销毁原有字体
    guiStrDelete(data1->textRender);
    // 创建新的字体
    data1->textRender = guiStrCreate(ttf, pixels, GUI_STR_MOD_CENTER, program.font, NULL, color);
    guiStrCpy(data1->textRender, data1->textplain);
    // 更新位置
    guiWidgetInputSetPos(id, data1->pos);
}

void guiWidgetInputSetColor(GUIid id, vec4 back, vec4 pre)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    guiDrawRRCSetColor(data1->lineBack, back);
    pre[3] = data1->linePre->color.color[3]; // 保持透明度不变
    guiDrawRRCSetColor(data1->linePre, pre);
}

void guiWidgetInputSetMax(GUIid id, size_t size)
{
    GUIwidgetInputData *data1 = (GUIwidgetInputData *)GUI_ID2WIDGET(id)->data1;
    data1->textMax = size;

    free(data1->textplain);
    data1->textplain = (wchar_t *)malloc(sizeof(wchar_t) * (data1->textMax + 1));
    data1->textplain[0] = L'\0';
    guiStrCpy(data1->textRender, L"");
}