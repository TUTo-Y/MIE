#include "login.h"

static bool login_choice(size_t flag, void *data)
{
    // 禁用登录页面所有控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_USER, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_PASS, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_BUTTON1, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_BUTTON2, GUI_WIDGET_CALLFLAG_ALL, false);
    GUIevent event = {
        .type = GUI_WIDGET_LOGINBACK_DRAW_ICON,
        .Custom = {.data1 = false}};
    CALL(GUI_ID2WIDGET(GUI_ID_LOGINBACK)->eventCall, GUI_ID_LOGINBACK, &event);

    // 添加控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_LOGIN_CHOICE,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);
    return true;
}

void loginBackCall(GUIid id, size_t flag, void *data)
{
    // 检查用户名和密码是否存在
    wchar_t *user = guiWidgetInputGetText(GUI_ID_LOGIN_USER);
    wchar_t *pass = guiWidgetInputGetText(GUI_ID_LOGIN_PASS);

    if (wcslen(user) == 0)
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"用户名为空", GUI_WIDGET_LOG_ERROR);
    }
    else if (wcslen(pass) == 0)
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"密码为空", GUI_WIDGET_LOG_ERROR);
    }
    else
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"正在登录", GUI_WIDGET_LOG_MSG);
        wchar_t *data = malloc((wcslen(user) + wcslen(pass) + 2) * sizeof(wchar_t));
        wcscpy(data, user);
        wcscpy(data + wcslen(user) + 1, pass);
        webAddTask(WEB_TASK_SEND, loginWebTask, LOGIN_WEB_TASK_LOGIN, (char *)data);
    }

    free(user);
    free(pass);
}

void registerBackCall(GUIid id, size_t flag, void *data)
{
    // 检查用户名和密码是否存在
    wchar_t *user = guiWidgetInputGetText(GUI_ID_LOGIN_USER);
    wchar_t *pass = guiWidgetInputGetText(GUI_ID_LOGIN_PASS);

    if (wcslen(user) == 0)
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"用户名为空", GUI_WIDGET_LOG_WARN);
    }
    else if (wcslen(pass) == 0)
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"密码为空", GUI_WIDGET_LOG_WARN);
    }
    else
    {
        // 添加注册选择任务
        guiTaskAddTask(login_choice, 0, NULL);
    }

    free(user);
    free(pass);
}

void loginWebTask(SOCKET fd, size_t flag, char *data)
{
    // 将用户名和密码转化为char类型
    wchar_t *user_w = (wchar_t *)data;
    wchar_t *pass_w = (wchar_t *)(data + wcslen(user_w) * sizeof(wchar_t) + sizeof(wchar_t));
    char user[0x20] = {0};
    char pass[0x20] = {0};
    wcstombs(user, user_w, 0x20);
    wcstombs(pass, pass_w, 0x20);
    free(data);

    // 计算用户名和密码的hash值
    uint8_t user_hash[SM3_DIGEST_SIZE] = {0};
    uint8_t pass_hash[SM3_DIGEST_SIZE] = {0};
    encHash(user, strlen(user), user_hash);
    encHash(pass, strlen(pass), pass_hash);

    // 打包数据 : 用户名hash + 密码hash + 用户名
    uint8_t buf[SM3_DIGEST_SIZE * 2 + 0x20];
    memcpy(buf, user_hash, SM3_DIGEST_SIZE);
    memcpy(buf + SM3_DIGEST_SIZE, pass_hash, SM3_DIGEST_SIZE);
    memcpy(buf + SM3_DIGEST_SIZE * 2, user, strlen(user) + 1);

    // 打包KEM
    ENCkem kem = encKEMEnc(buf, sizeof(buf), &keyServer);

    // 发送flag
    switch (flag & 12)
    {
    case LOGIN_WEB_TASK_LOGIN:
        webSendFlag(fd, WEB_MSG_LOGIN);
        break;
    case LOGIN_WEB_TASK_REGISTER:
        webSendFlag(fd, WEB_MSG_REGISTER);
        webSendFlag(fd, (flag & 3) == LOGIN_WEB_TASK_DOCTOR ? WEB_MSG_REGISTER_DOCTOR : WEB_MSG_REGISTER_PATIENT);
        break;
    }

    // 发送KEM数据包
    webSendData(fd, (const char *)kem, encKEMSizeKEM(kem));
    encKEMFree(kem);

    // 等待服务端回应
    FLAG recv_flag = webRecvFlag(fd);

    // 发送对应消息
    if (recv_flag == WEB_MSG_SUCESS) // 成功
    {
        if ((flag & 12) == LOGIN_WEB_TASK_LOGIN)
        {
            FLAG type = webRecvFlag(fd);
            guiWidgetLogAddMsg(GUI_ID_LOG, L"登录成功", GUI_WIDGET_LOG_MSG);
            guiTaskAddTask(loginSuccess, type, NULL);
        }
        else
        {
            guiWidgetLogAddMsg(GUI_ID_LOG, L"注册成功", GUI_WIDGET_LOG_MSG);
        }
    }
    else if (recv_flag == WEB_MSG_ERROR) // 失败
    {
        if ((flag & 12) == LOGIN_WEB_TASK_LOGIN)
        {
            FLAG error = webRecvFlag(fd);
            switch (error)
            {
            case WEB_MSG_LOGIN_ERROR_USER:
                guiWidgetLogAddMsg(GUI_ID_LOG, L"登录用户不存在", GUI_WIDGET_LOG_ERROR);
                break;
            case WEB_MSG_LOGIN_ERROR_PASS:
                guiWidgetLogAddMsg(GUI_ID_LOG, L"登录密码错误", GUI_WIDGET_LOG_ERROR);
                break;
            case WEB_MSG_LOGIN_ERROR_REPE:
                guiWidgetLogAddMsg(GUI_ID_LOG, L"账号已登录", GUI_WIDGET_LOG_ERROR);
                break;
            }
        }
        else
            guiWidgetLogAddMsg(GUI_ID_LOG, L"用户已被注册", GUI_WIDGET_LOG_WARN);
    }
}
// flag 取值为 WEB_MSG_LOGIN_DOCTOR 或 WEB_MSG_LOGIN_PATIENT
bool loginSuccess(size_t flag, void *data)
{
    // 删除登录页面
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGINBACK);
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_USER);
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_PASS);
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_BUTTON1);
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_BUTTON2);
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_LOGIN_CHOICE);

    // 患者登录
    if (flag == WEB_MSG_LOGIN_PATIENT)
    {
        guiSetDefaultClearColor(1.0f, 0.9f, 0.8f, 1.0f);
        // 添加患者界面
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BIGCHOICE,
                           GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                           800, 100, true);
    }
    // 医生登录
    else if (flag == WEB_MSG_LOGIN_DOCTOR)
    {
        guiSetDefaultClearColor(0.8f, 0.9f, 1.0f, 1.0f);
        // 设置医生等待界面
        mat4 model;
        glm_translate_make(model, (vec3){0.0f, -250.0f, 0.0f});
        guiStrCpy(guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_WAIT), L"等待患者上传文件中...");

        // 添加医生等待界面
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, 0, 100, true);
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_WAIT, GUI_WIDGET_CALLFLAG_DRAW, 0, 100, true);

        // 启用控件
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_IMG,
                           GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_SCROLL,
                           800, 100, false);

        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_NAME,
                           GUI_WIDGET_CALLFLAG_DRAW,
                           800, 100, false);
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_AGE,
                           GUI_WIDGET_CALLFLAG_DRAW,
                           800, 100, false);
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_STATE,
                           GUI_WIDGET_CALLFLAG_DRAW,
                           800, 100, false);
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_ADVICE,
                           GUI_WIDGET_CALLFLAG_DRAW,
                           800, 100, false);
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_INPUT_ADVICE,
                           GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                           800, 100, false);
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_BUTTON_SEND, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                           800, 100, false);

        // 设置图片显示控件
        guiWidgetImgSetRect(GUI_ID_DOCTOR_IMG, (GUIrect){-640.0f, 400.0f, 500.0f, 500.0f});

        // 设置四个文本的位置
        GUIstr *name = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_NAME);
        GUIstr *age = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_AGE);
        GUIstr *state = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_STATE);
        GUIstr *advice = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_ADVICE);
        guiStrSetMode(name, GUI_STR_MOD_LEFT_TOP);
        guiStrSetMode(age, GUI_STR_MOD_LEFT_TOP);
        guiStrSetMode(state, GUI_STR_MOD_LEFT_TOP);
        guiStrSetMode(advice, GUI_STR_MOD_LEFT_TOP);
        guiStrAppMode(name);
        guiStrAppMode(age);
        guiStrAppMode(state);
        guiStrAppMode(advice);

        glm_translate_make(model, (vec3){-640.0f, -240.0f, 0.0f});
        guiStrSetModel(advice, model);
        glm_translate_make(model, (vec3){100.0f, 0.0f, 0.0f});
        guiStrSetModel(state, model);
        glm_translate(model, (vec3){0.0f, 190.0f, 0.0f});
        guiStrSetModel(age, model);
        glm_translate(model, (vec3){0.0f, 190.0f, 0.0f});
        guiStrSetModel(name, model);

        guiStrCpy(advice, L"建议:");

        // 设置输入框
        guiWidgetInputSetPos(GUI_ID_DOCTOR_INPUT_ADVICE, (vec3){-50.0f, -320.0f, 0.0f});
        guiWidgetInputSetDefaultText(GUI_ID_DOCTOR_INPUT_ADVICE, L"请输入诊断建议");
        guiWidgetInputDefaultTextColor(GUI_ID_DOCTOR_INPUT_ADVICE, (vec4){0.5f, 0.5f, 0.5f, 0.9f});
        guiWidgetInputSetTextColor(GUI_ID_DOCTOR_INPUT_ADVICE, (vec4){0.26f, 0.26f, 0.26f, 1.0f});
        guiWidgetInputSetLineSize(GUI_ID_DOCTOR_INPUT_ADVICE, 800.0f);
        guiWidgetInputSetMax(GUI_ID_DOCTOR_INPUT_ADVICE, 0x15);

        // 设置按钮
        guiWidgetButtonSetText(GUI_ID_DOCTOR_BUTTON_SEND, L"发送");
        guiWidgetButtonSetPos(GUI_ID_DOCTOR_BUTTON_SEND, 400, -250, 200, 100);
        guiWidgetButtonSetColor(GUI_ID_DOCTOR_BUTTON_SEND, (vec4){0x0b / 255.0f, 0xe8 / 255.0f, 0x81 / 255.0f, 1.0f}, (vec4){0x05 / 255.0f, 0xc4 / 255.0f, 0x6b / 255.0f, 1.0f});
        guiWidgetButtonSetBackCall(GUI_ID_DOCTOR_BUTTON_SEND, doctorSend, 0, NULL);

        // 进入医生的Recv模式
        pthread_t thread;
        pthread_create(&thread, NULL, doctorWaitThread, NULL);
        pthread_detach(thread);
    }

    return true;
}