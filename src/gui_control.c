#include "gui_control.h"

int guiControlComparePriorityCall(const void *src, const void *dic)
{
    GUIcallback *srcControl = (GUIcallback *)src;
    GUIcallback *dicControl = (GUIcallback *)dic;

    // 从小到大
    // return srcControl->priority - dicControl->priority;
    // 从大到小
    return dicControl->priority - srcControl->priority;
}

bool guiControlCompareFindIDCall(const void *src, const void *dic)
{
    GUIid id = *(GUIid *)src;
    GUIcallback *dicControl = (GUIcallback *)dic;

    return id == dicControl->id;
}

void guiControlInit(GUIControl *control)
{
    vectorInit(&control->Draw, sizeof(GUIcallback), 0x10, guiControlComparePriorityCall);
    vectorInit(&control->EventMouseButton, sizeof(GUIcallback), 0x10, guiControlComparePriorityCall);
    vectorInit(&control->EventCursorPos, sizeof(GUIcallback), 0x10, guiControlComparePriorityCall);
    vectorInit(&control->EventCharMods, sizeof(GUIcallback), 0x10, guiControlComparePriorityCall);
    vectorInit(&control->EventKey, sizeof(GUIcallback), 0x10, guiControlComparePriorityCall);
    vectorInit(&control->EventScroll, sizeof(GUIcallback), 0x10, guiControlComparePriorityCall);
    listInitList(&control->widget);
}

void guiControlDestroy(GUIControl *control)
{
    // 删除所有控件
    list *node;
    while (node = listGetNodeFromStart(&control->widget))
    {
        // 处理控件的释放
        GUIwidget *widget = (GUIwidget *)node->data;
        guiControlDeleteWidget(control, ((GUItype *)widget)->id);

        // 释放该节点
        // 该节点在guiControlDeleteWidget中被删除
    }

    listDeleteList(&control->widget, NULL);
    vectorDestroy(&control->Draw);
    vectorDestroy(&control->EventMouseButton);
    vectorDestroy(&control->EventCursorPos);
    vectorDestroy(&control->EventCharMods);
    vectorDestroy(&control->EventKey);
    vectorDestroy(&control->EventScroll);
}

void guiControlAddWidget(GUIControl *control, GUIid id)
{
    listAddNodeInEnd(&control->widget, listDataToNode(listCreateNode(), guiIDGet(id), 0, false));
    CALL(((GUIwidget *)guiIDGet(id))->loadCall, id);
}

void guiControlDeleteWidget(GUIControl *control, GUIid id)
{
    guiControlDeleteCallback(control, id, GUI_WIDGET_CALLFLAG_ALL); // 删除这个ID的所有回调函数

    listDeleteNode(&control->widget, listSearchDataAddr(&control->widget, guiIDGet(id)), NULL);
    CALL(((GUIwidget *)guiIDGet(id))->uploadCall, id);
}

void guiControlAddCallback(GUIControl *control, GUIid id, size_t flag, uint64_t priority, bool enable)
{
    GUIcallback callback;
    callback.id = id;
    callback.priority = priority;
    callback.enable = enable;

    // 绘制回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_DRAW) &&
        ((GUIwidget *)guiIDGet(id))->drawCall)
    {
        callback.drawCall = ((GUIwidget *)guiIDGet(id))->drawCall;
        vectorAdd(&control->Draw, &callback);
    }

    // 鼠标事件回调
    if (((GUIwidget *)guiIDGet(id))->eventCall) // 回调函数必须存在才能添加
    {
        if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON))
        {
            callback.eventCall = ((GUIwidget *)guiIDGet(id))->eventCall;
            vectorAdd(&control->EventMouseButton, &callback);
        }

        // 光标事件回调
        if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS))
        {
            callback.eventCall = ((GUIwidget *)guiIDGet(id))->eventCall;
            vectorAdd(&control->EventCursorPos, &callback);
        }

        // 字符事件回调
        if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS))
        {
            callback.eventCall = ((GUIwidget *)guiIDGet(id))->eventCall;
            vectorAdd(&control->EventCharMods, &callback);
        }

        // 键盘事件回调
        if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_KEY))
        {
            callback.eventCall = ((GUIwidget *)guiIDGet(id))->eventCall;
            vectorAdd(&control->EventKey, &callback);
        }

        // 滚轮事件回调
        if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_SCROLL))
        {
            callback.eventCall = ((GUIwidget *)guiIDGet(id))->eventCall;
            vectorAdd(&control->EventScroll, &callback);
        }
    }
}

void guiControlResetCallbackPriority(GUIControl *control, GUIid id, size_t flag, uint64_t priority)
{
    GUIcallback *callback;

    // 绘制回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_DRAW))
    {
        size_t fret = vectorFindSimple(&control->Draw, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->Draw, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->Draw, fret);
            callback->priority = priority;
            vectorSort(&control->Draw);
        }
    }

    // 鼠标事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON))
    {
        size_t fret = vectorFindSimple(&control->EventMouseButton, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventMouseButton, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventMouseButton, fret);
            callback->priority = priority;
            vectorSort(&control->EventMouseButton);
        }
    }

    // 光标事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS))
    {
        size_t fret = vectorFindSimple(&control->EventCursorPos, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventCursorPos, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventCursorPos, fret);
            callback->priority = priority;
            vectorSort(&control->EventCursorPos);
        }
    }

    // 字符事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS))
    {
        size_t fret = vectorFindSimple(&control->EventCharMods, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventCharMods, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventCharMods, fret);
            callback->priority = priority;
            vectorSort(&control->EventCharMods);
        }
    }

    // 键盘事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_KEY))
    {
        size_t fret = vectorFindSimple(&control->EventKey, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventKey, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventKey, fret);
            callback->priority = priority;
            vectorSort(&control->EventKey);
        }
    }

    // 滚轮事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_SCROLL))
    {
        size_t fret = vectorFindSimple(&control->EventScroll, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventScroll, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventScroll, fret);
            callback->priority = priority;
            vectorSort(&control->EventScroll);
        }
    }
}

void guiControlDeleteCallback(GUIControl *control, GUIid id, size_t flag)
{
    // 绘制回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_DRAW))
    {
        size_t fret = vectorFindSimple(&control->Draw, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->Draw, fret)) // 检查搜索结果
            vectorDelete(&control->Draw, fret);
    }

    // 鼠标事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON))
    {
        size_t fret = vectorFindSimple(&control->EventMouseButton, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventMouseButton, fret)) // 检查搜索结果
            vectorDelete(&control->EventMouseButton, fret);
    }

    // 光标事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS))
    {
        size_t fret = vectorFindSimple(&control->EventCursorPos, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventCursorPos, fret)) // 检查搜索结果
            vectorDelete(&control->EventCursorPos, fret);
    }

    // 字符事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS))
    {
        size_t fret = vectorFindSimple(&control->EventCharMods, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventCharMods, fret)) // 检查搜索结果
            vectorDelete(&control->EventCharMods, fret);
    }

    // 键盘事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_KEY))
    {
        size_t fret = vectorFindSimple(&control->EventKey, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventKey, fret)) // 检查搜索结果
            vectorDelete(&control->EventKey, fret);
    }

    // 滚轮事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_SCROLL))
    {
        size_t fret = vectorFindSimple(&control->EventScroll, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventScroll, fret)) // 检查搜索结果
            vectorDelete(&control->EventScroll, fret);
    }
}

void guiControlEnableCallback(GUIControl *control, GUIid id, size_t flag, bool enable)
{
    GUIcallback *callback;
    // 绘制回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_DRAW))
    {
        size_t fret = vectorFindSimple(&control->Draw, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->Draw, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->Draw, fret);
            callback->enable = enable;
        }
    }

    // 鼠标事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON))
    {
        size_t fret = vectorFindSimple(&control->EventMouseButton, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventMouseButton, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventMouseButton, fret);
            callback->enable = enable;
        }
    }

    // 光标事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS))
    {
        size_t fret = vectorFindSimple(&control->EventCursorPos, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventCursorPos, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventCursorPos, fret);
            callback->enable = enable;
        }
    }

    // 字符事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS))
    {
        size_t fret = vectorFindSimple(&control->EventCharMods, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventCharMods, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventCharMods, fret);
            callback->enable = enable;
        }
    }

    // 键盘事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_KEY))
    {
        size_t fret = vectorFindSimple(&control->EventKey, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventKey, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventKey, fret);
            callback->enable = enable;
        }
    }

    // 滚轮事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT_SCROLL))
    {
        size_t fret = vectorFindSimple(&control->EventScroll, &id, guiControlCompareFindIDCall);
        if (vectorCheckFind(&control->EventScroll, fret)) // 检查搜索结果
        {
            callback = (GUIcallback *)vectorGet(&control->EventScroll, fret);
            callback->enable = enable;
        }
    }
}

void guiControlRunDrawCallback(Vector *vector)
{
    for (size_t i = 0; i < vector->index; i++)
    {
        GUIcallback *callback = (GUIcallback *)(vector->data + i * vector->step);
        // 检查是否启用
        if (callback->enable)
            CALL(callback->drawCall, callback->id);
    }
}

void guiControlRunEventCallback(Vector *vector, const GUIevent *event)
{
    for (size_t i = 0; i < vector->index; i++)
    {
        GUIcallback *callback = (GUIcallback *)(vector->data + i * vector->step);
        // 检查是否启用
        if (callback->enable)
            if (CALL(callback->eventCall, callback->id, event) == false)
                break;
    }
}