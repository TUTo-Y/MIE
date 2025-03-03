/**
 * \file vector.h
 * \brief 有序数组
 */
#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "log.h"

#define VECTOR_ADD_SIZE 0x8

// 动态数组
typedef struct
{
    unsigned char *data; // 数据
    size_t step;         // 步长

    size_t index; // 数组大小，下一个元素的位置
    size_t total; // 数组容量，总大小

    int (*compare)(const void *src, const void *dic); // 比较函数
} Vector;

/**
 * \brief 初始化一个有序数组
 * \param v 数组
 * \param step 步长
 * \param n 数组大小
 * \param compare 比较函数
 */
void vectorInit(Vector *v, size_t step, size_t n, int (*compare)(const void *src, const void *dic));

/**
 * \brief 添加数据
 * \param v 数组
 * \param data 数据
 */
void vectorAdd(Vector *v, const void *data);

/**
 * \brief 处理每个元素
 * \param v 数组
 * \param func 处理函数
 */
void vectorEach(Vector *v, void (*func)(void *data));

/**
 * \brief 查找元素，二分法
 * \param v 数组
 * \param data 数据
 * \param find 查找函数，返回值小于0表示src小于dic，等于0表示相等，大于0表示src大于dic
 * \return 元素索引，如果没有找到，返回index的值
 */
size_t vectorFind(Vector *v, const void *data, int (*find)(const void *src, const void *dic));

/**
 * \brief 查找元素，直线查找
 * \param v 数组
 * \param data 数据
 * \param find 查找函数
 * \return 元素索引，如果没有找到，返回index的值
 */
size_t vectorFindSimple(Vector *v, const void *data, bool (*find)(const void *src, const void *dic));

/**
 * \brief 检查索引是否找到
 * \param v 数组
 * \param index 索引
 */
static inline bool vectorCheckFind(Vector *v, size_t index)
{
    return (index < v->index);
}

/**
 * \brief 获取元素
 * \param v 数组
 * \param index 索引
 * \return 元素
 */
static inline void *vectorGet(Vector *v, size_t index)
{
    return v->data + index * v->step;
}

/**
 * \brief 排序数组
 * \param v 数组
 * \note 使用快速排序
 */
static inline void vectorSort(Vector *v)
{
    qsort(v->data, v->index, v->step, v->compare);
}

/**
 * \brief 删除数组元素
 * \param v 数组
 * \param index 索引
 */
void vectorDelete(Vector *v, size_t index);

/**
 * \brief 释放数组
 * \param v 数组
 */
static inline void vectorDestroy(Vector *v)
{
    free(v->data);
}

#endif // VECTOR_H