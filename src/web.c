#include "web.h"

struct
{
    pthread_t thread;      // 线程
    list task;             // 任务
    sem_t sem;             // 信号量
    pthread_mutex_t mutex; // 互斥锁
} WebTaskSend;

SOCKET sockfd;
SM2_KEY keyClient;
SM2_KEY keyServer;

static void *webSendThread(void *)
{
    while (sem_wait(&WebTaskSend.sem) == 0)
    {
        // 获取任务
        pthread_mutex_lock(&WebTaskSend.mutex);
        list *node = listGetNodeFromStart(&WebTaskSend.task);
        WEBtask *task = (WEBtask *)node->data;
        listDeleteNode(NULL, node, NULL);
        pthread_mutex_unlock(&WebTaskSend.mutex);

        // 处理任务
        switch (task->TaskFlag)
        {
        case WEB_TASK_SEND:
        case WEB_TASK_RECV:
            CALL(task->func, sockfd, task->flag, task->data);
            free(task);
            break;

        case WEB_TASK_EXIT:
            free(task);
            return NULL;
        default:
            ERR("未知任务");
            break;
        }
    }
    return NULL;
}

void webAddTask(enum WEB_TASK TaskFlag, void (*func)(SOCKET fd, size_t flag, char *data), size_t flag, char *data)
{
    // 创建任务
    WEBtask task;
    task.TaskFlag = TaskFlag;
    task.func = func;
    task.flag = flag;
    task.data = data;

    // 添加任务
    pthread_mutex_lock(&WebTaskSend.mutex);
    listAddNodeInEnd(&WebTaskSend.task, listDataToNode(listCreateNode(), &task, sizeof(WEBtask), true));
    pthread_mutex_unlock(&WebTaskSend.mutex);

    // 通知线程
    sem_post(&WebTaskSend.sem);
}

void webInit()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
        perror("WSAStartup failed");

    // 连接用户
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        perror("socket失败");
        exit(-1);
    }
    // 设置服务器地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(config.server_port));
    addr.sin_addr.S_un.S_addr = inet_addr(config.server_ip);
    // 连接服务器
    iResult = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (iResult == SOCKET_ERROR)
    {
        perror("connect失败");
        exit(-1);
    }

    // 获取服务器的公钥
    FILE *fp = fopen(config.public_key, "r");
    sm2_public_key_info_from_pem(&keyServer, fp);
    fclose(fp);

    // 生成客户端公钥
    sm2_key_generate(&keyClient);

    // 生成公钥PEM编码
    uint8_t *public_pem = NULL;
    size_t public_pem_len = 0;
    encSM2PublicSave(&keyClient, &public_pem, &public_pem_len);

    // 发送公钥
    ENCkem kem = encKEMEnc(public_pem, public_pem_len, &keyServer);
    webSendFlag(sockfd, WEB_MSG_CPUBLIC_KEY);
    webSendData(sockfd, (const char *)kem, encKEMSizeKEM(kem));

    // 连接成功
    SUCESS("(%s:%s)连接服务器成功: \n", config.server_ip, config.server_port);
    encKEMFree(kem);
    free(public_pem);

    // 创建线程
    listInitList(&WebTaskSend.task);
    sem_init(&WebTaskSend.sem, 0, 0);
    pthread_mutex_init(&WebTaskSend.mutex, NULL);
    pthread_create(&WebTaskSend.thread, NULL, webSendThread, NULL);
}
void webQuit()
{
    // 通知线程退出
    webAddTask(WEB_TASK_EXIT, NULL, 0, NULL);
    pthread_join(WebTaskSend.thread, NULL);
    listDeleteList(&WebTaskSend.task, free);
    sem_destroy(&WebTaskSend.sem);
    pthread_mutex_destroy(&WebTaskSend.mutex);

    // 发送断连flag
    webSendFlag(sockfd, WEB_MSG_DISCONNECT);
    closesocket(sockfd);

    WSACleanup();
}