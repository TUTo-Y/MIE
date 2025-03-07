#include "gui_widget_log.h"

#define LOG_WIDTH (200.0)
#define LOG_HEIGH (50.0)

// 创建和销毁调用函数
void gui_widget_log_registerCall(GUIid id)
{
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GUIwidgetLog));
    memset(GUI_ID2WIDGET(id)->data1, 0, sizeof(GUIwidgetLog));
    GUIwidgetLog *log = (GUIwidgetLog *)GUI_ID2WIDGET(id)->data1;

    // 初始化列表
    listInitList(&log->listA);
    listInitList(&log->listB);
    listInitList(&log->listT);

    // 初始化互斥锁
    pthread_mutex_init(&log->mutex, NULL);

    // 设置颜色
    glm_vec4_copy(log->color[0], (vec4){0.0f, 1.0f, 0.0f, 1.0f}); // 消息颜色
    glm_vec4_copy(log->color[1], (vec4){1.0f, 1.0f, 0.0f, 1.0f}); // 警告颜色
    glm_vec4_copy(log->color[2], (vec4){1.0f, 0.0f, 0.0f, 1.0f}); // 错误颜色

    // 创建渲染器
    log->drawrr = guiDrawRRCCreate(LOG_WIDTH, LOG_HEIGH, 8, 0.02f, (vec4){0.0f, 0.0f, 0.0f, 1.0f});
}
void gui_widget_log_logoffCall(GUIid id)
{
    GUIwidgetLog *log = (GUIwidgetLog *)GUI_ID2WIDGET(id)->data1;

    // 释放渲染器
    guiDrawRRCDestroy(log->drawrr);

    // 释放列表
    listDeleteList(&log->listA, freeLogmsg);
    pthread_mutex_lock(&log->mutex);
    listDeleteList(&log->listB, freeLogmsg);
    pthread_mutex_unlock(&log->mutex);
    listDeleteList(&log->listT, freeLogmsg);

    // 释放互斥锁
    pthread_mutex_destroy(&log->mutex);

    // 释放数据
    free(log);
}

bool gui_widget_log_drawCall(GUIid id)
{
    GUIwidgetLog *log = (GUIwidgetLog *)GUI_ID2WIDGET(id)->data1;

    // 设置起始点
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){-WINDOW_WIDTH / 2.0f + LOG_WIDTH / 2.0f + 20.0f,
                                -WINDOW_HEIGHT / 2.0f + LOG_HEIGH / 2.0f + 20.0f,
                                0.0f});

    guiDrawRRCSetModel(log->drawrr, model);
    guiDrawRRCRender(log->drawrr);
}

void guiWidgetLogAddMsg(GUIid id, const wchar_t *msg, size_t type)
{
    GUIlogmsg logmsg;
    logmsg.msg = (wchar_t *)malloc(sizeof(wchar_t) * (wcslen(msg) + 1));
    wcscpy(logmsg.msg, msg);
    logmsg.type = type;
    logmsg.time = glfwGetTime();

    GUIwidgetLog *log = (GUIwidgetLog *)GUI_ID2WIDGET(id)->data1;
    pthread_mutex_lock(&log->mutex);
    listAddNodeInEnd(&log->listB, listDataToNode(listCreateNode(), &logmsg, sizeof(GUIlogmsg), true));
    pthread_mutex_unlock(&log->mutex);
}

void freeLogmsg(void *arg)
{
    GUIlogmsg *msg = arg;
    free(msg->msg);
    free(msg);
}