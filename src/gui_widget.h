/**
 * \file gui_widget.h
 * \brief GUI 控件
 */

#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include <pthread.h>
#include <semaphore.h>

#include "gui_type.h"
#include "gui_ID.h"
#include "gui.h"

/**
 * \brief 注册时初始化一个控件
 * \param id 控件ID
 * \param registerCall 注册回调函数
 * \param logoffCall 注销回调函数
 * \param loadCall 加载回调函数
 * \param uploadCall 卸载回调函数
 * \param msg 消息回调函数
 * \param drawCall 绘制回调函数
 * \param eventCall 事件回调函数
 * \return ID
 * \note guiWidgetInit(guiIDRegister(ID), ...);
 */
GUIid guiWidgetInit(GUIid id,
                   void (*registerCall)(GUIid id),
                   void (*logoffCall)(GUIid id),
                   void (*loadCall)(GUIid id),
                   void (*uploadCall)(GUIid id),
                   bool (*drawCall)(GUIid id),
                   bool (*eventCall)(GUIid id, const GUIevent *event));

/**
 * \brief 注销一个控件
 * \param id 控件ID
 * \param isDelete 是否使用guiIDLogoff删除ID
 * \return 被注销的ID
 */
GUIid guiWidgetLogoff(GUIid id, bool isDelete);

/**
 * \brief 添加一个控件
 * \param control 控制器
 * \param id 控件ID
 * \param flag 控件启动的响应
 * \param priorityEvent 事件优先级
 * \param drawEvent 绘制优先级
 * \param enable 是否启用
 * \note guiControlAddWidget和guiControlAddCallback的简化版
 */
void guiWidgetToControl(GUIid fid, GUIid id, size_t flag, size_t priorityEvent, size_t drawEvent, bool enable);

#endif // GUI_WIDGET_H