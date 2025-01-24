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
#include "log.h"

/**
 * \brief 使用洗牌算法打乱和恢复图像数据
 * \param img 图像数据
 * \param size 数据大小
 * \param key 随机数种子
 */
void rdhShuffleImage(uint8_t *img, int size, uint64_t key);
void rdhUnshuffleImage(uint8_t *img, int size, uint64_t key);

/**
 * \brief 将图像随机分成加密份额1和加密份额2
 * \param img 图像数据
 * \param size 数据大小
 * \param img1 加密份额1
 * \param img2 加密份额2
 */
void rdhSplitImage(const uint8_t *img, int size, uint8_t **img1, uint8_t **img2);
void rdhCombineImage(const uint8_t *img1, const uint8_t *img2, int size, uint8_t **img);


uint8_t rdhEmbedDataByte(uint8_t *img1Line1, uint8_t *img1Line2, uint8_t *img1Line3,
                         uint8_t *img2Line1, uint8_t *img2Line2, uint8_t *img2Line3,
                         uint8_t *byte, int total, int now, int *size);

/**
 * \brief 嵌入数据
 * \param img1 图像份额1
 * \param img2 图像份额1
 * \param w 宽度
 * \param h 高度
 * \param str 需要嵌入的数据
 * \param size 嵌入的数据大小
 */
uint8_t *rdhEmbedData(uint8_t *img1, uint8_t *img2, int w, int h, uint8_t *str, int size);

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