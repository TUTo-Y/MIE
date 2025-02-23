#include "gui_widgetID.h"

GUIwidget **IDlist = NULL;
bool guiIDRegister(uint64_t id, GUIwidget *widget)
{
    if(IDlist == NULL)
    {
        IDlist = (GUIwidget**)malloc(GUI_WIDGET_ID_MAX * sizeof(GUIwidget**));
        memset(IDlist, 0, GUI_WIDGET_ID_MAX * sizeof(GUIwidget**));
    }
    if(id >= GUI_WIDGET_ID_MAX)
    {
        // ID范围出错了
        ERROR("ID范围出错了:ID = %lld\n", id);
        return false;
    }
    if(IDlist[id] != NULL)
    {
        // 已经注册过了
        ERROR("ID为:%lld的控件已经注册过了\n", id);
        return false;
    }
    IDlist[id] = widget;
    return true;
}

/**
 * \brief 根据控件ID获取控件
 */
GUIwidget *guiIDGetFromID(uint64_t id)
{
    if(IDlist == NULL)
    {
        // 没有注册过
        ERROR("ID列表为空\n");
        return NULL;
    }
    if(id >= GUI_WIDGET_ID_MAX)
    {
        // ID范围出错了
        ERROR("ID范围出错了:ID = %lld\n", id);
        return NULL;
    }
    if(IDlist[id] == NULL)
    {
        // 没有注册过
        ERROR("ID为:%lld的控件没有注册过\n", id);
        return NULL;
    }
    return IDlist[id];
}

/**
 * \brief 根据控件地址获取控件ID
 * \param widget 控件地址
 */
uint64_t guiIDGetFromWidget(GUIwidget *widget)
{
    if(IDlist == NULL)
    {
        // 没有注册过
        ERROR("ID列表为空\n");
        return 0;
    }
    for(int i = 0; i < GUI_WIDGET_ID_MAX; i++)
    {
        if(IDlist[i] == widget)
        {
            return (uint64_t)i;
        }
    }
    // 没有注册过
    ERROR("控件地址为:%p的控件没有注册过\n", widget);
    return 0;
}