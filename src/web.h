#ifndef WEB_H
#define WEB_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "log.h"
#include "config.h"
#include "enc.h"
#include "list.h"
#include "gui_base.h"

typedef uint64_t FLAG;
enum
{
    WEB_MSG_SUCESS,     // 成功
    WEB_MSG_ERROR,      // 错误
    WEB_MSG_DISCONNECT, // 断开连接

    WEB_MSG_CPUBLIC_KEY, // 发送公钥

    WEB_MSG_LOGIN,            // 登录
    WEB_MSG_LOGIN_DOCTOR,     // 登录医生
    WEB_MSG_LOGIN_PATIENT,    // 登录患者
    WEB_MSG_REGISTER,         // 注册
    WEB_MSG_REGISTER_DOCTOR,  // 注册医生
    WEB_MSG_REGISTER_PATIENT, // 注册患者

    WEB_MSG_LOGIN_ERROR_USER, // 登录用户不存在
    WEB_MSG_LOGIN_ERROR_PASS, // 登录密码错误
    WEB_MSG_LOGIN_ERROR_REPE, // 重复登录

    WEB_MSG_PAITENT_UPLOAD, // 患者上传图像及其数据
    WEB_MSG_PAITENT_DOWNLOAD // 患者上传图像及其数据成功
};

// 任务
enum WEB_TASK
{
    WEB_TASK_SEND, // 发送任务
    WEB_TASK_RECV, // 接收任务
    WEB_TASK_EXIT  // 退出任务
};
typedef struct
{
    enum WEB_TASK TaskFlag; // 线程需要处理的任务标识

    void (*func)(SOCKET fd, size_t flag, char *data);
    size_t flag;
    char *data;
} WEBtask;

// // 发送线程
// extern struct
// {
//     pthread_t thread;      // 线程
//     list task;             // 任务
//     sem_t sem;             // 信号量
//     pthread_mutex_t mutex; // 互斥锁
// } WebTaskSend;

extern SOCKET sockfd;     // 服务器套接字
extern SM2_KEY keyClient; // 客户端密钥
extern SM2_KEY keyServer; // 服务器密钥

void webInit();
void webQuit();

/**
 * \brief 添加web任务
 * \param TaskFlag 任务标志
 * \param func 函数
 * \param flag 标志
 * \param data 数据
 */
void webAddTask(enum WEB_TASK TaskFlag, void (*func)(SOCKET fd, size_t flag, char *data), size_t flag, char *data);

/**
 * \brief 发送flag
 * \param fd 文件描述符
 * \param flag 标志
 */
static inline void webSendFlag(SOCKET fd, FLAG flag)
{
    send(fd, (const char *)&flag, sizeof(FLAG), 0);
}

/**
 * \brief 接受flag
 * \param fd 文件描述符
 * \return 标志
 */
static inline FLAG webRecvFlag(SOCKET fd)
{
    FLAG flag;
    int ret = recv(fd, (char *)&flag, sizeof(FLAG), MSG_WAITALL);
    if (ret <= 0)
        flag = WEB_MSG_DISCONNECT;
    return flag;
}

/**
 * \brief 发送数据
 * \param data 数据
 * \param size 数据大小
 */
static inline void webSendData(SOCKET fd, const char *data, size_t size)
{
    // 发送长度
    send(fd, (const char *)&size, sizeof(size), 0);
    // 发送大小
    send(fd, (const char *)data, size, 0);
}

/**
 * \brief 接收数据
 * \param data 数据
 * \param size 数据大小
 */
static inline char *webRecvData(SOCKET fd, char **data, size_t *size)
{
    size_t s;
    char *d = NULL;

    // 接收长度
    recv(fd, (char *)&s, sizeof(size_t), MSG_WAITALL);

    // 接收数据
    d = (uint8_t *)malloc(s);
    recv(fd, (char *)d, s, MSG_WAITALL);

    // 赋值
    if (data)
        *data = d;
    if (size)
        *size = s;

    return d;
}
#endif // WEB_H