#include "gui_widget.h"

GUIid guiWidgetInit(GUIid id,
                    void (*registerCall)(GUIid id),
                    void (*logoffCall)(GUIid id),
                    void (*loadCall)(GUIid id),
                    void (*uploadCall)(GUIid id),
                    bool (*drawCall)(GUIid id),
                    bool (*eventCall)(GUIid id, const GUIevent *event))
{
    GUIwidget *widget = (GUIwidget *)guiIDGet(id);
    if (registerCall)
    {
        widget->registerCall = registerCall;
    }
    if (logoffCall)
    {
        widget->logoffCall = logoffCall;
    }
    if (loadCall)
    {
        widget->loadCall = loadCall;
    }
    if (uploadCall)
    {
        widget->uploadCall = uploadCall;
    }
    if (drawCall)
    {
        widget->drawCall = drawCall;
    }
    if (eventCall)
    {
        widget->eventCall = eventCall;
    }

    CALL(registerCall, id);

    return id;
}

GUIid guiWidgetLogoff(GUIid id, bool isDelete)
{
    GUIwidget *widget = (GUIwidget *)guiIDGet(id);

    CALL(widget->logoffCall, id);

    if (isDelete == true)
        guiIDLogoff(id);

    return id;
}
void guiWidgetToControl(GUIControl *control, GUIid id, size_t flag, size_t priorityEvent, size_t drawEvent, bool enable)
{
    guiControlAddWidget(control, id);
    // 添加进入绘制回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_DRAW))
        guiControlAddCallback(control, id, GUI_WIDGET_CALLFLAG_DRAW, drawEvent, enable);

    GUI_FLAG_UNSET(flag, GUI_WIDGET_CALLFLAG_DRAW);

    // 添加进入事件回调
    if (GUI_FLAG_CHECK(flag, GUI_WIDGET_CALLFLAG_EVENT))
        guiControlAddCallback(control, id, flag, priorityEvent, enable);
}