/**
 * \file resource.h
 * \brief 加载和读取资源文件
 * 
 * // 初始化资源，只需要调用一次即可
 * resInit();
 * 
 * // 加载资源文件
 * void *data = NULL;
 * size_t size = 0;
 * bool ret = resGetFile("test.txt", &data, &size, true);
 * 
 * // 释放资源
 * resQuit();
 */
#ifndef RESOURCE_H
#define RESOURCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "log.h"
#include "list.h"
#include "config.h"

/**
 * \brief 初始化资源列表
 * \param resourceDir 资源目录
 */
void resInit();

/**
 * \brief 加载资源文件
 * \param fileName 资源文件名
 * \param data 数据
 * \param size 大小
 * \param live 是否常驻内存(只有在第一次加载时有效)
 * \return 是否成功
 */
uint8_t *resGetFile(const char *fileName, uint8_t **data, size_t *size, bool live);

/**
 * \brief 删除资源列表
 */
void resQuit(void);

#endif // RESOURCE_H
