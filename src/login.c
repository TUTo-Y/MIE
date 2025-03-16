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

    // 医生登录
    if (flag == WEB_MSG_LOGIN_DOCTOR)
    {
        
    }
    // 患者登录
    else if (flag == WEB_MSG_LOGIN_PATIENT)
    {
        // 添加患者界面
        guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BIGCHOICE,
                           GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                           800, 100, true);
    }

    return true;
}