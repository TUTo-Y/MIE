#include "vector.h"

// 扩展数组，在添加元素前调用
void vectorExpand(Vector *v)
{
    if (v->index < v->total)
        return;

    v->total += VECTOR_ADD_SIZE;
    v->data = (unsigned char *)realloc(v->data, v->total * v->step);
    memset(v->data + v->index * v->step, 0, VECTOR_ADD_SIZE * v->step);
}

void vectorInit(Vector *v, size_t step, size_t n, int (*compare)(const void *src, const void *dic))
{
    v->step = step;
    v->index = 0;
    v->total = n;
    v->compare = compare;
    v->data = (unsigned char *)malloc(step * n);
    memset(v->data, 0, step * n);
}

void vectorAdd(Vector *v, const void *data)
{
    vectorExpand(v);

    // 如果没有提供比较函数，直接添加数据
    if (v->compare == NULL)
    {
        memcpy(v->data + v->index * v->step, data, v->step);
        v->index++;
        return;
    }

    // 二分法寻找插入点
    size_t left = 0;
    size_t right = v->index;
    size_t mid = 0;
    while (left < right)
    {
        mid = (left + right) / 2;
        if (v->compare(data, v->data + mid * v->step) < 0)
            right = mid;
        else
            left = mid + 1;
    }

    // 插入数据
    if (left < v->index)
        memmove(v->data + (left + 1) * v->step, v->data + left * v->step, (v->index - left) * v->step);
    memcpy(v->data + left * v->step, data, v->step);

    // 更新index
    v->index++;
}

void vectorEach(Vector *v, void (*func)(void *data))
{
    for (size_t i = 0; i < v->index; i++)
    {
        func(v->data + i * v->step);
    }
}

size_t vectorFind(Vector *v, const void *data, int (*find)(const void *src, const void *dic))
{
    // 如果没有提供查找函数，直接返回第一个数据
    if (find == NULL)
        return 0;

    // 二分法查找
    size_t left = 0;
    size_t right = v->index;
    size_t mid = 0;
    while (left < right)
    {
        mid = (left + right) / 2;
        int ret = find(data, v->data + mid * v->step);
        if (ret == 0)
            return mid;
        else if (ret < 0)
            right = mid;
        else
            left = mid + 1;
    }

    return v->index;
}


size_t vectorFindSimple(Vector *v, const void *data, bool (*find)(const void *src, const void *dic))
{
    // 如果没有提供查找函数，直接返回第一个数据
    if (find == NULL)
        return 0;

    // 直线查找
    for (size_t i = 0; i < v->index; i++)
    {
        if (find(data, v->data + i * v->step))
            return i;
    }

    return v->index;
}

void vectorDelete(Vector *v, size_t index)
{
    if (index >= v->index)
        return;

    memmove(v->data + index * v->step, v->data + (index + 1) * v->step, (v->index - index - 1) * v->step);
    v->index--;
}