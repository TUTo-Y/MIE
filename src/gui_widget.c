#include "gui_widget.h"
#include "gui_widgetID.h"
#include "gui.h"
/**
 * \brief 初始化并注册控件
 */
void guiWidgetInit(GUIwidget *widget,
                   uint64_t flag,
                   uint64_t id,
                   void (*init)(GUIwin *win, GUIwidget *widget),
                   void (*destroy)(GUIwin *win, GUIwidget *widget),
                   void (*msg)(GUIwin *win, GUIwidget *widget, uint64_t type, void *data),
                   bool (*callDraw)(GUIwin *win, GUIwidget *widget),
                   bool (*callEvent)(GUIwin *win, GUIwidget *widget, const GUIevent *event),
                   int priorityDraw,
                   int priorityEventMouseButton,
                   int priorityEventCursorPos,
                   int priorityEventCharMods,
                   int priorityEventScroll,
                   void *data,
                   void *data2,
                   void (*StartCall)(GUIwidget *widget),
                   void (*DestroyCall)(GUIwidget *widget))
{
    memset(widget, 0, sizeof(GUIwidget));

    widget->flag = flag;
    widget->id = id;

    widget->init = init;
    widget->destroy = destroy;
    widget->msg = msg;
    widget->callDraw = callDraw;
    widget->callEvent = callEvent;

    widget->priorityDraw = priorityDraw;
    widget->priorityEventMouseButton = priorityEventMouseButton;
    widget->priorityEventCursorPos = priorityEventCursorPos;
    widget->priorityEventCharMods = priorityEventCharMods;
    widget->priorityEventScroll = priorityEventScroll;

    widget->data = data;
    widget->data2 = data2;

    widget->win = NULL;

    widget->StartCall = StartCall;
    widget->DestroyCall = DestroyCall;

    // 注册控件
    guiIDRegister(widget->id, widget);

    // 调用初始化函数
    CALL(widget->StartCall, widget);
}

void guiWidgetDestroy(GUIwidget *widget)
{
    // 调用销毁函数
    CALL(widget->DestroyCall, widget);
}