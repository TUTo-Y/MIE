#include "gui_ttf.h"

#define GUI_TTF_MAX_ADD 128 // 每次添加文本数量

struct
{
    stbtt_fontinfo font; // 字体信息

    int textHigh; // 高度

    GUIchar *textRend; // TTF字体列表
    wchar_t *textList; // 文本列表

    int textCount; // 文本数量
    int textMax;   // 最大文本数量
} GUIfont = {0};

/**
 * \brief 初始化TTF字体
 * \param fontData TTF字体数据
 */
void guiTTFInit(const unsigned char *fontData)
{
    // 初始化字体
    stbtt_InitFont(&GUIfont.font, fontData, stbtt_GetFontOffsetForIndex(fontData, 0));

    // 获取字体高度
    int ascent = 0;  // 上升
    int descent = 0; // 下降
    int lineGap = 0; // 行间距
    stbtt_GetFontVMetrics(&GUIfont.font, &ascent, &descent, &lineGap);
    GUIfont.textHigh = ascent - descent + lineGap;

    // 初始化文本列表
    GUIfont.textMax = GUI_TTF_MAX_ADD;
    GUIfont.textCount = 0;
    GUIfont.textList = (wchar_t *)malloc(sizeof(wchar_t) * GUIfont.textMax);
    GUIfont.textRend = (GUIchar *)malloc(sizeof(GUIchar) * GUIfont.textMax);

    // 清空文本列表
    memset(GUIfont.textList, 0, sizeof(wchar_t) * GUIfont.textMax);
    memset(GUIfont.textRend, 0, sizeof(GUIchar) * GUIfont.textMax);
}

/**
 * \brief 清除TTF缓存
 */
void guiTTFQuit()
{
}

/**
 * \brief 渲染一个文字的顶点
 * \param text 文本内容
 * \return 渲染文本
 */
GUIchar *guiTTFGetChar(const wchar_t text)
{
    // 在列表中查找
    wchar_t *w = wcschr(GUIfont.textList, text);
    if (w != NULL)
    {
        return &GUIfont.textRend[w - GUIfont.textList];
    }

    // 创建新文本
    GUIfont.textList[GUIfont.textCount] = text;
    GUIchar *ttf = &GUIfont.textRend[GUIfont.textCount];
    ttf->c = text;
    stbtt_GetCodepointHMetrics(&GUIfont.font, text, &ttf->advance, &ttf->advance);

    /* 渲染新文本 */
    stbtt_vertex *stbVertex = NULL;
    int verCount = stbtt_GetCodepointShape(&GUIfont.font, text, &stbVertex);

    /* 点控制 */
    int pointMax = verCount;
    int pointCount = 0;
    float *point = (float *)malloc(sizeof(float) * pointMax * 2);

    /* VBO间隔控制 */
    int VBOmax = 20;
    ttf->VBOs = (int *)malloc(sizeof(int) * VBOmax);
    ttf->VBOcount = 0;

    glGenVertexArrays(1, &ttf->VAO);
    glBindVertexArray(ttf->VAO);
    for (int i = 0; i < verCount; i++)
    {
        // 检查点空间是否足够
        if (pointMax < pointCount + 0x100)
        {
            pointMax += 0x100;
            point = (float *)realloc(point, sizeof(float) * pointMax * 2);
        }
        stbtt_vertex v = stbVertex[i];
        switch (v.type)
        {
        case STBTT_vmove: //    移动
            // 检查VBO空间是否足够
            if (VBOmax < ttf->VBOcount + 10)
            {
                VBOmax += 20;
                ttf->VBOs = (int *)realloc(ttf->VBOs, sizeof(int) * VBOmax);
            }
            // 添加新的断点
            ttf->VBOs[ttf->VBOcount++] = pointCount;

        case STBTT_vline: //    直线
            point[pointCount * 2] = v.x;
            point[pointCount * 2 + 1] = v.y;
            pointCount++;
            break;
        case STBTT_vcurve: //   二次贝塞尔曲线
            short x0 = point[(pointCount - 1) * 2];
            short y0 = point[(pointCount - 1) * 2 + 1];
            short x1 = v.cx;
            short y1 = v.cy;
            short x2 = v.x;
            short y2 = v.y;

            for (int j = 0; j <= 10; j++)
            {
                float t = (float)j / 10.0f;
                float xt = (1 - t) * (1 - t) * x0 + 2 * t * (1 - t) * x1 + t * t * x2;
                float yt = (1 - t) * (1 - t) * y0 + 2 * t * (1 - t) * y1 + t * t * y2;
                point[pointCount * 2] = xt;
                point[pointCount * 2 + 1] = yt;
                pointCount++;
            }
            break;
        case STBTT_vcubic: //   三次贝塞尔曲线
            short x0_cubic = point[(pointCount - 1) * 2];
            short y0_cubic = point[(pointCount - 1) * 2 + 1];
            short x1_cubic = v.cx;
            short y1_cubic = v.cy;
            short x2_cubic = v.cx1;
            short y2_cubic = v.cy1;
            short x3_cubic = v.x;
            short y3_cubic = v.y;

            for (int j = 0; j <= 10; j++)
            {
                float t = (float)j / 10.0f;
                float xt = (1 - t) * (1 - t) * (1 - t) * x0_cubic + 3 * (1 - t) * (1 - t) * t * x1_cubic + 3 * (1 - t) * t * t * x2_cubic + t * t * t * x3_cubic;
                float yt = (1 - t) * (1 - t) * (1 - t) * y0_cubic + 3 * (1 - t) * (1 - t) * t * y1_cubic + 3 * (1 - t) * t * t * y2_cubic + t * t * t * y3_cubic;

                point[pointCount * 2] = xt;
                point[pointCount * 2 + 1] = yt;
                pointCount++;
            }
            break;
        default:
            break;
        }
    }
    ttf->VBOs[ttf->VBOcount++] = pointCount;

    // 渲染VBO
    if (pointCount > 0)
    {
        glGenBuffers(1, &ttf->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, ttf->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointCount * 2, point, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }
    else
    {
        ERROR("TTF渲染错误");
    }

    // 释放内存
    free(point);
    stbtt_FreeShape(&GUIfont.font, stbVertex);

    // 检查列表是否已满
    if (++GUIfont.textCount >= GUIfont.textMax)
    {
        // 扩展列表
        GUIfont.textMax += GUI_TTF_MAX_ADD;
        GUIfont.textList = (wchar_t *)realloc(GUIfont.textList, sizeof(wchar_t) * GUIfont.textMax);
        GUIfont.textRend = (GUIchar *)realloc(GUIfont.textRend, sizeof(GUIchar) * GUIfont.textMax);

        // 清空新扩展的列表
        memset(GUIfont.textList + GUIfont.textCount, 0, sizeof(wchar_t) * GUI_TTF_MAX_ADD);
        memset(GUIfont.textRend + GUIfont.textCount, 0, sizeof(GUIchar) * GUI_TTF_MAX_ADD);
    }

    return ttf;
}

/**
 * \brief 渲染一个文字
 * \param ttf 文本
 */
void guiTTFRenderChar(GUIchar *ttf)
{
    glBindVertexArray(ttf->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, ttf->VBO);
    for (int i = 0; i < ttf->VBOcount - 1; i++)
    {
        glDrawArrays(GL_LINE_STRIP, ttf->VBOs[i], ttf->VBOs[i + 1] - ttf->VBOs[i]);
    }

    // debug
    // glBindVertexArray(ttf->VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, ttf->VBO);
    // for (int i = 0; i < ttf->VBOcount - 1; i++)
    // {
    //     if (ttf->VBOs[i + 1] < ((int)(glfwGetTime() * 25)) % (ttf->VBOs[ttf->VBOcount - 1]))
    //         glDrawArrays(GL_LINE_STRIP, ttf->VBOs[i], ttf->VBOs[i + 1] - ttf->VBOs[i]);
    //     else
    //     {
    //         glDrawArrays(GL_LINE_STRIP, ttf->VBOs[i], ((int)(glfwGetTime() * 25)) % (ttf->VBOs[ttf->VBOcount - 1]) - ttf->VBOs[i]);
    //         break;
    //     }
    // }
}