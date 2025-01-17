#include "RDH.h"

#define RDH_SPLIT_MALLOC_SIZE(size) (((size) + 7) & ~7)
#define RDH_SPLIT_MASK_H 0xF8
#define RDH_SPLIT_MASK_L (~(RDH_SPLIT_MASK_H))
#define RDH_SPLIT_SET_MASK(x, mask) ((x) & (mask))

void rdhShuffleImage(uint8_t *data, int size, uint64_t key)
{
    uint64_t *chunk = (uint64_t *)data;
    size /= sizeof(uint64_t) / sizeof(uint8_t);
    xSrand32(key);
    for (int i = size - 1; i > 0; i--)
    {
        int j = xRand32() % (i + 1);
        uint64_t temp = chunk[i];
        chunk[i] = chunk[j];
        chunk[j] = temp;
    }
}

void rdhUnshuffleImage(uint8_t *data, int size, uint64_t key)
{
    uint64_t *chunk = (uint64_t *)data;
    size /= sizeof(uint64_t) / sizeof(uint8_t);
    xSrand32(key);
    int *indices = (int *)malloc(size * sizeof(int));
    uint64_t *tempData = (uint64_t *)malloc(size * sizeof(uint64_t));

    // 初始化索引数组
    for (int i = 0; i < size; i++)
    {
        indices[i] = i;
    }

    // 使用Fisher-Yates算法打乱索引数组
    for (int i = size - 1; i > 0; i--)
    {
        int j = xRand32() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // 根据打乱的索引数组重新排列数据
    for (int i = 0; i < size; i++)
    {
        tempData[indices[i]] = chunk[i];
    }

    // 将恢复后的数据复制回原数组
    memcpy(chunk, tempData, size * sizeof(uint64_t));

    free(indices);
    free(tempData);
}

void rdhSplitImage(const uint8_t *data, int size, uint8_t **data1, uint8_t **data2)
{
    *data1 = (uint8_t *)rdhMalloc(size);
    *data2 = (uint8_t *)rdhMalloc(size);

    const uint8_t *t = data;
    uint8_t *t1 = *data1;
    uint8_t *t2 = *data2;

    srand((unsigned int)time(NULL));
    xSrand8(rand());

    // 随机分割
    while (size--)
    {
        uint8_t r = xRand8();
        uint8_t high_masked_t = RDH_SPLIT_SET_MASK(*t, RDH_SPLIT_MASK_H);
        uint8_t low_masked_t = RDH_SPLIT_SET_MASK(*t, RDH_SPLIT_MASK_L);
        uint8_t high_masked_r = RDH_SPLIT_SET_MASK(r, RDH_SPLIT_MASK_H);
        uint8_t low_masked_r = RDH_SPLIT_SET_MASK(r, RDH_SPLIT_MASK_L);

        *t1 = (high_masked_t - (high_masked_t ? (high_masked_r % high_masked_t) : 0)) |
              (low_masked_t - (low_masked_t ? (low_masked_r % low_masked_t) : 0));
        *t2 = (high_masked_t - RDH_SPLIT_SET_MASK(*t1, RDH_SPLIT_MASK_H)) |
              (low_masked_t - RDH_SPLIT_SET_MASK(*t1, RDH_SPLIT_MASK_L));

        t++;
        t1++;
        t2++;
    }
}
void rdhCombineImage(const uint8_t *data1, const uint8_t *data2, int size, uint8_t **data)
{
    *data = (uint8_t *)rdhMalloc(size);

    uint8_t *t, *t1, *t2;
    t = *data;
    t1 = (uint8_t *)data1;
    t2 = (uint8_t *)data2;

    // 合并
    while (size--)
    {
        *t = *t1 + *t2;

        t++;
        t1++;
        t2++;
    }
}

void *rdhMalloc(size_t size)
{
    return malloc(RDH_SPLIT_MALLOC_SIZE(size));
}
void rdhFree(void *data)
{
    free(data);
}