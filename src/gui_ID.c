#include "gui_ID.h"
#define GUI_LIST_ADD_SIZE 0x20

GUIidList_t GUIidList;

void guiIDExpand()
{
    if (GUIidList.index < GUIidList.count)
        return;

    GUIidList.count += GUI_LIST_ADD_SIZE;
    GUIidList.id = (GUItype **)realloc(GUIidList.id, GUIidList.count * sizeof(GUItype *));
    memset(GUIidList.id + GUIidList.index, 0, GUI_LIST_ADD_SIZE * sizeof(GUItype *));
}

void guiIDInit()
{
    GUIidList.count = GUI_ID_END + GUI_LIST_ADD_SIZE;
    GUIidList.index = GUI_ID_END;
    GUIidList.id = (GUItype **)malloc(GUIidList.count * sizeof(GUItype *));
    memset(GUIidList.id, 0, GUIidList.count * sizeof(GUItype *));
}

void guiIDQuit()
{
    for (size_t i = 0; i < GUIidList.count; i++)
    {
        if (GUIidList.id[i] != NULL)
            free(GUIidList.id[i]);
    }

    free(GUIidList.id);
}

GUIid guiIDCreateID()
{
    return GUIidList.index++;
}

GUIid guiIDRegister(GUIid id)
{
    guiIDExpand();

    // 分配内存
    GUIidList.id[id] = (GUItype *)malloc(sizeof(GUItype));
    memset(GUIidList.id[id], 0, sizeof(GUItype));
    GUIidList.id[id]->id = id;

    // 更新index
    if (GUIidList.index == id)
        GUIidList.index++;

    return id;
}

void guiIDLogoff(GUIid id)
{
    if (GUIidList.id[id] == NULL)
        return;

    // 释放内存
    free(GUIidList.id[id]);
    GUIidList.id[id] = NULL;

    // 更新index
    if (GUIidList.index > id)
        GUIidList.index = id;
}
