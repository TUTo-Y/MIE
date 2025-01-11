#include "resource.h"

typedef struct
{
    char *fileName;
    void *data;
    size_t size;
} resItem;

list resList;

/**
 * \brief 删除resItem
 */
void resDeleteItem(resItem *res)
{
    if (res == NULL)
        return;

    if (res->data)
        free(res->data);
    if (res->fileName)
        free(res->fileName);

    free(res);
}

/**
 * \brief 初始化资源列表
 */
void resInit(void)
{
    listInitList(&resList);
}

/**
 * \brief 加载资源文件
 * \param fileName 文件名
 * \param data 数据
 * \param size 大小
 * \param live 是否常驻内存(只有在第一次加载时有效)
 * \return 是否成功
 */
bool resGetFile(const char *fileName, void **data, size_t *size, bool live)
{
    if (fileName == NULL || data == NULL)
        return false;

    // 检查文件是否存在资源列表中
    list *node = resList.fd;
    while (node != &resList)
    {
        // 检查文件名
        resItem *item = (resItem *)node->data;
        if (strcmp(item->fileName, fileName) == 0)
        {
            // 如果已经加载，返回数据
            *data = item->data;
            if (size)
                *size = item->size;

            // // 如果不常驻内存，删除节点
            // if (live == false)
            //     listDeleteNode(&resList, node, free); // 不删除数据，只删除节点
            return true;
        }
        node = node->fd;
    }

    // 如果没有加载，加载文件
    char path[256] = {0};
    strcpy(path, RESOURCE_DIR);
    strcat(path, fileName);

    FILE *fp = fopen(path, "rb");
    if (fp == NULL)
    {
        ERROR("资源文件 %s 不存在\n", fileName);
        return false;
    }

    // 获取文件大小
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 读取文件
    char *fileData = malloc(fileSize + 1);
    fread(fileData, 1, fileSize, fp);
    fileData[fileSize] = '\0';

    // 关闭文件
    fclose(fp);

    // 拷贝数据
    *data = fileData;
    if (size)
        *size = fileSize;

    // 如果常驻内存，添加到资源列表
    if (live == true)
    {
        resItem *item = malloc(sizeof(resItem));
        item->fileName = strdup(fileName);
        item->data = fileData;
        item->size = fileSize;

        listAddNodeInStart(&resList, listDataToNode(listCreateNode(), item, 0, false));
    }

    return true;
}

/**
 * \brief 删除资源列表
 */
void resQuit(void)
{
    listDeleteList(&resList, (void (*)(void *))resDeleteItem);
}