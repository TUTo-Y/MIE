/**
 * @file RDH.h
 * @brief RDH - EI
 */
#ifndef RDH_H
#define RDH_H

#define _CRT_RAND_S
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rand.h"

/**
 * \brief 使用洗牌算法打乱和恢复图像数据
 * \param data 图像数据
 * \param size 数据大小
 * \param key 随机数种子
 */
void rdhShuffleImage(uint8_t *data, int size, uint64_t key);
void rdhUnshuffleImage(uint8_t *data, int size, uint64_t key);

/**
 * \brief 将图像随机分成加密份额1和加密份额2
 * \param data 图像数据
 * \param size 数据大小
 * \param data1 加密份额1
 * \param data2 加密份额2
 */
void rdhSplitImage(const uint8_t *data, int size, uint8_t **data1, uint8_t **data2);
void rdhCombineImage(const uint8_t *data1, const uint8_t *data2, int size, uint8_t **data);


/**
 * \brief 分配空间
 * \param 大小
 */
void* rdhMalloc(size_t size);
/**
 * \brief 释放空间
 * \param data 数据
 */
void rdhFree(void *data);

#endif // RDH_H