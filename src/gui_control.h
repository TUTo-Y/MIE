/**
 * \file gui_control.h
 * \brief GUI control 控制器
 */
#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

#include "gui_type.h"
#include "gui_ID.h"
#include "gui.h"

/**
 * \brief 控制器回调函数优先级比较回调函数
 * \param src 源
 * \param dic 目标
 */
int guiControlComparePriorityCall(const void *src, const void *dic);

/**
 * \brief 控制器查找回调函数
 * \param src 源
 * \param dic 目标
 */
bool guiControlCompareFindIDCall(const void *src, const void *dic);

/**
 * \brief 初始化控制器
 */
void guiControlInit(GUIControl *control);

/**
 * \brief 销毁控制器
 */
void guiControlDestroy(GUIControl *control);

/**
 * \brief 向控制器的控件列表添加控件
 * \param control 控制器
 * \param id 控件ID
 * \note 将会调用回调函数
 */
void guiControlAddWidget(GUIControl *control, GUIid id);

/**
 * \brief 从控制器的控件列表删除控件
 * \param control 控制器
 * \param id 控件ID
 * \note 将会调用回调函数
 */
void guiControlDeleteWidget(GUIControl *control, GUIid id);

/**
 * \brief 向控制器的回调函数列表添加回调函数
 * \param control 控制器
 * \param id 控件ID
 * \param flag 标志, 例如GUI_WIDGET_CALLFLAG_DRAW | 例如GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON
 * \param priority 优先级
 * \param enable 是否启用
 */
void guiControlAddCallback(GUIControl *control, GUIid id, size_t flag, uint64_t priority, bool enable);

/**
 * \brief 重新设置回调函数的优先级
 * \param control 控制器
 * \param id 控件ID
 * \param flag 标志, 例如GUI_WIDGET_CALLFLAG_DRAW | 例如GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON
 * \param priority 优先级
 * \note 重新设置优先级后, 会重新排序回调函数列表
 */
void guiControlResetCallbackPriority(GUIControl *control, GUIid id, size_t flag, uint64_t priority);

/**
 * \brief 从控制器的回调函数列表删除回调函数
 * \param control 控制器
 * \param id 控件ID
 * \param flag 标志, 例如GUI_WIDGET_CALLFLAG_DRAW | 例如GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON
 */
void guiControlDeleteCallback(GUIControl *control, GUIid id, size_t flag);

/**
 * \brief 启用或禁用回调函数
 * \param control 控制器
 * \param id 控件ID
 * \param flag 标志, 用于区分不同的回调函数, 例如GUI_WIDGET_CALLFLAG_DRAW
 * \param enable 是否启用
 */
void guiControlEnableCallback(GUIControl *control, GUIid id, size_t flag, bool enable);

/**
 * \brief 运行回调渲染函数
 * \param Vector 回调函数列表
 */
void guiControlRunDrawCallback(Vector *vector);

/**
 * \brief 运行回调事件函数
 * \param Vector 回调函数列表
 */
void guiControlRunEventCallback(Vector *vector, const GUIevent *event);

#endif // GUI_CONTROL_H
