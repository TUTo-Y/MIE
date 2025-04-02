/**
 * \file gui_ID.h
 * \brief 为GUI创建和绑定唯一的ID
 */
#ifndef GUI_ID_H
#define GUI_ID_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "gui_type.h"

// GUI 的静态ID
enum
{
    GUI_ID_WINDOW = 0, // 窗口ID

    GUI_ID_MOUSEMOVE, // 鼠标移动窗口ID
    GUI_ID_LOG,       // 日志ID
    GUI_ID_LOGINBACK, // 登录背景ID

    GUI_ID_WAIT, // 等待ID

    GUI_ID_LOGIN_USER,    // 登录用户名文本框ID
    GUI_ID_LOGIN_PASS,    // 登录密码文本框ID
    GUI_ID_LOGIN_BUTTON1, // 登录提交按钮ID
    GUI_ID_LOGIN_BUTTON2, // 登录注册按钮ID
    GUI_ID_LOGIN_CHOICE,  // 注册选择用户医生类型

    // 医生和患者公用的图像列表ID
    GUI_ID_IMG_CHOICE_LIST, // 图像列表ID

    // 患者控件ID
    GUI_ID_PATIENT_IMG_IMG1,            // 患者图像, 未嵌入的图像ID
    GUI_ID_PATIENT_IMG_IMG2,            // 患者图像, 嵌入的图像ID
    GUI_ID_PATIENT_BUTTON_CHOICE_IMG,   // 患者选择文本按钮
    GUI_ID_PATIENT_BUTTON_CHOICE_TXET,  // 患者选择文本按钮
    GUI_ID_PATIENT_BUTTON_IMPLANT_TXET, // 患者嵌入文本按钮
    GUI_ID_PATIENT_BUTTON_UPLOAD,       // 患者上传按钮ID

    GUI_ID_PATIENT_TEXT_WAIT1, // 患者等待医生接单文本

    GUI_ID_PATIENT_TEXT_ALLOW,    // 患者是否允许医生治疗
    GUI_ID_PATIENT_BUTTON_ALLOW,  // 患者允许医生接单按钮ID
    GUI_ID_PATIENT_BUTTON_REFUSE, // 患者拒绝医生接单按钮ID

    GUI_ID_PATIENT_TEXT_WAIT2, // 患者等待医生检查文本

    GUI_ID_PATIENT_BUTTON_RESULT, // 患者查看检查结果按钮ID

    // 医生控件ID
    GUI_ID_DOCTOR_TEXT_WAIT1, // 医生等待接单文本

    GUI_ID_DOCTOR_IMG_IMG,       // 图像
    GUI_ID_DOCTOR_BUTTON_RESULT, // 医生请求查看病例按钮
    GUI_ID_DOCTOR_BUTTON_UPLOAD, // 医生上传医嘱按钮

    GUI_ID_DOCTOR_TEXT_ADVICE,    // 医生医嘱提示文本

    GUI_ID_END
};

// 本想做成外部不可见，但是为了将获取ID转变成内联函数，所以暴露出来
typedef struct
{
    GUItype **id; // ID数组
    size_t count; // ID数组元素数量
    size_t index; // 动态ID的index, 表示下一个要创建的动态ID的起始地址
} GUIidList_t;
extern GUIidList_t GUIidList;

// 初始化ID列表
void guiIDInit();

// 删除ID列表
void guiIDQuit();

// 获取一个动态ID
GUIid guiIDCreateID();

/**
 * \brief 注册ID
 * \param id ID
 * \return ID
 */
GUIid guiIDRegister(GUIid id);

/**
 * \brief 注销ID
 * \param id ID
 */
void guiIDLogoff(GUIid id);

/**
 * \brief 获取ID
 * \param id ID
 */
static inline GUItype *guiIDGet(GUIid id)
{

    return GUIidList.id[id];
}

#endif // GUI_ID_H