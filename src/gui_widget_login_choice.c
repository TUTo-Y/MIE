#include "gui_widget_login_choice.h"

#define DOCTOR 0  // 医生
#define PATIENT 1 // 患者

static bool choiceBackCall(size_t flag, void *data);

void gui_widget_login_choice_registerCall(GUIid id)
{
    // 初始化结构体
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GUIwidgetLoginChoice));
    memset(GUI_ID2WIDGET(id)->data1, 0, sizeof(GUIwidgetLoginChoice));
    GUIwidgetLoginChoice *st = (GUIwidgetLoginChoice *)GUI_ID2WIDGET(id)->data1;

    // 加载图标
    int width, height, nrChannels;
    unsigned char *doctor = stbi_load(config.login_choice_doctor_path, &width, &height, &nrChannels, 4);
    unsigned char *patient = stbi_load(config.login_choice_patient_path, &width, &height, &nrChannels, 4);
    st->iconTex[DOCTOR] = guiTextureCreate(doctor, width, height, 4, GL_RGBA);
    st->iconTex[PATIENT] = guiTextureCreate(patient, width, height, 4, GL_RGBA);
    st->icon[DOCTOR] = guiDrawIconCreate(width, height, st->iconTex[DOCTOR], (vec4){0.8f, 0.8f, 0.8f, 1.0f});
    st->icon[PATIENT] = guiDrawIconCreate(width, height, st->iconTex[PATIENT], (vec4){0.8f, 0.8f, 0.8f, 1.0f});
    stbi_image_free(doctor);
    stbi_image_free(patient);

    // 加载背景
    st->back[DOCTOR] = guiDrawRRCCreate(width, height, 30.0f, 0.08f, (vec4){0.8f, 0.8f, 0.8f, 0.0f});
    st->back[PATIENT] = guiDrawRRCCreate(width, height, 30.0f, 0.08f, (vec4){0.8f, 0.8f, 0.8f, 0.0f});

    // 加载文本
    st->str = guiStrCreate(font_default, 96, GUI_STR_MOD_CENTER2, program.font, NULL, (vec4){0.3f, 0.3f, 0.3f, 1.0f});
    guiStrCpy(st->str, L"请选择注册身份");

    // 加载圆形背景
    st->cc = guiDrawCCCreate(0.0f, (vec4){0x1E / 255.0, 0x90 / 255.0, 0xFF / 255.0, 0.3f});
}
void gui_widget_login_choice_logoffCall(GUIid id)
{
    GUIwidgetLoginChoice *st = (GUIwidgetLoginChoice *)GUI_ID2WIDGET(id)->data1;

    // 释放图标纹理
    glDeleteTextures(1, &st->iconTex[DOCTOR]);
    glDeleteTextures(1, &st->iconTex[PATIENT]);

    // 释放图标
    guiDrawIconDestroy(st->icon[DOCTOR]);
    guiDrawIconDestroy(st->icon[PATIENT]);

    // 释放背景
    guiDrawRRCDestroy(st->back[DOCTOR]);
    guiDrawRRCDestroy(st->back[PATIENT]);

    // 释放文本
    guiStrDelete(st->str);

    // 释放圆形背景
    guiDrawCCDestroy(st->cc);
}

// 回调函数
void gui_widget_login_choice_loadCall(GUIid id)
{
    GUIwidgetLoginChoice *st = (GUIwidgetLoginChoice *)GUI_ID2WIDGET(id)->data1;

    // 设置flag
    st->flag = 0;

    // 设置颜色
    guiDrawIconSetColor(st->icon[DOCTOR], (vec4){0.8f, 0.8f, 0.8f, 1.0f});
    guiDrawRRCSetColor(st->back[DOCTOR], (vec4){0.8f, 0.8f, 0.8f, 0.0f});

    guiDrawIconSetColor(st->icon[PATIENT], (vec4){0.8f, 0.8f, 0.8f, 1.0f});
    guiDrawRRCSetColor(st->back[PATIENT], (vec4){0.8f, 0.8f, 0.8f, 0.0f});

    // 设置位置
    mat4 model;

    glm_translate_make(model, (vec3){-250.0f, 90.0f, 0.0f});
    guiDrawIconSetModel(st->icon[DOCTOR], model);
    guiDrawRRCSetModel(st->back[DOCTOR], model);

    glm_translate_make(model, (vec3){250.0f, 90.0f, 0.0f});
    guiDrawIconSetModel(st->icon[PATIENT], model);
    guiDrawRRCSetModel(st->back[PATIENT], model);

    glm_translate_make(model, (vec3){0.0f, -200.0f, 0.0f});
    guiStrSetModel(st->str, model);

    // 设置圆
    guiDrawCCSetPos(st->cc, 0.0f, 0.0f);
    guiDrawCCSetR(st->cc, 0.0f);
}
void gui_widget_login_choice_uploadCall(GUIid id)
{
    GUIwidgetLoginChoice *st = (GUIwidgetLoginChoice *)GUI_ID2WIDGET(id)->data1;
}

bool gui_widget_login_choice_drawCall(GUIid id)
{
    GUIwidgetLoginChoice *st = (GUIwidgetLoginChoice *)GUI_ID2WIDGET(id)->data1;

    // 初始化模板测试
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilMask(0xFF);

    // 设置模板测试
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    for (int i = 0; i < 2; i++)
    {
        glStencilFunc(GL_ALWAYS, i + 1, 0xFF);

        // 渲染背景
        guiDrawRRCRender(st->back[i]);

        // 渲染按钮
        guiDrawIconRender(st->icon[i]);
    }

    // 计算圆
    if (GUI_FLAG_CHECK(st->flag, GUI_WIDGET_LOGIN_CHOICE_PRESS))
    {
        double time = glfwGetTime() - st->time;
        guiDrawCCSetR(st->cc, sqrt(2 * 380 * 280) * time / 0.3f);
        if (time > 0.3f)
            GUI_FLAG_UNSET(st->flag, GUI_WIDGET_LOGIN_CHOICE_PRESS);
    }

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, st->flag & 3, 0xFF);
    guiDrawCCRender(st->cc);

    // 恢复模板测试
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0x00);

    // 渲染文本
    guiStrRender(st->str);

    return true;
}
bool gui_widget_login_choice_eventCall(GUIid id, const GUIevent *event)
{
    GUIwidgetLoginChoice *st = (GUIwidgetLoginChoice *)GUI_ID2WIDGET(id)->data1;

    int choice[2];
    double posx = event->mousePos.x;
    double posy = event->mousePos.y;
    double x = -190 - 250;
    double y = 190 + 90;
    double w = 380;
    double h = 380;
    choice[DOCTOR] = (posx >= x && posx <= x + w && posy <= y && posy >= y - h);
    x = -190 + 250;
    y = 190 + 90;
    choice[PATIENT] = choice[DOCTOR] ? 0 : (posx >= x && posx <= x + w && posy <= y && posy >= y - h);

    switch (event->type)
    {
        // 鼠标移动
    case GUI_EVENT_TYPE_CURSOR_POS:
        // 选择医生
        if (choice[DOCTOR] && !GUI_FLAG_CHECK(st->flag, GUI_WIDGET_LOGIN_CHOICE_DOCTOR))
        {
            GUI_FLAG_SET(st->flag, GUI_WIDGET_LOGIN_CHOICE_DOCTOR);
            guiDrawIconSetColor(st->icon[DOCTOR], (vec4){0xF0 / 255.0, 0xF8 / 255.0, 0xFF / 255.0, 1.0f});
            guiDrawRRCSetColor(st->back[DOCTOR], (vec4){0xF0 / 255.0, 0xF8 / 255.0, 0xFF / 255.0, 0.2f});
        }
        // 选择患者
        else if (choice[PATIENT] && !GUI_FLAG_CHECK(st->flag, GUI_WIDGET_LOGIN_CHOICE_PATIENT))
        {
            GUI_FLAG_SET(st->flag, GUI_WIDGET_LOGIN_CHOICE_PATIENT);
            guiDrawIconSetColor(st->icon[PATIENT], (vec4){0xF0 / 255.0, 0xF8 / 255.0, 0xFF / 255.0, 1.0f});
            guiDrawRRCSetColor(st->back[PATIENT], (vec4){0xF0 / 255.0, 0xF8 / 255.0, 0xFF / 255.0, 0.2f});
        }
        // 取消选择
        else if (!choice[DOCTOR] && !choice[PATIENT] &&
                 (GUI_FLAG_CHECK(st->flag, GUI_WIDGET_LOGIN_CHOICE_DOCTOR) || GUI_FLAG_CHECK(st->flag, GUI_WIDGET_LOGIN_CHOICE_PATIENT)))
        {
            GUI_FLAG_UNSET(st->flag, GUI_WIDGET_LOGIN_CHOICE_DOCTOR);
            GUI_FLAG_UNSET(st->flag, GUI_WIDGET_LOGIN_CHOICE_PATIENT);

            guiDrawIconSetColor(st->icon[DOCTOR], (vec4){0.8f, 0.8f, 0.8f, 1.0f});
            guiDrawRRCSetColor(st->back[DOCTOR], (vec4){0.8f, 0.8f, 0.8f, 0.0f});

            guiDrawIconSetColor(st->icon[PATIENT], (vec4){0.8f, 0.8f, 0.8f, 1.0f});
            guiDrawRRCSetColor(st->back[PATIENT], (vec4){0.8f, 0.8f, 0.8f, 0.0f});
        }
        break;

    // 鼠标按下并弹起
    case GUI_EVENT_TYPE_MOUSE_BUTTON:
        // 鼠标左键按下
        if (event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT && event->MouseButton.action == GLFW_PRESS)
        {
            if (choice[DOCTOR] + choice[PATIENT])
            {
                GUI_FLAG_SET(st->flag, GUI_WIDGET_LOGIN_CHOICE_PRESS);
                st->time = glfwGetTime();

                guiDrawCCSetPos(st->cc, event->mousePos.x, event->mousePos.y);
            }
        }
        // 鼠标左键弹起
        else if (event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT && event->MouseButton.action == GLFW_RELEASE)
        {
            if (choice[DOCTOR] + choice[PATIENT])
            {
                // 添加任务
                guiTaskAddTask(choiceBackCall, (choice[PATIENT] << 1) | choice[DOCTOR], NULL);
            }
        }
        break;
    }

    return true;
}

static bool choiceBackCall(size_t flag, void *data)
{
    // 恢复登录页面所有控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_USER, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_PASS, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_BUTTON1, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_BUTTON2, GUI_WIDGET_CALLFLAG_ALL, true);
    GUIevent event = {
        .type = GUI_WIDGET_LOGINBACK_DRAW_ICON,
        .Custom = {.data1 = (void *)true}};
    CALL(GUI_ID2WIDGET(GUI_ID_LOGINBACK)->eventCall, GUI_ID_LOGINBACK, &event);

    // 删除选择控件
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_CHOICE);

    // 检查注册的是医生还是患者

    wchar_t *user = guiWidgetInputGetText(GUI_ID_LOGIN_USER);
    wchar_t *pass = guiWidgetInputGetText(GUI_ID_LOGIN_PASS);
    wchar_t *user_pass = malloc((wcslen(user) + wcslen(pass) + 2) * sizeof(wchar_t));
    wcscpy(user_pass, user);
    wcscpy(user_pass + wcslen(user) + 1, pass);
    guiWidgetLogAddMsg(GUI_ID_LOG, GUI_FLAG_CHECK(flag, GUI_WIDGET_LOGIN_CHOICE_DOCTOR) ? L"正在注册医生" : L"正在注册患者", GUI_WIDGET_LOG_MSG);
    webAddTask(WEB_TASK_SEND, loginWebTask, LOGIN_WEB_TASK_REGISTER | (flag & 3), (char *)user_pass);
    
    return true;
}
