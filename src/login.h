#ifndef LOGIN_H
#define LOGIN_H

#include "gui.h"
#include "gui_widget_loginback.h"
#include "web.h"
#include "patient.h"
#include "doctor.h"

#define LOGIN_WEB_TASK_DOCTOR 1   // 医生
#define LOGIN_WEB_TASK_PATIENT 2  // 患者
#define LOGIN_WEB_TASK_LOGIN 4    // 注册
#define LOGIN_WEB_TASK_REGISTER 8 // 登录

// 登录按钮backcall回调
void loginBackCall(GUIid id, size_t flag, void *data);
// 注册按钮backcall回调
void registerBackCall(GUIid id, size_t flag, void *data);

// 登录或注册的web任务
void loginWebTask(SOCKET fd, size_t flag, char *data);

// 登录成功后的操作
bool loginSuccess(size_t flag, void *data);

#endif // LOGIN_H