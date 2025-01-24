#include "RDH.h"

// 内存对齐
#define RDH_MALLOC_SIZE(size) (((size) + 7) & ~7)

// 高低位掩码
#define RDH_IMG_MASK_HIGH 0xF8
#define RDH_IMG_MASK_LOW (~(RDH_IMG_MASK_HIGH))
#define RDH_IMG_MASK_SET(x, mask) ((x) & (mask))

// 获取高位的值
#define RDH_IMG_GET_HIGH(x) (RDH_IMG_MASK_SET((x), RDH_IMG_MASK_HIGH) >> 3)

// 图像数据位置
#define RDH_IMG_POS(img, w, x, y) ((img)[(y) * (w) + (x)])

// 图像哈希表
static uint8_t hash[4 * (RDH_IMG_GET_HIGH(RDH_IMG_MASK_HIGH) + 1)];
#define RDH_HASH_INIT() memset(hash, 0, sizeof(hash))
#define RDH_HASH_GET(x) (hash)[2 * (RDH_IMG_GET_HIGH(RDH_IMG_MASK_HIGH) + 1) + (x)]
#define RDH_HASH_SET(x) (RDH_HASH_GET(x)++)

void rdhShuffleImage(uint8_t *img, int size, uint64_t key)
{
    uint64_t *chunk = (uint64_t *)img;
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

void rdhUnshuffleImage(uint8_t *img, int size, uint64_t key)
{
    uint64_t *chunk = (uint64_t *)img;
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

void rdhSplitImage(const uint8_t *img, int size, uint8_t **img1, uint8_t **img2)
{
    *img1 = (uint8_t *)rdhMalloc(size);
    *img2 = (uint8_t *)rdhMalloc(size);

    const uint8_t *t = img;
    uint8_t *t1 = *img1;
    uint8_t *t2 = *img2;

    srand((unsigned int)time(NULL));
    xSrand8(rand());

    // 随机分割
    while (size--)
    {
        uint8_t r = xRand8();
        uint8_t high_masked_t = RDH_IMG_MASK_SET(*t, RDH_IMG_MASK_HIGH);
        uint8_t low_masked_t = RDH_IMG_MASK_SET(*t, RDH_IMG_MASK_LOW);
        uint8_t high_masked_r = RDH_IMG_MASK_SET(r, RDH_IMG_MASK_HIGH);
        uint8_t low_masked_r = RDH_IMG_MASK_SET(r, RDH_IMG_MASK_LOW);

        // 随机分割
        // *t1 = (high_masked_t - (high_masked_t ? (high_masked_r % high_masked_t) : 0)) |
        //       (low_masked_t - (low_masked_t ? (low_masked_r % low_masked_t) : 0));

        // 快速随机分割
        *t1 = (high_masked_t - RDH_IMG_MASK_SET(high_masked_r, high_masked_t)) |
              (low_masked_t - RDH_IMG_MASK_SET(low_masked_r, low_masked_t));

        *t2 = (high_masked_t - RDH_IMG_MASK_SET(*t1, RDH_IMG_MASK_HIGH)) |
              (low_masked_t - RDH_IMG_MASK_SET(*t1, RDH_IMG_MASK_LOW));

        t++;
        t1++;
        t2++;
    }
}
void rdhCombineImage(const uint8_t *img1, const uint8_t *img2, int size, uint8_t **img)
{
    *img = (uint8_t *)rdhMalloc(size);

    uint8_t *t, *t1, *t2;
    t = *img;
    t1 = (uint8_t *)img1;
    t2 = (uint8_t *)img2;

    // 合并
    while (size--)
    {
        *t = *t1 + *t2;

        t++;
        t1++;
        t2++;
    }
}

#define RDH_EMBED_ME_MASK_COUNT_SP 0xE0
#define RDH_EMBED_ME_MASK_COUNT_EP 0x0C
#define RDH_EMBED_ME_MASK_RDU_SP 0x10
#define RDH_EMBED_ME_MASK_RDU_EP 0x02
#define RDH_EMBED_ME_MASK_INUSE 0x01
#define RDH_EMBED_ME_OFFSET_COUNT_SP 5
#define RDH_EMBED_ME_OFFSET_COUNT_EP 2
#define RDH_EMBED_ME_OFFSET_RDU_SP 4
#define RDH_EMBED_ME_OFFSET_RDU_EP 1
#define RDH_EMBED_ME_OFFSET_INUSE 0

#define RDH_EMBED_ME_MASK_SET(x, mask) ((x) & (mask))
#define RDH_EMBED_ME_OFFSET_SET(x, offset) ((x) << (offset))
#define RDH_EMBED_ME_OFFSET_GET(x, offset) ((x) >> (offset))

#define RDH_EMBED_ME_GET(me, field) \
    RDH_EMBED_ME_OFFSET_GET(RDH_EMBED_ME_MASK_SET((me), RDH_EMBED_ME_MASK_##field), RDH_EMBED_ME_OFFSET_##field)

#define RDH_EMBED_ME_SET(value, field) \
    RDH_EMBED_ME_MASK_SET(RDH_EMBED_ME_OFFSET_SET((value), RDH_EMBED_ME_OFFSET_##field), RDH_EMBED_ME_MASK_##field)

#define RDH_EMBED_VALUE 0x08

typedef struct
{
    uint8_t EP[5];
    uint8_t SP[4];
} rdhChunk;
#define RDH_CHUNK_EP(chunk, i) ((chunk).EP[(i) + 1])
#define RDH_CHUNK_SP(chunk, i) ((chunk).SP[(i) + 1])

uint8_t rdhEmbedDataByte(uint8_t *img1Line1, uint8_t *img1Line2, uint8_t *img1Line3,
                         uint8_t *img2Line1, uint8_t *img2Line2, uint8_t *img2Line3,
                         uint8_t *byte, int total, int now, int *size)
{
    uint8_t m = 0;
    rdhChunk chunk[2];
    // 获取采样像素采样像素 SPs 和可嵌入像素 EPs
    RDH_CHUNK_SP(chunk[0], 1) = img1Line1[0];
    RDH_CHUNK_EP(chunk[0], 1) = img1Line1[1];
    RDH_CHUNK_SP(chunk[0], 2) = img1Line3[2];
    RDH_CHUNK_EP(chunk[0], 2) = img1Line3[0];
    RDH_CHUNK_EP(chunk[0], 3) = img1Line3[1];
    RDH_CHUNK_EP(chunk[0], 4) = img1Line3[2];
    RDH_CHUNK_SP(chunk[0], 3) = img1Line1[0];
    RDH_CHUNK_EP(chunk[0], 5) = img1Line1[1];
    RDH_CHUNK_SP(chunk[0], 4) = img1Line3[2];

    // 获取采样像素采样像素 SPs 和可嵌入像素 EPs的HSB值
    rdhChunk chunkHSB[2];
    RDH_CHUNK_SP(chunkHSB[0], 1) = RDH_IMG_GET_HIGH(img1Line1[0]);
    RDH_CHUNK_EP(chunkHSB[0], 1) = RDH_IMG_GET_HIGH(img1Line1[1]);
    RDH_CHUNK_SP(chunkHSB[0], 2) = RDH_IMG_GET_HIGH(img1Line3[2]);
    RDH_CHUNK_EP(chunkHSB[0], 2) = RDH_IMG_GET_HIGH(img1Line3[0]);
    RDH_CHUNK_EP(chunkHSB[0], 3) = RDH_IMG_GET_HIGH(img1Line3[1]);
    RDH_CHUNK_EP(chunkHSB[0], 4) = RDH_IMG_GET_HIGH(img1Line3[2]);
    RDH_CHUNK_SP(chunkHSB[0], 3) = RDH_IMG_GET_HIGH(img1Line1[0]);
    RDH_CHUNK_EP(chunkHSB[0], 5) = RDH_IMG_GET_HIGH(img1Line1[1]);
    RDH_CHUNK_SP(chunkHSB[0], 4) = RDH_IMG_GET_HIGH(img1Line3[2]);

    // 计算 dHSB
    int16_t dHSB[5];
    dHSB[0] = ((2 * EP1HSB[0] - (SP1HSB[0] + SP2HSB[0])) + (2 * EP1HSB[1] - (SP1HSB[1] + SP2HSB[1]))) / 2;
    dHSB[1] = ((2 * EP2HSB[0] - (SP1HSB[0] + SP3HSB[0])) + (2 * EP2HSB[1] - (SP1HSB[1] + SP3HSB[1]))) / 2;
    dHSB[2] = ((4 * EP3HSB[0] - (SP1HSB[0] + SP2HSB[0] + SP3HSB[0] + SP4HSB[0])) + (4 * EP3HSB[1] - (SP1HSB[1] + SP2HSB[1] + SP3HSB[1] + SP4HSB[1]))) / 4;
    dHSB[3] = ((2 * EP4HSB[0] - (SP2HSB[0] + SP4HSB[0])) + (2 * EP4HSB[1] - (SP2HSB[1] + SP4HSB[1]))) / 2;
    dHSB[4] = ((2 * EP5HSB[0] - (SP3HSB[0] + SP4HSB[0])) + (2 * EP5HSB[1] - (SP3HSB[1] + SP4HSB[1]))) / 2;

    // 计算Me1
    int16_t Me1 = dHSB[0];
    RDH_HASH_INIT();
    for (int i = 0; i < 5; i++) // 计算哈希表
    {
        RDH_HASH_SET(dHSB[i]);
    }
    for (int i = 1; i < 5; i++) // 计算出现最多次的值，如果出现次数相同则取最大值
    {
        if (RDH_HASH_GET(Me1) < RDH_HASH_GET(dHSB[i]) ||
            (RDH_HASH_GET(Me1) == RDH_HASH_GET(dHSB[i]) && Me1 < dHSB[i]))
            Me1 = dHSB[i];
    }

    // 计算sdHSB
    int16_t sdHSB[4];
    sdHSB[0] = (SP1[0] - EP1[0]) + (SP1[1] - EP1[1]);
    sdHSB[1] = (SP2[0] - EP4[0]) + (SP2[1] - EP4[1]);
    sdHSB[2] = (SP3[0] - EP2[0]) + (SP3[1] - EP2[1]);
    sdHSB[3] = (SP4[0] - EP5[0]) + (SP4[1] - EP5[1]);

    // 嵌入数据
    for (int i = 0; i < 4; i++)
    {
        if (dHSB[i] > 0xFF - 0x08) // 检查是否存在溢出
        {
            return 0;
        }
        else if (dHSB[i] == Me1) // 峰值
        {

        }
        else if (dHSB[i] > Me1) // 大于峰值
        {
        }
        else // 小于峰值
        {
        }
    }

    // 计算Me2
    int16_t Me2 = sdHSB[0];
    RDH_HASH_INIT();
    for (int i = 0; i < 4; i++) // 计算哈希表
    {
        RDH_HASH_SET(sdHSB[i]);
    }
    for (int i = 1; i < 4; i++) // 计算出现最多次的值，如果出现次数相同则取最大值
    {
        if (RDH_HASH_GET(Me2) < RDH_HASH_GET(sdHSB[i]) ||
            (RDH_HASH_GET(Me2) == RDH_HASH_GET(sdHSB[i]) && Me2 < sdHSB[i]))
            Me2 = sdHSB[i];
    }

    // DEBUG
    printf("|----------img1----------|\n");
    printf("%3d %3d %3d\n", SP1[0], EP1[0], SP2[0]);
    printf("%3d %3d %3d\n", EP2[0], EP3[0], EP4[0]);
    printf("%3d %3d %3d\n", SP3[0], EP5[0], SP4[0]);
    printf("|----------img2----------|\n");
    printf("%3d %3d %3d\n", SP1[1], EP1[1], SP2[1]);
    printf("%3d %3d %3d\n", EP2[1], EP3[1], EP4[1]);
    printf("%3d %3d %3d\n", SP3[1], EP5[1], SP4[1]);
    printf("|----------dHSB----------|\n");
    printf("dHSB1 = %3d\ndHSB2 = %3d\ndHSB3 = %3d\ndHSB4 = %3d\ndHSB5 = %3d\n", dHSB[0], dHSB[1], dHSB[2], dHSB[3], dHSB[4]);
    printf("max = %d\n", Me1);
    printf("|----------sdHSB----------|\n");
    printf("sdHSB1 = %3d\nsdHSB2 = %3d\nsdHSB3 = %3d\nsdHSB4 = %3d\n", sdHSB[0], sdHSB[1], sdHSB[2], sdHSB[3]);
    printf("max = %d\n", Me2);
}

uint8_t *rdhEmbedData(uint8_t *img1, uint8_t *img2, int w, int h, uint8_t *str, int size)
{
    uint8_t *m;
    m = (uint8_t *)rdhMalloc((w / 3) * (h / 3));
    // for (int i = 0; i < w; i += 3)
    // {
    //     for (int j = 0; j < h; j += 3)
    //     {
    int i = rand() % (w - 3);
    int j = rand() % (h - 3);
    if (1)
    {
        if (1)
        {
            // printf("x: %d, y: %d\n", i, j);

            // uint8_t SP1 = RDH_IMG_GET_HIGH(RDH_IMG_POS(img, w, i + 0, j + 0));
            // uint8_t EP1 = RDH_IMG_GET_HIGH(RDH_IMG_POS(img, w, i + 1, j + 0));
            // uint8_t SP2 = RDH_IMG_GET_HIGH(RDH_IMG_POS(img, w, i + 2, j + 0));
            // uint8_t EP2 = RDH_IMG_GET_HIGH(RDH_IMG_POS(img, w, i + 0, j + 1));
            // uint8_t EP3 = RDH_IMG_GET_HIGH(RDH_IMG_POS(img, w, i + 1, j + 1));
            // uint8_t EP4 = RDH_IMG_GET_HIGH(RDH_IMG_POS(img, w, i + 2, j + 1));
            // uint8_t SP3 = RDH_IMG_GET_HIGH(RDH_IMG_POS(img, w, i + 0, j + 2));
            // uint8_t EP5 = RDH_IMG_GET_HIGH(RDH_IMG_POS(img, w, i + 1, j + 2));
            // uint8_t SP4 = RDH_IMG_GET_HIGH(RDH_IMG_POS(img, w, i + 2, j + 2));

            // int8_t dHSB[5];
            // dHSB[0] = 2 * EP1 - (SP1 + SP2);
            // dHSB[1] = 2 * EP2 - (SP1 + SP3);
            // dHSB[2] = 4 * EP3 - (SP1 + SP2 + SP3 + SP4);
            // dHSB[3] = 2 * EP4 - (SP2 + SP4);
            // dHSB[4] = 2 * EP5 - (SP3 + SP4);

            // RDH_HASH_INIT();
            // RDH_HASH_SET(dHSB[0]);
            // RDH_HASH_SET(dHSB[1]);
            // RDH_HASH_SET(dHSB[2]);
            // RDH_HASH_SET(dHSB[3]);
            // RDH_HASH_SET(dHSB[4]);

            // int8_t dHSBMax = dHSB[0];
            // if (RDH_HASH_GET(dHSBMax) <= RDH_HASH_GET(dHSB[1]) &&
            //     (RDH_HASH_GET(dHSBMax) < RDH_HASH_GET(dHSB[1]) || dHSBMax < dHSB[1]))
            //     dHSBMax = dHSB[1];
            // if (RDH_HASH_GET(dHSBMax) <= RDH_HASH_GET(dHSB[2]) &&
            //     (RDH_HASH_GET(dHSBMax) < RDH_HASH_GET(dHSB[2]) || dHSBMax < dHSB[2]))
            //     dHSBMax = dHSB[2];
            // if (RDH_HASH_GET(dHSBMax) <= RDH_HASH_GET(dHSB[3]) &&
            //     (RDH_HASH_GET(dHSBMax) < RDH_HASH_GET(dHSB[3]) || dHSBMax < dHSB[3]))
            //     dHSBMax = dHSB[3];
            // if (RDH_HASH_GET(dHSBMax) <= RDH_HASH_GET(dHSB[4]) &&
            //     (RDH_HASH_GET(dHSBMax) < RDH_HASH_GET(dHSB[4]) || dHSBMax < dHSB[4]))
            //     dHSBMax = dHSB[4];

            // DEBUG("SP1: %3d, EP1: %3d, SP2: %3d\nEP2: %3d, EP3: %3d, EP4: %3d\nSP3: %3d, EP5: %3d, SP4: %3d\n\n", SP1, EP1, SP2, EP2, EP3, EP4, SP3, EP5, SP4);
            // DEBUG("dHSB1: %3d\ndHSB2: %3d\ndHSB3: %3d\ndHSB4: %3d\ndHSB5: %3d\n", dHSB[0], dHSB[1], dHSB[2], dHSB[3], dHSB[4]);
            // DEBUG("dHSBMax: %3d\n\n", dHSBMax);

            // int16_t sdHSB[4];
            // sdHSB[0] = SP1 - EP1;
            // sdHSB[1] = SP2 - EP4;
            // sdHSB[2] = SP3 - EP2;
            // sdHSB[3] = SP4 - EP5;

            // RDH_HASH_INIT();
            // RDH_HASH_SET(sdHSB[0]);
            // RDH_HASH_SET(sdHSB[1]);
            // RDH_HASH_SET(sdHSB[2]);
            // RDH_HASH_SET(sdHSB[3]);

            // int16_t sdHSBMax = sdHSB[0];
            // if (RDH_HASH_GET(sdHSBMax) <= RDH_HASH_GET(sdHSB[1]) &&
            //     (RDH_HASH_GET(sdHSBMax) < RDH_HASH_GET(sdHSB[1]) || sdHSBMax < sdHSB[1]))
            //     sdHSBMax = sdHSB[1];
            // if (RDH_HASH_GET(sdHSBMax) <= RDH_HASH_GET(sdHSB[2]) &&
            //     (RDH_HASH_GET(sdHSBMax) < RDH_HASH_GET(sdHSB[2]) || sdHSBMax < sdHSB[2]))
            //     sdHSBMax = sdHSB[2];
            // if (RDH_HASH_GET(sdHSBMax) <= RDH_HASH_GET(sdHSB[3]) &&
            //     (RDH_HASH_GET(sdHSBMax) < RDH_HASH_GET(sdHSB[3]) || sdHSBMax < sdHSB[3]))
            //     sdHSBMax = sdHSB[3];

            // DEBUG("sdHSB1: %3d\nsdHSB2: %3d\nsdHSB3: %3d\nsdHSB4: %3d\n", sdHSB[0], sdHSB[1], sdHSB[2], sdHSB[3]);
            // DEBUG("sdHSBMax: %3d\n\n", sdHSBMax);
        }
    }
}

void *rdhMalloc(size_t size)
{
    return malloc(RDH_MALLOC_SIZE(size));
}
void rdhFree(void *data)
{
    free(data);
}