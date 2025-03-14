#include "gui_widget_log.h"

#define LOG_WIDTH (200.0)
#define LOG_HEIGH (60.0)

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

    // 创建渲染器
    vec4 Color[3][4] = {
        // 消息颜色
        {0xcb / 255.0f, 0xf1 / 255.0f, 0xf5 / 255.0f, 1.0f,
         0xe3 / 255.0f, 0xfd / 255.0f, 0xfd / 255.0f, 1.0f,
         0xe3 / 255.0f, 0xfd / 255.0f, 0xfd / 255.0f, 1.0f,
         0xcb / 255.0f, 0xf1 / 255.0f, 0xf5 / 255.0f, 1.0f},
        // 警告颜色
        {0xff / 255.0f, 0xf5 / 255.0f, 0x9d / 255.0f, 1.0f,
         0xff / 255.0f, 0xf9 / 255.0f, 0xc4 / 255.0f, 1.0f,
         0xff / 255.0f, 0xf9 / 255.0f, 0xc4 / 255.0f, 1.0f,
         0xff / 255.0f, 0xf5 / 255.0f, 0x9d / 255.0f, 1.0f},
        // 错误颜色
        {254.0f / 255.0f, 67.0f / 255.0f, 101.0f / 255.0f, 1.0f,
         252.0f / 255.0f, 157.0f / 255.0f, 154.0f / 255.0f, 1.0f,
         252.0f / 255.0f, 157.0f / 255.0f, 154.0f / 255.0f, 1.0f,
         254.0f / 255.0f, 67.0f / 255.0f, 101.0f / 255.0f, 1.0f}};
    for (int i = 0; i < 3; i++)
    {
        log->backColor[i] = guiDrawRRC2Create(Color[i], 10.0f, 0.02f);
        guiDrawRRC2SetWH(log->backColor[i], LOG_WIDTH, LOG_HEIGH);
    }
}
void gui_widget_log_logoffCall(GUIid id)
{
    GUIwidgetLog *log = (GUIwidgetLog *)GUI_ID2WIDGET(id)->data1;

    // 释放渲染器
    for (int i = 0; i < 3; i++)
        guiDrawRRC2Destroy(log->backColor[i]);

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

    // 如果T列表中存在node, 则将node添加到A列表
    if (log->listT.fd != &log->listT)
    {
        GUIlogmsg *msg = log->listT.fd->data;
        double time = glfwGetTime() - msg->time;
        // 如果时间大于0.3s, 则将T列表中的消息添加到A列表
        if (time > 0.3)
        {
            pthread_mutex_lock(&log->mutex);
            listAddNodeInEnd(&log->listA, listGetNodeFromStart(&log->listT));
            pthread_mutex_unlock(&log->mutex);
        }
        // // 消息上移
        // else if (time < 0.2)
        // {
        //     float a = time / 0.2;
        //     // 其余消息上移
        //     glm_translate(model, (vec3){0.0f, a * (LOG_HEIGH + 10.0f), 0.0f});
        // }
        // // 显示
        // else
        // {
        //     // 设置透明度
        //     float a = (time - 0.2) / 0.3;
        //     // 渲染背景
        //     guiDrawRRC2SetAlpha(log->backColor[msg->type], a);
        //     guiDrawRRC2SetModel(log->backColor[msg->type], model);
        //     guiDrawRRC2Render(log->backColor[msg->type]);
        //     // 渲染消息
        //     glm_vec4_copy((vec4){0.2f, 0.2f, 0.2f, a}, guiStrGetColor(msg->msg));
        //     guiStrSetModel(msg->msg, model);
        //     guiStrRender(msg->msg);
        //     // 其余消息上移
        //     glm_translate(model, (vec3){0.0f, (LOG_HEIGH + 10.0f), 0.0f});
        // }
        else
        {
            // 设置透明度
            float a = time / 0.3;
            // 渲染背景
            guiDrawRRC2SetAlpha(log->backColor[msg->type], a);
            guiDrawRRC2SetModel(log->backColor[msg->type], model);
            guiDrawRRC2Render(log->backColor[msg->type]);
            // 渲染消息
            glm_vec4_copy((vec4){0.2f, 0.2f, 0.2f, a}, guiStrGetColor(msg->msg));
            guiStrSetModel(msg->msg, model);
            guiStrRender(msg->msg);

            // 其余消息上移
            // glm_translate(model, (vec3){0.0f, a * (LOG_HEIGH + 10.0f), 0.0f});
            glm_translate(model, (vec3){0.0f, (70 * 2 / 0.3) * time - 0.5 * (70 * 2 / (0.3 * 0.3)) * time * time, 0.0f});
        }
    }
    // 检查B列表是否有消息
    else if (log->listB.fd != &log->listB)
    {
        // 将B列表中的消息添加到T列表，同时更新time
        pthread_mutex_lock(&log->mutex);
        listAddNodeInEnd(&log->listT, listGetNodeFromStart(&log->listB));
        pthread_mutex_unlock(&log->mutex);

        // 渲染消息
        ((GUIlogmsg *)log->listT.fd->data)->msg = guiStrCreate(font_default, 32, GUI_STR_MOD_CENTER2, program.font, NULL, (vec4){0.2f, 0.2f, 0.2f, 1.0f});
        guiStrCpy(((GUIlogmsg *)log->listT.fd->data)->msg, ((GUIlogmsg *)log->listT.fd->data)->str);
        free(((GUIlogmsg *)log->listT.fd->data)->str);
        ((GUIlogmsg *)log->listT.fd->data)->str = NULL;

        // 更新时间
        ((GUIlogmsg *)log->listT.fd->data)->time = glfwGetTime();
    }

    // 渲染消息
    list *node = log->listA.fd;
    while (node != &log->listA)
    {
        list *tmp = node;
        node = node->fd;
        GUIlogmsg *msg = tmp->data;

        double time = glfwGetTime() - msg->time;
        // 删除超时的消息
        if (time > 3.0)
        {
            // 从列表中删除该消息
            listDeleteNode(&log->listA, tmp, freeLogmsg);
            continue;
        }
        // 渲染当前消息
        else
        {
            // 设置透明度
            float a = time < 2.0 ? 1.0f : 3.0 - time;
            // 渲染背景
            guiDrawRRC2SetAlpha(log->backColor[msg->type], a);
            guiDrawRRC2SetModel(log->backColor[msg->type], model);
            guiDrawRRC2Render(log->backColor[msg->type]);
            // 渲染消息
            glm_vec4_copy((vec4){0.2f, 0.2f, 0.2f, a}, guiStrGetColor(msg->msg));
            guiStrSetModel(msg->msg, model);
            guiStrRender(msg->msg);
        }

        // 更新下一个位置
        glm_translate(model, (vec3){0.0f, LOG_HEIGH + 10.0f, 0.0f});
    }
}

void guiWidgetLogAddMsg(GUIid id, const wchar_t *msg, size_t type)
{
    // 初始化结构体
    GUIlogmsg logmsg;
    logmsg.str = wcsdup(msg);
    logmsg.type = type;

    // 添加进入B列表
    GUIwidgetLog *log = (GUIwidgetLog *)GUI_ID2WIDGET(id)->data1;
    pthread_mutex_lock(&log->mutex);
    listAddNodeInEnd(&log->listB, listDataToNode(listCreateNode(), &logmsg, sizeof(GUIlogmsg), true));
    // listAddNodeInEnd(&log->listA, listDataToNode(listCreateNode(), &logmsg, sizeof(GUIlogmsg), true));
    pthread_mutex_unlock(&log->mutex);
}

void freeLogmsg(void *arg)
{
    GUIlogmsg *msg = arg;
    guiStrDelete(msg->msg);
    free(msg);
}