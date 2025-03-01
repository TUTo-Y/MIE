#include "resource.h"

typedef struct
{
    char *fileName;
    void *data;
    size_t size;
} resItem;

list resList; // 资源列表

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

void resInit()
{
    // 初始化资源目录
    listInitList(&resList);
}

uint8_t *resGetFile(const char *fileName, uint8_t **data, size_t *size, bool live)
{
    if (fileName == NULL)
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
            if (data)
                *data = item->data;
            if (size)
                *size = item->size;

            // // 如果不常驻内存，删除节点
            // if (live == false)
            //     listDeleteNode(&resList, node, free); // 不删除数据，只删除节点
            return item->data;
        }
        node = node->fd;
    }

    // 如果没有加载，加载文件
    char path[256] = {0};
    strncpy(path, fileName, sizeof(path) - strlen(path) - 1);

    FILE *fp = fopen(path, "rb");
    if (fp == NULL)
    {
        ERROR("资源文件 %s 不存在\n", fileName);
        return NULL;
    }

    // 获取文件大小
    fseek(fp, 0L, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    // 读取文件
    char *fileData = malloc(fileSize + 1);
    fread(fileData, 1, fileSize, fp);
    fileData[fileSize] = '\0';

    // 关闭文件
    fclose(fp);

    // 拷贝数据
    if (data)
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
    
    return fileData;
}

void resQuit(void)
{
    listDeleteList(&resList, (void (*)(void *))resDeleteItem);
}