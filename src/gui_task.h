#ifndef GUI_TASK_H
#define GUI_TASK_H

#include <stdbool.h>

#include "gui_base.h"
#include "gui_type.h"
#include "gui_ID.h"

#include "list.h"

typedef struct
{
    bool (*taskfun)(size_t flag, void *data); // true完成这次任务后自我删除, false保留
    size_t flag;
    void *data;
} GUItask;

/**
 * \brief 向窗口添加任务
 * \param taskfun 任务函数
 * \param flag 任务标志
 * \param data 任务数据
 */
void guiTaskAddTask(bool (*taskfun)(size_t flag, void *data), size_t flag, void *data);

/**
 * \brief 运行任务
 * \param task 任务
 * \return true完成这次任务后自我删除, false保留
 */
bool guiTaskRunTask(GUItask *task);

#endif // GUI_TASK_H